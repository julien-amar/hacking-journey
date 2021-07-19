#!/bin/bash

FRIDA_REMOTE_PATH=/data/local/tmp/ssl-pinning
TMP_FOLDER=/tmp/ssl-pinning
FRIDA_SERVER_LOG=$TMP_FOLDER/frida-server.log

rm -rf $TMP_FOLDER
mkdir $TMP_FOLDER

echo "Connecting to device"

until adb get-state 1>/dev/null 2>&1
do
    printf '.'
    sleep 5
done

echo "Retrieve list of running process"

if frida-ps -U  1>/dev/null 2>&1 ; then
else
	echo "Start frida server"

	adb shell $FRIDA_REMOTE_PATH/frida-server 1> $FRIDA_SERVER_LOG 2>&1 &
fi

frida-ps -U

echo "Which application do you want to inject the script to ?"
read app

echo "Inject script into "

frida --codeshare pcipolloni/universal-android-ssl-pinning-bypass-with-frida -f $app --no-paus
