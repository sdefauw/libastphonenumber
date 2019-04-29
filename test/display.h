
#include <stdio.h>
#include <stdlib.h>

#include "../src/regioninfo.h"
#include "../src/numinfo.h"
#include "../src/formatting.h"


#ifndef LIBASTPHONENUMBER_DISPLAY_H
#define LIBASTPHONENUMBER_DISPLAY_H

#define BOOL2STR(b)     b ? "Yes" : "No"

void print_region_info(char* country);
void print_num_info(char* num, char* country);
void print_formatted(char* num, char* country);

#endif //LIBASTPHONENUMBER_DISPLAY_H
