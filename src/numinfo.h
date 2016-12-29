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

// Type of phone numbers.
// Source: https://github.com/googlei18n/libphonenumber
#define FOREACH_PHONE_TYPE(TYPE) \
        TYPE(FIXED_LINE)   \
        TYPE(MOBILE)  \
        TYPE(FIXED_LINE_OR_MOBILE)   \
        TYPE(TOLL_FREE)  \
        TYPE(PREMIUM_RATE)  \
        TYPE(SHARED_COST)  \
        TYPE(VOIP)  \
        TYPE(PERSONAL_NUMBER)  \
        TYPE(PAGER)  \
        TYPE(UAN)  \
        TYPE(VOICEMAIL)  \
        TYPE(UNKNOWN)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum phone_type {
	FOREACH_PHONE_TYPE(GENERATE_ENUM)
};

static const char *phone_type_str[] = {
	FOREACH_PHONE_TYPE(GENERATE_STRING)
};

int is_valid_number(char* number, char* country, int local);
int get_region(char* number, char* country, char *region);
int get_country_code(char* number, char* country);
enum phone_type get_number_type(char* number, char* country);

#ifdef __cplusplus
}
#endif

#endif //LIBASTPHONENUMBER_NUMINFO_H
