# Cryptanalysis

This list is just a short list of possible decryption tools:

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
   - Online:  & hash/sha512
* Online
   - https://md5decrypt.net/
   - https://md5hashing.net/
   - https://crackstation.net/



