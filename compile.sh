#!/bin/bash

set -e

CFLAGS="-Wall -ansi"
OPTCFLAGS="${CFLAGS} -O2"
DBGCFLAGS="${CFLAGS} -ggdb3 -DDEBUG"
# DBGCFLAGS+=" -DDEBUG_SECRETS"

rm -f *.exe *.dbg

gcc ${OPTCFLAGS} main.c secret.c mt19937-64.c -o static_key.exe
gcc ${DBGCFLAGS} main.c secret.c mt19937-64.c -o static_key.dbg

gcc ${OPTCFLAGS} -DTOTP main.c secret.c manipulation.c mt19937-64.c -o totp.exe
gcc ${DBGCFLAGS} -DTOTP main.c secret.c manipulation.c mt19937-64.c -o totp.dbg

gcc ${OPTCFLAGS} -DHOTP main.c secret.c manipulation.c mt19937-64.c -o hotp.exe
gcc ${DBGCFLAGS} -DHOTP main.c secret.c manipulation.c mt19937-64.c -o hotp.dbg

strip *.exe
