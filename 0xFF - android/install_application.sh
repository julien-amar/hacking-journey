#!/bin/bash
#
# Install an app onto a connected device/emulator.
#
# Handles two cases:
#   * a single .apk            -> adb install
#   * an .xapk / split bundle  -> unzip and adb install-multiple
#
# An XAPK is just a ZIP containing a base APK plus "split" APKs (per-ABI,
# per-density, per-language). A plain `adb install` rejects them, so the base
# and all splits must be installed together with `install-multiple`.
#
# Usage: ./install_application.sh <app.apk | app.xapk>

set -euo pipefail

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <app.apk | app.xapk>"
    exit 1
fi

INPUT="$1"
TMP_FOLDER="$(mktemp -d)"
trap 'rm -rf "$TMP_FOLDER"' EXIT

echo "Waiting for a device..."
adb wait-for-device
echo "Device connected."

case "$INPUT" in
    *.apk)
        echo "Installing single APK: $INPUT"
        adb install -r "$INPUT"
        ;;

    *.xapk|*.zip)
        echo "Extracting bundle: $INPUT"
        unzip -o -q "$INPUT" -d "$TMP_FOLDER"

        # Install the base APK together with every split APK in one transaction.
        mapfile -t APKS < <(find "$TMP_FOLDER" -maxdepth 1 -name '*.apk')

        if [ "${#APKS[@]}" -eq 0 ]; then
            echo "[ERROR] No .apk files found inside the bundle."
            exit 1
        fi

        echo "Installing ${#APKS[@]} APK(s) with install-multiple..."
        adb install-multiple -r "${APKS[@]}"

        # Some bundles ship a manifest.json describing extra runtime permissions
        # to grant and OBB data files to push. Handle them if present.
        MANIFEST="$TMP_FOLDER/manifest.json"
        if [ -f "$MANIFEST" ] && command -v jq >/dev/null 2>&1; then
            PKG="$(jq -r '.package_name // empty' "$MANIFEST")"
            if [ -n "$PKG" ]; then
                jq -r '.permissions[]? ' "$MANIFEST" | while read -r perm; do
                    [ -n "$perm" ] && adb shell pm grant "$PKG" "$perm" || true
                done
            fi
        fi
        ;;

    *)
        echo "[ERROR] Unsupported file type: $INPUT (expected .apk or .xapk)"
        exit 1
        ;;
esac

echo "Done."
