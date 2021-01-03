# Cryptanalysis

This list is just a short list of possible decryption tools:

* HEX to ASCII
   - Python 2: `"48656c6c6f0a".decode("hex")`
   - Command line: `echo "48656c6c6f0a" | xxd -r -ps`
   - Online: http://www.asciitohex.com/
* UUEncode
   - Online: https://www.dcode.fr/encodage-uu
   - Python 3: `import binascii; binascii.a2b_uu(c)`
* MD5
   - Command line: https://tools.kali.org/password-attacks/findmyhash
   - Online: https://md5decrypt.net/
* SHA
   - Command line: https://tools.kali.org/password-attacks/findmyhash
   - Online: https://crackstation.net/ & https://md5decrypt.net/Sha256/



