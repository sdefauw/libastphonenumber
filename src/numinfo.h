
#ifndef LIBASTPHONENUMBER_NUMINFO_H
#define LIBASTPHONENUMBER_NUMINFO_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Type of phone numbers.
 * @see https://github.com/googlei18n/libphonenumber
 */
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

/**
 * Check the validity of a number.
 *
 * @param number	 Number to check
 * @param country	 Two-letter country (ISO 3166-1) where the national number will be taken into account.
 * @param local		 1 if the number is valid in the country specified otherwise the check is done in national and
 * 					 international.
 * @param short_code 0 Don't check if the number is a short number
 *                   1 Check only if it's a short number
 *                   2 Check if it's a short number or a classic number
 * @return			 1 if the number is valid and 0 if it is invalid.
 */
int is_valid_number(char *number, char *country, int local, int short_code);

/**
 * Get the country based on the number's information.
 *
 * @param number	Information is based on this number.
 * @param country	Two-letter country (ISO 3166-1) where the national number will be taken into account.
 * @param region	Two-letter country (ISO 3166-1) of the number. If the software can't get the region ZZ, is returned.
 * @return 			0 if there is no issue, otherwise 1
 */
int get_region(char *number, char *country, char *region);

/**
 * Extract the country calling code of a number.
 *
 * @param number	Information is based on this number.
 * @param country	Two-letter country (ISO 3166-1) where the national number will be taken into account.
 * @return			Return the country calling code from the number.
 */
int get_country_code(char *number, char *country);

/**
 * Get information about the nature of the phone number.
 *
 * @param number	Information is based on this number.
 * @param country	Two-letter country (ISO 3166-1) where the national number will be taken into account.
 * @return			Return the type of number.
 * @see 	enum phone_type
 */
enum phone_type get_number_type(char *number, char *country);

#ifdef __cplusplus
}
#endif

#endif //LIBASTPHONENUMBER_NUMINFO_H
