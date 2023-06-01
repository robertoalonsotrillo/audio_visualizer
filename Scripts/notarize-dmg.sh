#!/bin/bash
# Notarize dmg

if [ "$#" -ne 1 ]; then
    echo "Please supply 1 argument: source-dmg"
fi

echo "Notarizing $1 ..."

uuid=$(xcrun altool --notarize-app --primary-bundle-id "com.metabow.audio-feature-visualiser" --asc-provider 9XT2X2HYD7 --username "adam@nodeaudio.com" --password "@keychain:Notarize" --file "$1" 2>&1 | grep 'RequestUUID' | awk '{ print $3 }')

if [ "$uuid" = "" ]; then
  echo "RequestUUID empty, something went wrong"
  exit 1
fi

echo "RequestUUID: $uuid" # Display identifier string
sleep 15
while :
do
    fullstatus=$(xcrun altool --notarization-info "$uuid" --asc-provider 9XT2X2HYD7 --username "adam@nodeaudio.com" --password "@keychain:Notarize" 2>&1)  # get the status
    status1=$(echo "$fullstatus" | grep 'Status\:' | awk '{ print $2 }')
    if [ "$status1" = "success" ]; then
        xcrun stapler staple "$1"   #  staple the ticket
        xcrun stapler validate -v "$1"
        echo "dmg Notarization success"
        break
    elif [ "$status1" = "in" ]; then
        echo "dmg Notarization still in progress, sleeping for 15 seconds and trying again"
        sleep 15
    else
        echo "dmg Notarization failed fullstatus below"
        echo "$fullstatus"
        exit 1
    fi
done

