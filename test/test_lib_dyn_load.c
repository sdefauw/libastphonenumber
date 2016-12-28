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
#include <dlfcn.h>

int main(int argc, char **argv)
{
    void *lib_handle;
    int (*fn)(char *, char *, char *);
    char *error;
    char out_num[100];

    lib_handle = dlopen("/usr/lib/libastphonenumber.so", RTLD_LAZY);
    if (!lib_handle)
    {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(1);
    }

    fn = (int (*)(char *, char *, char *))dlsym(lib_handle, "e164_format");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "dlsym: %s\n", error);
        exit(1);
    }

    (*fn)(argv[1], argv[2], out_num);
    printf("Formatted: %s\n",out_num);

    dlclose(lib_handle);
    return 0;
}