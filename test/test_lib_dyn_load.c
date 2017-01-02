
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "../src/formatting.h"

int main(int argc, char **argv)
{
    void *lib_handle;
    int (*fn)(char *, char *, enum phone_format, char *);
    char *error;
    char out_num[100];

    lib_handle = dlopen("/usr/lib/libastphonenumber.so", RTLD_LAZY);
    if (!lib_handle)
    {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(1);
    }

    fn = (int (*)(char *, char *, enum phone_format, char *))dlsym(lib_handle, "num_format");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "dlsym: %s\n", error);
        exit(1);
    }

	if (!argv[1] || !argv[2]) {
		printf("Invalid input ! Input: <NUMBER> <COUNTRY>\n");
		return 0;
	}

    (*fn)(argv[1], argv[2], PLUSE164, out_num);
    printf("Formatted: %s\n",out_num);

    dlclose(lib_handle);
    return 0;
}