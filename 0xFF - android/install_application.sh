#!/bin/bash

APK=

rm -rf $TMP_FOLDER
mkdir $TMP_FOLDER

echo "Connecting to device"

until adb get-state 1>/dev/null 2>&1
do
    printf '.'
    sleep 5
done

see https://github.com/cubehouse/xapkinstall/blob/cc0ae2bb90b93dc43a383cead4e5419ba288362d/index.js#L177

* unzip le xapk
* parse manifest.json
* extract manifest.split_apks.where(x => x.id == 'base').select(x => x.file)
* extract manifest.split_apks.where(x => x.id != 'base').select(x => x.file)
* adb install-multiple [... apks]
* adb shell pm grant {manifest.package_name} {permission}

