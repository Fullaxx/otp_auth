#!/bin/bash

bail()
{
  >&2 echo "$1"
  exit 1
}

if [ "$#" != "2" ]; then
  bail "$0: <openssl|dd> <file>"
fi

case $1 in
  openssl) openssl rand 256 >$2;;
       dd) dd if=/dev/random of=$2 bs=1 count=256;;
        *) bail "$0: <openssl|dd> <file>";;
esac
