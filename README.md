# hacking-journey
Want to learn hacking, here is a good place to start...

This is a groupment of information I acquired through my experiences, such as:
* [LiveOverflow youtube playlist](https://www.youtube.com/watch?v=iyAyN3GFM7A&list=PLhixgUqwRTjxglIswKp9mpkfPNfHkzyeN)
* CTF challenges
* Wikipedia / RFCs / Tutorials

## TODO
* https://www.root-me.org/fr/Challenges/Programmation/
* https://www.root-me.org/fr/Challenges/Realiste/
* https://www.root-me.org/fr/Challenges/Reseau/
* https://www.root-me.org/fr/Challenges/Steganographie/
* https://www.root-me.org/fr/Challenges/Web-Client/
* https://www.root-me.org/fr/Challenges/Web-Serveur/

Android: (Decompile & reverse app)
* https://www.youtube.com/watch?v=lhRXV9LZ7bY

install jadx (it has a gui)
adb shell pm path <com.editor.app>
adb pull <path>

for activity that are exporter (marked with `android:exported="true"` in AndroidManifest.xml), they can be spawned up:
adb shell am start -n <apk>/<activity>

Traffic interception:

* https://www.youtube.com/watch?v=Ft3H-3J67UE

To review: 0x0E