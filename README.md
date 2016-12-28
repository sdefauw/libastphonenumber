# Asterisk Libphonenumber

## Manual installation

Compile the main library that is linked with asterisk:
```
mkdir build
cd build
cmake ..
make
make install
```

Add asterisk application:
```
cp asterisk/app_numformat.c <ASTERISK_SOURCE_CODE_PATH>/apps/
```
