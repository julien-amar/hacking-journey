# Cryptanalysis

In CTFs you constantly meet data that has been **encoded** (reversible without a
key, e.g. Base64/hex — just a different representation) or **encrypted**
(needs a key). The first job is always to recognise which you're looking at:
encodings can simply be decoded; encryption needs the key, a weakness, or a
brute-force/dictionary attack.

> **One tool to try first:** [CyberChef](https://gchq.github.io/CyberChef/) — a
> browser-based "Swiss-army knife" that chains decoders, decrypters and
> analysers together, with a "Magic" operation that guesses the encoding for
> you. It covers almost everything in the list below.

## Common encodings

* **Base64**
   - JavaScript: `atob(btoa("Hello"))` (`btoa` encodes, `atob` decodes)
   - Command line: `base64 -d <<< "SGVsbG8="`
   - Online: https://www.base64decode.org/
* **Hex ⇆ ASCII**
   - Python 3: `bytes.fromhex("48656c6c6f0a")` → `b'Hello\n'`
   - Command line: `echo "48656c6c6f0a" | xxd -r -ps`
   - Online: http://www.asciitohex.com/
* **UUencode**
   - Python 3: `import binascii; binascii.a2b_uu(line)`
   - Online: https://www.dcode.fr/encodage-uu
* **ROT13 / Caesar** (letters shifted by a fixed amount)
   - Command line: `echo "Uryyb" | tr 'A-Za-z' 'N-ZA-Mn-za-m'`
   - dcode auto-solves Caesar/Vigenère: https://www.dcode.fr/

## Hashes

A hash is one-way, so you can't "decrypt" it — you recover the original only by
hashing candidate inputs and comparing (dictionary or brute-force), or by
looking it up in a database of pre-computed hashes.

* **Identify the hash type first:** `hashid <hash>` or `hash-identifier`.
* **Online lookup databases:**
   - https://crackstation.net/ (NTLM, MD5, SHA family, MySQL)
   - https://md5decrypt.net/
   - https://md5hashing.net/

## Symmetric encryption with OpenSSL

> **Correction / note:** the command below uses **AES** (a symmetric cipher —
> the same key encrypts and decrypts), not RSA. RSA is *asymmetric* (separate
> public/private keys) and is handled with `openssl rsautl` / `openssl pkeyutl`
> and a key file, not a passphrase. Earlier notes mislabelled this as "RSA".

To decrypt AES-256-CBC content with OpenSSL:

```sh
openssl enc -d -aes-256-cbc -in <file> -out <output> -pass pass:<key> -nosalt -iv <iv>
```

If no initialisation vector (IV) is given, it defaults to all zeros:
`00000000000000000000000000000000`.

## Why crypto breaks (fundamentals)

In CTFs you rarely break the *maths* of a cipher — you exploit how it was
*used*. The recurring weaknesses:

### ECB mode leaks patterns

Block ciphers split data into fixed-size blocks. In **ECB** mode each block is
encrypted independently, so **identical plaintext blocks produce identical
ciphertext blocks**. Structure in the input survives encryption — the famous
"[ECB penguin](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Electronic_codebook_(ECB))"
image is still clearly a penguin after encryption. Tell-tale sign: repeating
16-byte chunks in the ciphertext. Modes like CBC/CTR/GCM fix this with an IV so
equal blocks differ.

### XOR "encryption"

XOR with a repeating key is extremely common in challenges and trivially
reversible, because XOR is its own inverse: `cipher XOR key = plain` and
`cipher XOR plain = key`.

```python
# If you know (or can guess) part of the plaintext, you recover the key:
key = bytes(c ^ p for c, p in zip(cipher, known_plaintext))
```

Single-byte XOR is brute-forceable over all 256 keys; repeating-key XOR is
broken by finding the key length (Hamming distance) then solving each position
as single-byte XOR — exactly the [Cryptopals](https://cryptopals.com/) set 1
exercises.

### Weak / predictable randomness

If keys, IVs or tokens come from a predictable source — `rand()` seeded with
`time(NULL)` (chapter `0x0C`), a small keyspace, or a non-cryptographic PRNG —
you can regenerate or brute-force them. Always ask *where did this secret come
from?*

### Hash pitfalls

* **MD5 / SHA-1** are broken for collision resistance — never trust them for
  integrity/signatures.
* **Unsalted** password hashes fall to the lookup databases above instantly.
* **Length-extension**: with MD5/SHA-1/SHA-2, knowing `H(secret ‖ message)` and
  the length of `secret` lets you compute `H(secret ‖ message ‖ padding ‖ extra)`
  *without* knowing the secret. HMAC exists to prevent this.

### A good training ground

[Cryptopals](https://cryptopals.com/) walks you through all of the above by
building the attacks yourself — highly recommended.

## Password cracking

Both tools below take a list of hashes and a wordlist, hash each candidate, and
report any matches. John is CPU-based and very convenient; hashcat is
GPU-accelerated and much faster on large jobs.

### John the Ripper

[John the Ripper](https://github.com/openwall/john) is an open-source password
cracker used for dictionary-style attacks.

On Kali, the installed wordlists live in `/usr/share/wordlists` (the famous
`rockyou.txt` is there — you may need to `gunzip` it first).

```sh
john --format=<format> --wordlist=<wordlist> <hash_file>
john --show <hash_file>          # show already-cracked passwords
```

### hashcat

[hashcat](https://hashcat.net/hashcat/) is the GPU-powered equivalent.

```sh
# -m selects the hash mode (e.g. 0 = MD5, 100 = SHA1, 1800 = sha512crypt),
# -a 0 is dictionary attack mode.
hashcat -m 0 -a 0 hashes.txt /usr/share/wordlists/rockyou.txt
```

Run `hashcat --help | less` to find the `-m` mode number for a given hash type.
