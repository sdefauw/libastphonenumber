
#include <iostream>
#include <string>

#include "formatting.h"
#include "phonenumbers/base/logging.h"
#include "phonenumbers/phonenumber.pb.h"
#include "phonenumbers/phonenumberutil.h"

#define REMOVE_SPACES(x) x.erase(std::remove(x.begin(), x.end(), ' '), x.end())

using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::PhoneNumberUtil;

int num_format(char* number, char* country, enum phone_format type, char* formatted) {

    strcpy(formatted, number);

    PhoneNumber pnumber;

    const PhoneNumberUtil& phone_util = *PhoneNumberUtil::GetInstance();
    phone_util.Parse(number, country, &pnumber);
    if (!phone_util.IsValidNumber(pnumber)) {
        strcpy(formatted, number);
        return -1;
    }

    std::string formatted_number;
	switch (type){
		case PLUSE164:
			phone_util.Format(pnumber, PhoneNumberUtil::E164, &formatted_number);
			break;
		case E164:
			phone_util.Format(pnumber, PhoneNumberUtil::E164, &formatted_number);
			if(formatted_number.at(0) == '+') {
				formatted_number = formatted_number.erase(0, 1);
			}
			break;
		case INTERNATIONAL:
			phone_util.Format(pnumber, PhoneNumberUtil::INTERNATIONAL, &formatted_number);
			break;
		case EXTERNAL_CALL:
			phone_util.FormatOutOfCountryCallingNumber(pnumber, country, &formatted_number);
			REMOVE_SPACES(formatted_number);
			break;
		case NATIONAL:
			phone_util.Format(pnumber, PhoneNumberUtil::NATIONAL, &formatted_number);
			break;
		case NATIONAL_COMPACT:
			phone_util.Format(pnumber, PhoneNumberUtil::NATIONAL, &formatted_number);
			REMOVE_SPACES(formatted_number);
			break;
		case NATIONAL_SHORT:
			formatted_number = std::to_string(pnumber.national_number());
			break;
		default:
			formatted_number = number;
			break;
	}

    strcpy(formatted, formatted_number.c_str());

    return 0;

}
