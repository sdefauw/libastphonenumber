
#ifndef LIBPHONENUMBER_LIBASTFORMAT_H
#define LIBPHONENUMBER_LIBASTFORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Type of output phone format
 *  Example: 0473012345 in BE
 */
enum phone_format {
    // +32473012345
    PLUSE164,

    // 32473012345
    E164,

    // +32 473 01 23 45
    INTERNATIONAL,

    // 003247012345
    EXTERNAL_CALL,

    // 0473 01 23 45
    NATIONAL,

    // 0473012345
    NATIONAL_COMPACT,

    // 473012345
    NATIONAL_SHORT
};

/**
 * Format a number into a specific phone format and based on a country.
 *
 * @param number		Number to format
 * @param country		Two-letter country (ISO 3166-1) where the national number will be taken into account.
 * @param type			Type of output phone format. @see enum phone_format
 * @param formatted		Number that will be formatted.
 * @return 				0 if there is no issue, otherwise 1
 */
int num_format(char *number, char *country, enum phone_format type, char *formatted);

#ifdef __cplusplus
}
#endif

#endif //LIBPHONENUMBER_LIBASTFORMAT_H
