/*
 * Libphonenmuber for Asterisk
 *
 * Copyright (c) 2016.
 *
 * Sebastien De Fauw <sdefauw@gmail.com>
 *
 */


#ifndef LIBPHONENUMBER_LIBASTFORMAT_H
#define LIBPHONENUMBER_LIBASTFORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

    int e164_format(char* number, char* country, char* formatted);

#ifdef __cplusplus
}
#endif

#endif //LIBPHONENUMBER_LIBASTFORMAT_H
