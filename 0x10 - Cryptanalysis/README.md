# Cryptanalysis

Short list of possible decryption tools:

* Base64
   - Javascript: `atob(btoa("Hello"))`
   - Online: https://www.base64decode.org/
* HEX to ASCII
   - Python 2: `"48656c6c6f0a".decode("hex")`
   - Command line: `echo "48656c6c6f0a" | xxd -r -ps`
   - Online: http://www.asciitohex.com/
* UUEncode
   - Online: https://www.dcode.fr/encodage-uu
   - Python 3: `import binascii; binascii.a2b_uu(c)`
* Hashing
   - Command line: https://tools.kali.org/password-attacks/findmyhash
* Online
   - https://md5decrypt.net/
   - https://md5hashing.net/
   - https://crackstation.net/ (NTLM, md5, sha, MySQL)

## RSA

To decode RSA encrypted content, you can use `openssl`:
```
openssl enc -d -aes-256-cbc -in <file> -out <output> -pass pass:<key> -nosalt -iv <iv>
```
Initialisation vectore (iv), if not set, will be assigned to: `00000000000000000000000000000000`


## John the Ripper

[John the ripper](https://github.com/openwall/john) is an open-source password cracker used for dictionary-style attack.

On Kali, installed word list are located in: `/usr/share/wordlists`

```
john --format=<line_return_format> --wordlist=<word_list> <hash_dump>
```
