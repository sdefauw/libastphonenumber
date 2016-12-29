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

/*
 * Type of output format
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


int num_format(char* number, char* country, enum phone_format type, char* formatted);

#ifdef __cplusplus
}
#endif

#endif //LIBPHONENUMBER_LIBASTFORMAT_H
