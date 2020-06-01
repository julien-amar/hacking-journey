#!/bin/bash

FRIDA_VERSION=12.8.20
FRIDA_REMOTE_PATH=/data/local/tmp
TMP_FOLDER=/tmp/ssl-pinning
FIDDLER_CERT=http://localhost:8888/FiddlerRoot.cer

rm -rf $TMP_FOLDER
mkdir $TMP_FOLDER

echo "Downloading Fiddler certificate"

until $(wget -O /dev/null -q $FIDDLER_CERT); do
    printf '.'
    sleep 5
done

wget $FIDDLER_CERT -O $TMP_FOLDER/FiddlerRoot.cer -q

echo "Connecting to device"

until adb get-state 1>/dev/null 2>&1
do
    printf '.'
    sleep 5
done

echo "Downloading frida-server (version: $FRIDA_VERSION)"

ARCH=`adb shell getprop ro.product.cpu.abi`

wget "https://github.com/frida/frida/releases/download/"$FRIDA_VERSION"/frida-server-"$FRIDA_VERSION"-android-"$ARCH".xz" -O $TMP_FOLDER/frida-server.xz -q
unxz $TMP_FOLDER/frida-server.xz

echo "Pushing frida-server & Fiddler certificate to device"

adb push $TMP_FOLDER $FRIDA_REMOTE_PATH

echo "Setting permission"

adb shell chmod 755 $FRIDA_REMOTE_PATH/ssl-pinning
adb shell chmod 555 $FRIDA_REMOTE_PATH/ssl-pinning/frida-server
adb shell chmod 644 $FRIDA_REMOTE_PATH/ssl-pinning/FiddlerRoot.cer

echo "Cleaning up"

rm -rf $TMP_FOLDER

echo "Done"

# frida --codeshare pcipolloni/universal-android-ssl-pinning-bypass-with-frida -f YOUR_BINARY
