
#ifndef LIBASTPHONENUMBER_REGIONINFO_H
#define LIBASTPHONENUMBER_REGIONINFO_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the country calling code from a country/region
 *
 * @param region	Two-letter country/region (ISO 3166-1) where the national number will be taken into account.
 * @return			Return the country calling code from the region.
 */
int get_country_code_for_region(char* region);

#ifdef __cplusplus
}
#endif


#endif //LIBASTPHONENUMBER_REGIONINFO_H
