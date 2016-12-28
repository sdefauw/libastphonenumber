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

#include "libastformat.h"
#include "phonenumbers/base/logging.h"
#include "phonenumbers/phonenumber.pb.h"
#include "phonenumbers/phonenumberutil.h"


using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::PhoneNumberUtil;

int e164_format(char* number, char* country, char* formatted) {

    strcpy(formatted, number);

    PhoneNumber pnumber;

    const PhoneNumberUtil& phone_util = *PhoneNumberUtil::GetInstance();
    phone_util.Parse(number, country, &pnumber);
    if (!phone_util.IsValidNumber(pnumber)) {
        strcpy(formatted, number);
        return -1;
    }

    std::string formatted_number;
    phone_util.Format(pnumber, PhoneNumberUtil::E164, &formatted_number);
    strcpy(formatted, formatted_number.c_str());

    return 0;

}
