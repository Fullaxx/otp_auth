#!/bin/bash

SECRETFILE="256.bin"
SPINVALUE="3000000"

if [ ! -r ${SECRETFILE} ]; then
  ./gen_secret.sh openssl ${SECRETFILE}
fi

while [ true ]; do
  ./totp.dbg ${SECRETFILE} ${SPINVALUE}
  sleep 1
done
