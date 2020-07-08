
#include <iostream>
#include <string>

#include "numinfo.h"
#include "regioninfo.h"

#include "phonenumbers/phonenumber.pb.h"
#include "phonenumbers/phonenumberutil.h"
#include "phonenumbers/shortnumberinfo.h"

using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::PhoneNumberUtil;
using i18n::phonenumbers::ShortNumberInfo;

int is_valid_number(char *number, char *country, int local, int short_code) {

    PhoneNumber num;
    const PhoneNumberUtil &phone_util = *PhoneNumberUtil::GetInstance();
    const ShortNumberInfo &short_info = ShortNumberInfo();
    bool is_valid;
    bool is_valid_short_number;
    phone_util.Parse(number, country, &num);

    if (local) {
        is_valid = phone_util.IsValidNumberForRegion(num, country);
    } else {
        is_valid = phone_util.IsValidNumber(num);
    }

    if (short_code > 0) {
        if (local) {
            is_valid_short_number = short_info.IsValidShortNumberForRegion(num, country);
        } else {
            is_valid_short_number = short_info.IsValidShortNumber(num);
        }
        is_valid = (short_code == 2) ? (is_valid || is_valid_short_number) : is_valid_short_number;
    }
    return (int) is_valid;
}

int get_region(char *number, char *country, char *region) {

    PhoneNumber num;
    const PhoneNumberUtil &phone_util = *PhoneNumberUtil::GetInstance();
    phone_util.Parse(number, country, &num);

    std::string r;
    phone_util.GetRegionCodeForNumber(num, &r);

    strcpy(region, r.c_str());

    return 0;
}

int get_country_code(char *number, char *country) {

    PhoneNumber num;
    const PhoneNumberUtil &phone_util = *PhoneNumberUtil::GetInstance();
    phone_util.Parse(number, country, &num);

    return (int) num.country_code();
}

enum phone_type get_number_type(char *number, char *country) {

    PhoneNumber num;
    const PhoneNumberUtil &phone_util = *PhoneNumberUtil::GetInstance();
    phone_util.Parse(number, country, &num);

    return (enum phone_type) phone_util.GetNumberType(num);
}