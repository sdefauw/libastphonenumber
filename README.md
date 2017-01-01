# Asterisk Libphonenumber
libastphonenumber is a new library that exposes the [libphonenumber](https://github.com/googlei18n/libphonenumber) into [Asterisk](http://www.asterisk.org/).

## Highlights of asterisk functions
 * ```FORMAT_NUM``` - Get formatted numbers in specific format based on a country.
 * ```IS_VALID_NUM``` - Check validity of a number.
 * ```NUM_INFO``` - Get information about a number: Country calling code, Country of the phone and Type of phone number. 
 * ```REGION_INFO``` - Get information about a country/region.

## Installation
The installation is processed in two parts:
 * A library is added (libastphonenumber) on the device to allow the link between the libphonenumber and Asterisk.
 * Adding a new Asterisk application.

### Requirements
The installation of following programs are required:
 - The C++ part of [libphonenumner](https://github.com/googlei18n/libphonenumber/tree/master/cpp)
 - CMake
 - A gnu compiler compatible with c++11
 - Asterisk source code and be able to compile it.

### Manual lib installation
Compile the main library libastphonenumber:
```
mkdir build
cd build
cmake ..
make
make install
```

### Asterisk installation
Add Asterisk application to the source code:
```
cp asterisk/app_numformat.c <ASTERISK_SOURCE_CODE_PATH>/apps/
```
Compile Asterisk [source code](https://wiki.asterisk.org/wiki/display/AST/Installing+Asterisk+From+Source).
