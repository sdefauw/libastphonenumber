/*
 * Libphonenmuber for Asterisk
 *
 * Copyright (c) 2016.
 *
 * Sebastien De Fauw <sdefauw@gmail.com>
 *
 */

#ifndef LIBASTPHONENUMBER_NUMINFO_H
#define LIBASTPHONENUMBER_NUMINFO_H

#ifdef __cplusplus
extern "C" {
#endif

int is_valid_number(char* number, char* country, int local);
int get_region(char* number, char* country, char *region);
int get_country_code(char* number, char* country);

#ifdef __cplusplus
}
#endif

#endif //LIBASTPHONENUMBER_NUMINFO_H
