
#include "display.h"

#include "../src/regioninfo.h"
#include "../src/numinfo.h"
#include "../src/formatting.h"


int main(int argc, char **argv) {
    char *number;
    char *country;

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