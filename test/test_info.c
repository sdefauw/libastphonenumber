/*
 * Libphonenmuber for Asterisk
 *
 * Copyright (c) 2016.
 *
 * Sebastien De Fauw <sdefauw@gmail.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "../src/regioninfo.h"
#include "../src/numinfo.h"
#include "../src/libastformat.h"

#define BOOL2STR(b)     b ? "Yes" : "No"

void print_num_info(char* num, char* country) {
	char buf[100];

	printf("=== NUMBER INFO ===\n");

	printf(" - Is valid number: %s\n", BOOL2STR(is_valid_number(num, country, 0)));
	printf(" - Is valid number for %s: %s\n", country, BOOL2STR(is_valid_number(num, country, 1)));

	printf(" - Country calling code: %d\n", get_country_code(num, country));
	get_region(num, country, buf);
	printf(" - Country: %s\n", buf);


	get_region(num, country, buf);
	print_region_info(buf);
}

void print_formatted(char* num, char* country) {
	char buf[100];

	printf("=== NUMBER FORMATTED ===\n");
	printf(" - Original number: %s\n", num);
	e164_format(num, country, buf);
	printf(" - E.164: %s\n", buf);
}

void print_region_info(char* country) {

	printf("=== REGION INFO (%s) ===\n", country);
	printf(" - Country: %s\n", country);
	printf(" - Country calling code: %d\n", get_country_code_for_region(country));
}

int main(int argc, char **argv)
{
	char* number;
	char* country;

	number = argv[1];
	country = argv[2];

	printf("=== INPUT ===\n");
	printf(" - Number: %s\n", number);
	printf(" - Country: %s\n", country);

	print_num_info(number, country);
	print_region_info(country);
	print_formatted(number, country);

	return 0;
}