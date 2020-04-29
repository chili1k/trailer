#!/bin/sh

MY_DIR="$(dirname "$0")"

VER_FILE="$MY_DIR/../Ver.h"
VER=$(grep "\sVER\s" $VER_FILE | cut -f3 -d' ' | sed "s/\"//g")
NEW_VER=$(bc <<< "$VER + 0.1" | awk '{printf "%.1f", $0}')
RELEASE_DATE=$(date +"%b %Y")

sed -i \
    -e "s/VER *\"$VER\"/VER \"$NEW_VER\"/g" \
    -e "s/RELEASE_DATE *\".*\"/RELEASE_DATE \"$RELEASE_DATE\"/g" \
    $VER_FILE
 git add "$MY_DIR/../." && git commit -m "Release $NEW_VER"
 git tag $NEW_VER
 git push origin master
 git push origin $NEW_VER
