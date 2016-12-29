/*
 * Libphonenmuber for Asterisk
 *
 * Copyright (c) 2016.
 *
 * Sebastien De Fauw <sdefauw@gmail.com>
 *
 */

#include <iostream>
#include <string>

#include "numinfo.h"
#include "regioninfo.h"

#include "phonenumbers/phonenumber.pb.h"
#include "phonenumbers/phonenumberutil.h"

using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::PhoneNumberUtil;

int is_valid_number(char* number, char* country, int local) {

	PhoneNumber num;
    const PhoneNumberUtil& phone_util = *PhoneNumberUtil::GetInstance();
	phone_util.Parse(number, country, &num);

    if (local) {
        return (int) phone_util.IsValidNumberForRegion(num, country);
    } else {
        return (int) phone_util.IsValidNumber(num);
    }
}

int get_region(char* number, char* country, char *region) {

    PhoneNumber num;
    const PhoneNumberUtil& phone_util = *PhoneNumberUtil::GetInstance();
    phone_util.Parse(number, country, &num);

    std::string r;
    phone_util.GetRegionCodeForNumber(num, &r);

    strcpy(region, r.c_str());

    return 0;
}

int get_country_code(char* number, char* country) {

	PhoneNumber num;
	const PhoneNumberUtil& phone_util = *PhoneNumberUtil::GetInstance();
	phone_util.Parse(number, country, &num);

	return (int) num.country_code();
}

enum phone_type get_number_type(char* number, char* country) {

	PhoneNumber num;
	const PhoneNumberUtil& phone_util = *PhoneNumberUtil::GetInstance();
	phone_util.Parse(number, country, &num);

	return (enum phone_type) phone_util.GetNumberType(num);
}