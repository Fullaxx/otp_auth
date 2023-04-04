# otp_auth
A set of TOTP and HOTP examples that utilize [MT64](http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/VERSIONS/C-LANG/mt19937-64.c)

## Compile the code
```
./compile.sh
```

## Create the shared secret key file
This is your secret key. Keep it private.
This file will seed the MT PRNG.
./gen_secret.sh: <openssl|dd> <file>
```
./gen_secret.sh openssl 256.bin
<or>
./gen_secret.sh dd 256.bin
```

## Static Key
Run the MT64 to generate a static key based only on the secret keyfile. \
This is just used to show the usage of the MT64 algorithm. \
This will not produce a "One-Time" value. The input will not be manipulated. \
It will reproduce consistent output from the same input. \
Set the spin value to any unsigned integer. \
./static_key.exe: <SECRET FILE> <SPIN VALUE>
```
./static_key.exe 256.bin 1000000
```

## Time-based OTP
The totp example will take the current time and feed it into the MT seed.
./totp.exe: <SECRET FILE> <SPIN VALUE>
```
./totp.exe 256.bin 1000000
TOTP: B4DE50FC4428E12A6211A64C59244FE168B7FF0BD800056DD353EF9CEA6E59A9 (valid for   34s)
<wait 60s>
./totp.exe 256.bin 1000000
TOTP: EE7A8C7603AE9E38DD9FA38E8BBE75F8735B2417159C55D338621C9A66C95ED7 (valid for   35s)
```

## HMAC-based OTP
The hotp example will take an external counter value and feed it into the MT seed. \
Currently any resyncing and validation window handling is not part of this code. \
./hotp.exe: <SECRET FILE> <SPIN VALUE>
```
HOTP_COUNTER=47 ./hotp.exe 256.bin 1000000
HOTP: 9ECFCD099E264C6CAFD09FFBB6B6D09C1A0D079AC32B8FF2A719294C30AC1630

HOTP_COUNTER=48 ./hotp.exe 256.bin 1000000
HOTP: 43E55DFDFF921E45EC494A18CECB7201B550E0818882D0200AFEE950C6D58975
```

## Design Decisions
This code was not designed to be compliant with any RFC or specification. It was strictly designed to generate OTP codes for use in an external authentication system.
Currently the output is 32 bytes of binary data (represented as 64 hex digits). This can be manipulated higher or lower by changing OUTPUT_ULONGS in main.c

## More Info
[Mersenne Twister](https://en.wikipedia.org/wiki/Mersenne_Twister)
[TOTP](https://en.wikipedia.org/wiki/Time-based_one-time_password)
[HOTP](https://en.wikipedia.org/wiki/HMAC-based_one-time_password)
