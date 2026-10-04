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
