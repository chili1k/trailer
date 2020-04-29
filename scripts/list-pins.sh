#!/bin/sh

MY_DIR="$(dirname "$0")"

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
  echo "Prints all used pins"
  exit 0
fi

gcc -fpreprocessed -dD -E "$MY_DIR/../Config.h" 2>/dev/null | grep -E -o "\s[0-9]{1,2},?$" | sed -e "s/,//g" -e "s/ //g" | sort -n
