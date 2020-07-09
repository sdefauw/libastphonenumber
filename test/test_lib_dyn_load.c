
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "../src/formatting.h"

static int (*num_format_fn)(char *, char *, enum phone_format, char *);

static int (*is_valid_number_fn)(char *, char *, int);

static int (*get_region_fn)(char *, char *, char *);

static int (*get_country_code_fn)(char *, char *);

static enum phone_type (*get_number_type_fn)(char *, char *);

static int (*get_country_code_for_region_fn)(char *region);

int main(int argc, char **argv) {
    void *lib_handle;
    int (*fn)(char *, char *, enum phone_format, char *);
    char *error;
    char out_num[100];

    lib_handle = dlopen("/usr/lib/libastphonenumber.so.1", RTLD_LAZY);
    if (!lib_handle) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(1);
    }

    fn = (int (*)(char *, char *, enum phone_format, char *)) dlsym(lib_handle, "num_format");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "dlsym: %s\n", error);
        exit(1);
    }

    if (!argv[1] || !argv[2]) {
        printf("Invalid input ! Input: <NUMBER> <COUNTRY>\n");
        return 0;
    }

    (*fn)(argv[1], argv[2], PLUSE164, out_num);

    num_format_fn = (int (*)(char *, char *, enum phone_format, char *)) dlsym(lib_handle, "num_format");
    is_valid_number_fn = (int (*)(char *, char *, int)) dlsym(lib_handle, "is_valid_number");
    get_region_fn = (int (*)(char *, char *, char *)) dlsym(lib_handle, "get_region");
    get_country_code_fn = (int (*)(char *, char *)) dlsym(lib_handle, "get_country_code");
    get_number_type_fn = (enum phone_type (*)(char *, char *)) dlsym(lib_handle, "get_number_type");
    get_country_code_for_region_fn = (int (*)(char *)) dlsym(lib_handle, "get_country_code_for_region");


    printf("Formatted: %s\n", out_num);

    dlclose(lib_handle);
    return 0;
}