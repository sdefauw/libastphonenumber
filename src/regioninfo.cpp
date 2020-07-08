
#include "regioninfo.h"

#include "phonenumbers/phonenumberutil.h"

using i18n::phonenumbers::PhoneNumberUtil;

int get_country_code_for_region(char *region) {

    const PhoneNumberUtil &phone_util = *PhoneNumberUtil::GetInstance();

    return phone_util.GetCountryCodeForRegion(region);
}

