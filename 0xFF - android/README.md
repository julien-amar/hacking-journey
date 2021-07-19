# Android

## APK Decompilation

To decompile an APK, you will need to use those 2 tools:
* [dex2jar](https://sourceforge.net/projects/dex2jar/) enables to convert Dalvik Executable format to jar files.
* [JD-GUI](http://java-decompiler.github.io/) is a standalone graphical utility that displays Java source codes of “.class” files. You can browse the reconstructed source code with the JD-GUI for instant access to methods and fields.

The process rely on those two steps:
1. Convert APK to JAR using: `dex2jar <apk>`
2. Open JAR with JD-GUI

## Hooking (SMS)

First of all you have to start frida server on your device (or using the `install_frida-server.sh` script):
```
adb root
adb push frida-server-12.6.16-android-x86 /data/local/tmp/frida/frida-server
adb shell "/data/local/tmp/frida/frida-server"
adb shell "/data/local/tmp/frida-server &"
```

Create a file `hook.js` to ask frida, to hook the required function:
```js
Java.perform(function () {
  send("Starting hooks");
  var sendSMS = Java.use("android.telephony.SmsManager");

  sendSMS.getDefault().sendTextMessage.overload("java.lang.String","java.lang.String", "java.lang.String", "android.app.PendingIntent", "android.app.PendingIntent").implementation = function(var1, var2, var3, var4, var5) {
    send("phone number : " + var1);
    send("sms value : " + var3);
    return true;
  };
});
```

Execute frida on your host
```
frida -U -f <apk> -l hook.js --no-pause
```

## SSL pinning

TBD

## Online analysis

The following website enables to dynamically analyze binary behavior:
* https://www.joesandbox.com/#android