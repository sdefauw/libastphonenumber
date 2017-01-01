
#include <stdio.h>
#include <stdlib.h>

#include "../src/regioninfo.h"
#include "../src/numinfo.h"
#include "../src/formatting.h"

#define BOOL2STR(b)     b ? "Yes" : "No"


void print_region_info(char* country) {

	printf("=== REGION INFO (%s) ===\n", country);
	printf(" - Country: %s\n", country);
	printf(" - Country calling code: %d\n", get_country_code_for_region(country));
}

void print_num_info(char* num, char* country) {
	char buf[100];

	printf("=== NUMBER INFO ===\n");

	printf(" - Is valid number: %s\n", BOOL2STR(is_valid_number(num, country, 0)));
	printf(" - Is valid number for %s: %s\n", country, BOOL2STR(is_valid_number(num, country, 1)));

	printf(" - Country calling code: %d\n", get_country_code(num, country));
	get_region(num, country, buf);
	printf(" - Country: %s\n", buf);

	printf(" - Type: %s\n", phone_type_str[get_number_type(num, country)]);


	get_region(num, country, buf);
	print_region_info(buf);
}

void print_formatted(char* num, char* country) {
	char buf[100];

	printf("=== NUMBER FORMATTED ===\n");
	printf(" - Original number: %s\n", num);
	num_format(num, country, PLUSE164, buf);
	printf(" - +E.164: %s\n", buf);
	num_format(num, country, E164, buf);
	printf(" - E.164: %s\n", buf);
	num_format(num, country, INTERNATIONAL, buf);
	printf(" - International: %s\n", buf);
	num_format(num, country, EXTERNAL_CALL, buf);
	printf(" - External: %s\n", buf);
	num_format(num, country, NATIONAL, buf);
	printf(" - National: %s\n", buf);
	num_format(num, country, NATIONAL_COMPACT, buf);
	printf(" - National compact: %s\n", buf);
	num_format(num, country, NATIONAL_SHORT, buf);
	printf(" - National short: %s\n", buf);
}

int main(int argc, char **argv)
{
	char* number;
	char* country;

	number = argv[1];
	country = argv[2];

	if (!number || !country) {
		printf("Invalid input ! Input: <NUMBER> <COUNTRY>\n");
		return 0;
	}

	printf("=== INPUT ===\n");
	printf(" - Number: %s\n", number);
	printf(" - Country: %s\n", country);

	print_num_info(number, country);
	print_region_info(country);
	print_formatted(number, country);

	return 0;
}