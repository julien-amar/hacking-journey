# Android

Android apps ship as **APK** files (a ZIP archive containing compiled Dalvik
bytecode in `classes.dex`, resources, and the `AndroidManifest.xml`). Reversing
one means unpacking that archive and turning the bytecode back into readable
Java/Smali.

> Work on an emulator or a device you own. Many steps below need **root** and/or
> a **debuggable** build.

## Tooling setup

The [`setup_android-ssl-pining.sh`](setup_android-ssl-pining.sh) script installs
the toolchain used in this chapter: `adb`/`fastboot`, and the Python packages
`frida`, `frida-tools` and `objection`.

The Android Debug Bridge (`adb`) is how you talk to a device:

```sh
adb devices                       # list connected devices/emulators
adb root                          # restart adbd as root (if allowed)
adb shell                         # open a shell on the device
adb push <local> <remote>         # copy a file to the device
adb pull <remote> <local>         # copy a file from the device
adb install <apk>                 # install an APK
```

## Getting the APK off a device

```sh
adb shell pm path <com.editor.app>   # find the installed APK's path(s)
adb pull <path>                      # pull it to your machine
```

## APK decompilation

A few complementary tools, easiest first:

* **[jadx](https://github.com/skylot/jadx)** — the quickest option. `jadx-gui`
  opens an APK and shows reconstructed Java directly; great for browsing code.
* **[apktool](https://ibotpeaches.github.io/Apktool/)** — decodes resources and
  the manifest, and disassembles the bytecode to **Smali**. Use it when you want
  to *modify and repackage* an app: `apktool d app.apk`, edit, then
  `apktool b app` and re-sign.
* **[dex2jar](https://sourceforge.net/projects/dex2jar/)** +
  **[JD-GUI](http://java-decompiler.github.io/)** — the older two-step route:
  1. convert the APK to a JAR: `dex2jar <apk>`
  2. open the resulting JAR in JD-GUI to read the Java source.

### Spawning exported activities

Activities marked `android:exported="true"` in `AndroidManifest.xml` can be
launched directly, which sometimes bypasses the app's normal entry flow:

```sh
adb shell am start -n <package>/<activity>
```

## Dynamic instrumentation with Frida

[Frida](https://frida.re/) injects a JavaScript engine into a running app so you
can inspect and rewrite its behaviour at runtime ("hooking").

First start the Frida server on the device (or use
[`install_frida-server.sh`](install_frida-server.sh), which detects the device
CPU ABI, downloads the matching `frida-server`, and pushes it over):

```sh
adb root
adb push frida-server /data/local/tmp/frida-server
adb shell "chmod 755 /data/local/tmp/frida-server"
adb shell "/data/local/tmp/frida-server &"
```

### Example: hooking outgoing SMS

Create `hook.js` to intercept `SmsManager.sendTextMessage` and log its
arguments:

```js
Java.perform(function () {
  send("Starting hooks");
  var SmsManager = Java.use("android.telephony.SmsManager");

  SmsManager.getDefault().sendTextMessage.overload(
    "java.lang.String", "java.lang.String", "java.lang.String",
    "android.app.PendingIntent", "android.app.PendingIntent"
  ).implementation = function (destination, source, text, sentIntent, deliveryIntent) {
    send("phone number : " + destination);
    send("sms value    : " + text);
    return true;   // swallow the call instead of letting it send
  };
});
```

Run it, spawning the target app (`-U` = USB device, `-f` = spawn & attach):

```sh
frida -U -f <package> -l hook.js --no-pause
```

> **[objection](https://github.com/sensepost/objection)** builds on Frida and
> gives you many of these capabilities (including SSL-pinning bypass) without
> writing scripts: `objection -g <package> explore`.

## SSL pinning (and bypassing it for traffic interception)

To inspect an app's HTTPS traffic you put an intercepting proxy (Burp Suite,
mitmproxy, Fiddler) in the middle and trust its CA certificate on the device.
Many apps defeat this with **certificate pinning**: they ship the expected
certificate/public key and reject anything else — so even a trusted proxy CA is
refused, and you see no traffic.

The workaround is to hook the pinning checks at runtime with Frida so they
always pass. The [`inject_application.sh`](inject_application.sh) script does
exactly this using a well-known community script:

```sh
frida -U --codeshare pcipolloni/universal-android-ssl-pinning-bypass-with-frida \
      -f <package> --no-pause
```

End-to-end, the workflow the helper scripts automate is:

1. Run your proxy (e.g. Fiddler/Burp) and note its CA certificate.
2. `install_frida-server.sh` — download the proxy's CA cert and a matching
   `frida-server`, push both to the device, and fix their permissions.
3. Set the device's Wi-Fi proxy to point at your machine.
4. `inject_application.sh` — start `frida-server`, pick the target app, and
   inject the universal SSL-pinning-bypass script.
5. Use the app; its HTTPS requests now appear in your proxy.

## Installing split / XAPK bundles

Some apps ship as an **XAPK** (a ZIP of a base APK plus split APKs — split by
CPU ABI, screen density and language). A plain `adb install` rejects them: the
base and all splits must be installed together in one transaction:

```sh
adb install-multiple base.apk split_config.*.apk
```

[`install_application.sh`](install_application.sh) automates both cases — it
installs a plain `.apk` directly, or unzips an `.xapk`, runs `install-multiple`
on every APK inside, and grants any extra permissions listed in the bundle's
`manifest.json`:

```sh
./install_application.sh app.xapk
```

## Online analysis

These services dynamically run a sample and report its behaviour (useful for
suspected-malware triage):

* https://www.joesandbox.com/#android
* https://mobsf.github.io/Mobile-Security-Framework-MobSF/ (MobSF — can also be
  run locally for static + dynamic analysis)

_Video walkthroughs that inspired this chapter:_
* _Decompiling & reversing an app: https://www.youtube.com/watch?v=lhRXV9LZ7bY_
* _Traffic interception: https://www.youtube.com/watch?v=Ft3H-3J67UE_
