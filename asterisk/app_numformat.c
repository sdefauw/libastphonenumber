/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (c) 2017.
 *
 * Sebastien De Fauw <sdefauw@gmail.com>
 *
 * This code is released by the author with no restrictions on usage.
 *
 * See http://www.asterisk.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 */

/*! \file
 *
 * \brief App to format numbers and get inforamtion about phone numbers.
 *
 * \author Sebastien De Fauw <sdefauw@gmail.com>
 *
 * \ingroup applications
 */

/*** MODULEINFO
	<support_level>core</support_level>
 ***/

#include <dlfcn.h>

#include "asterisk.h"

#include "asterisk/module.h"
#include "asterisk/app.h"
#include "asterisk/pbx.h"
#include "asterisk/channel.h"
#include "asterisk/cli.h"

#include "astphonenumber/formatting.h"
#include "astphonenumber/numinfo.h"

/*** DOCUMENTATION
	<function name="FORMAT_NUM" language="en_US">
 		<synopsis>
			Get formatted number in specific format based on a country.
		</synopsis>
		<syntax argsep=":">
			<parameter name="number" required="true">
				<para>Number to format.</para>
			</parameter>
			<parameter name="country" required="true">
				<para>Two-letter country (ISO 3166-1) where the national number will be taken into account.</para>
			</parameter>
			<parameter name="format" required="true">
				<para>Ouput format type</para>
				<enumlist>
					<enum name="e164">
						<para>E.164 format: https://en.wikipedia.org/wiki/E.164.</para>
					</enum>
					<enum name="+e164">
						<para>+E.164 format.</para>
					</enum>
					<enum name="nat">
						<para>National format.</para>
					</enum>
					<enum name="nat_short">
						<para>National format without national prefix.</para>
					</enum>
					<enum name="ext">
						<para>Format number to call like you are in the country: National format or add an international call prefix.</para>
					</enum>
				</enumlist>
			</parameter>
		</syntax>
		<description>
			<para>This function returns the number formatted.</para>
		</description>
	</function>
	<function name="IS_VALID_NUM" language="en_US">
 		<synopsis>
			Check validity of a number.
		</synopsis>
		<syntax argsep=":">
			<parameter name="number" required="true">
				<para>Number to check.</para>
			</parameter>
			<parameter name="country" required="true">
				<para>Two-letter country (ISO 3166-1) where the national number will be taken into account.</para>
			</parameter>
			<parameter name="option" required="false">
				<para>Option</para>
				<enumlist>
					<enum name="l">
						<para>Check if the number is valid only in the country specified.</para>
					</enum>
				</enumlist>
			</parameter>
		</syntax>
		<description>
			<para>This function returns 1 if the number is valid. Otherwise, it returns 0. An option can check if the number belongs only to the country.</para>
		</description>
	</function>
	<function name="NUM_INFO" language="en_US">
 		<synopsis>
			Get information about a number.
		</synopsis>
		<syntax argsep=":">
			<parameter name="number" required="true">
				<para>Number to check.</para>
			</parameter>
			<parameter name="country" required="true">
				<para>Two-letter country (ISO 3166-1) where the national number will be taken into account.</para>
			</parameter>
			<parameter name="option" required="true">
				<para>Type of information to get.</para>
				<enumlist>
					<enum name="ccc">
						<para>Get the country calling code of the number.</para>
					</enum>
					<enum name="country">
						<para>Country or region whose number is owned. The country is described by two-letter (ISO 3166-1).</para>
					</enum>
					<enum name="type">
						<para>Type of phone numbers: FIXED_LINE, MOBILE, FIXED_LINE_OR_MOBILE, TOLL_FREE, PREMIUM_RATE, SHARED_COST, VOIP, PERSONAL_NUMBER, PAGER, UAN, VOICEMAIL or UNKNOWN.</para>
					</enum>
				</enumlist>
			</parameter>
		</syntax>
		<description>
			<para>This function returns information about a phone number.</para>
		</description>
	</function>
	<function name="REGION_INFO" language="en_US">
 		<synopsis>
			Get information about a country/region.
		</synopsis>
		<syntax argsep=":">
			<parameter name="country" required="true">
				<para>Two-letter country/region (ISO 3166-1) where the national number will be taken into account.</para>
			</parameter>
			<parameter name="option" required="true">
				<para>Type of information to get.</para>
				<enumlist>
					<enum name="ccc">
						<para>Get the country calling code of the region.</para>
					</enum>
				</enumlist>
			</parameter>
		</syntax>
		<description>
			<para>This function returns information about a region.</para>
		</description>
	</function>
 ***/

#define BOOL2STR(b)     b ? "Yes" : "No"

static int (*num_format_fn)(char *, char *, enum phone_format, char *);
static int (*is_valid_number_fn)(char* , char*, int);
static int (*get_region_fn)(char* , char* , char *);
static int (*get_country_code_fn)(char* , char*);
static enum phone_type (*get_number_type_fn)(char* , char*);
static int (*get_country_code_for_region_fn)(char* region);

static void *lib_astphonenumber_handle;

static char *cli_formatnum_e164(struct ast_cli_entry *e, int cmd, struct ast_cli_args *a);
static char *cli_formatnum_info(struct ast_cli_entry *e, int cmd, struct ast_cli_args *a);

static int format_num_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len);
static int valid_num_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len);
static int info_num_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len);
static int info_region_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len);

/*! \brief Format Number Cli commands definition */
static struct ast_cli_entry cli_formatnum[] = {
	AST_CLI_DEFINE(cli_formatnum_info, "Format number information"),
	AST_CLI_DEFINE(cli_formatnum_e164, "Format number in +E.164")
};

static struct ast_custom_function format_num_function = {
	.name = "FORMAT_NUM",
	.read = format_num_func_read,
};

static struct ast_custom_function valid_num_function = {
	.name = "IS_VALID_NUM",
	.read = valid_num_func_read,
};

static struct ast_custom_function info_num_function = {
	.name = "NUM_INFO",
	.read = info_num_func_read,
};

static struct ast_custom_function info_region_function = {
	.name = "REGION_INFO",
	.read = info_region_func_read,
};

static char *cli_formatnum_info(struct ast_cli_entry *e, int cmd, struct ast_cli_args *a)
{
	switch (cmd) {
	case CLI_INIT:
		e->command = "number lib info";
		e->usage =
			"Usage: number lib info\n"
			"       Display information about the formatting library\n";
		return NULL;
	case CLI_GENERATE:
		return NULL;
	}

	if (a->argc != 3) {
		return CLI_SHOWUSAGE;
	}

	if (!lib_astphonenumber_handle) {
		ast_cli(a->fd, "External library unloaded !\n");
		return CLI_SUCCESS;
	}

	ast_cli(a->fd, " - External library\n \t * /usr/lib/libastphonenumber.so loaded\n");

	return CLI_SUCCESS;
}

static char *cli_formatnum_e164(struct ast_cli_entry *e, int cmd, struct ast_cli_args *a)
{
	char buf[100];
	char *number;
	char *country;

	switch (cmd) {
	case CLI_INIT:
		e->command = "number info";
		e->usage =
			"Usage: number info <number> <country>\n"
			"       Get information about the number in the country specified.\n"
			"       Validity, country, type of number, region information and some formatting.\n";
		return NULL;
	case CLI_GENERATE:
		return NULL;
	}

	if (a->argc != 4) {
		return CLI_SHOWUSAGE;
	}

	number = (char*) a->argv[2];
	country = (char*) a->argv[3];

	memset(buf, 0, 100);

	ast_cli(a->fd, "Original information\n");
	ast_cli(a->fd, " - Number: %s\n", number);
	ast_cli(a->fd, " - Country: %s\n", country);

	ast_cli(a->fd, "Number information\n");
	ast_cli(a->fd, " - Is valid number: %s\n", BOOL2STR((*is_valid_number_fn)(number, country, 0)));
	ast_cli(a->fd, " - Is valid number in %s: %s\n", country, BOOL2STR((*is_valid_number_fn)(number, country, 1)));
	ast_cli(a->fd, " - Country calling code: %d\n", (*get_country_code_fn)(number, country));
	(*get_region_fn)(number, country, buf);
	ast_cli(a->fd, " - Country: %s\n", buf);
	ast_cli(a->fd, " - Type: %s\n", phone_type_str[(*get_number_type_fn)(number, country)]);

	ast_cli(a->fd, "Region information\n");
	ast_cli(a->fd, " - %s country calling code: %d\n", buf, (*get_country_code_for_region_fn)(buf));
	ast_cli(a->fd, " - %s country calling code: %d\n", country, (*get_country_code_for_region_fn)(country));

	ast_cli(a->fd, "Formatting result\n");
	(*num_format_fn)(number, country, PLUSE164, buf);
	ast_cli(a->fd, " - +E.164: %s\n", buf);
	(*num_format_fn)(number, country, E164, buf);
	ast_cli(a->fd, " - E.164: %s\n", buf);
	(*num_format_fn)(number, country, INTERNATIONAL, buf);
	ast_cli(a->fd, " - International: %s\n", buf);
	(*num_format_fn)(number, country, EXTERNAL_CALL, buf);
	ast_cli(a->fd, " - External call: %s\n", buf);
	(*num_format_fn)(number, country, NATIONAL, buf);
	ast_cli(a->fd, " - National: %s\n", buf);
	(*num_format_fn)(number, country, NATIONAL_COMPACT, buf);
	ast_cli(a->fd, " - National (compact): %s\n", buf);
	(*num_format_fn)(number, country, NATIONAL_SHORT, buf);
	ast_cli(a->fd, " - National (short): %s\n", buf);

	return CLI_SUCCESS;
}

static int format_num_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len)
{
	char *number, *country, *option;
	char number_formatted[len];
	enum phone_format format;

	number = ast_strdupa(data);

	if ((country = strchr(number, ':'))) {
		*country = '\0';
		country++;
	}

	if ((option = strchr(country, ':'))) {
		*option = '\0';
		option++;
	}


	if (ast_strlen_zero(number) || ast_strlen_zero(country) || ast_strlen_zero(option)) {
		ast_log(LOG_ERROR, "This function needs a number and a country: number:country\n");
		return 0;
	}

	if(!strcasecmp(option, "+e164")) {
		format = PLUSE164;
	} else if (!strcasecmp(option, "e164")) {
		format = E164;
	} else if (!strcasecmp(option, "nat")) {
		format = NATIONAL_COMPACT;
	} else if (!strcasecmp(option, "nat_short")) {
		format = NATIONAL_SHORT;
	} else if (!strcasecmp(option, "ext")) {
		format = EXTERNAL_CALL;
	} else {
		format = PLUSE164;
	}

	ast_debug(3, "Format number %s %s(%d) in %s country.\n", number, option, format, country);
	(*num_format_fn)(number, country, format, number_formatted);

	ast_copy_string(buf, number_formatted, len);

	return 0;
}

static int valid_num_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len)
{
	char *number, *country, *option;
	int is_valid;

	number = ast_strdupa(data);

	if ((country = strchr(number, ':'))) {
		*country = '\0';
		country++;
	}

	if ((option = strchr(country, ':'))) {
		*option = '\0';
		option++;
	}

	if (ast_strlen_zero(number) || ast_strlen_zero(country)) {
		ast_log(LOG_ERROR, "This function needs a number and a country: number:country\n");
		return 0;
	}
	if(!ast_strlen_zero(option) && strcasecmp(option, "l")) {
		ast_log(LOG_ERROR, "Invalid option.\n");
		return 0;
	}

	// Check validity of the number
	is_valid = (*is_valid_number_fn)(number, country, (!ast_strlen_zero(option) && *option == 'l'));
	if (!ast_strlen_zero(option) && *option == 'l') {
		ast_debug(3, "Number %s is %s format in country %s.\n", number, is_valid ? "a valid" : "an invalid", country);
	} else {
		ast_debug(3, "Number %s is %s format.\n", number, is_valid ? "a valid" : "an invalid");
	}
	ast_copy_string(buf, is_valid ? "1" : "0", 2);


	return 0;
}

static int info_num_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len)
{
	char *number, *country, *option;

	number = ast_strdupa(data);

	if ((country = strchr(number, ':'))) {
		*country = '\0';
		country++;
	}

	if ((option = strchr(country, ':'))) {
		*option = '\0';
		option++;
	}

	if (ast_strlen_zero(number) || ast_strlen_zero(country) || ast_strlen_zero(option)) {
		ast_log(LOG_ERROR, "This function needs a number, a country and an option: number:country:option\n");
		return 0;
	}

	// Get information about the number
	if(!strcasecmp(option, "ccc")) {
		// Get country calling code.
		sprintf(buf, "%d", (*get_country_code_fn)(number, country));
	} else if (!strcasecmp(option, "country")) {
		// Get country.
		(*get_region_fn)(number, country, buf);
	} else if (!strcasecmp(option, "type")) {
		// Type of number.
		strcpy(buf, phone_type_str[(*get_number_type_fn)(number, country)]);
	} else {
		ast_log(LOG_ERROR, "Invalid option\n");
		return 0;
	}

	return 0;
}

static int info_region_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len)
{
	char *country, *option;

	country = ast_strdupa(data);

	if ((option = strchr(country, ':'))) {
		*option = '\0';
		option++;
	}

	if (ast_strlen_zero(country) || ast_strlen_zero(option)) {
		ast_log(LOG_ERROR, "This function needs a number and an option: country:option\n");
		return 0;
	}

	// Get information about a region
	if(!strcasecmp(option, "ccc")) {
		// Get country calling code.
		sprintf(buf, "%d", (*get_country_code_for_region_fn)(country));
	} else {
		ast_log(LOG_ERROR, "Invalid option\n");
		return 0;
	}

	return 0;
}

static int unload_module(void)
{
	/* Unregister CLI commands */
	ast_cli_unregister_multiple(cli_formatnum, ARRAY_LEN(cli_formatnum));

	/* Unregister all functions */
	ast_custom_function_unregister(&format_num_function);
	ast_custom_function_unregister(&valid_num_function);
	ast_custom_function_unregister(&info_num_function);
	ast_custom_function_unregister(&info_region_function);

	/* Unlink the dynamic library */
	dlclose(lib_astphonenumber_handle);

	return 0;
}

static int load_module(void)
{
    char *error;

	/* Register all CLI functions for number formatting */
	ast_cli_register_multiple(cli_formatnum, ARRAY_LEN(cli_formatnum));

	/* Load external module */
	lib_astphonenumber_handle = dlopen("/usr/lib/libastphonenumber.so.1", RTLD_LAZY);
	if (!lib_astphonenumber_handle){
		ast_log(LOG_WARNING, "Unable to load libastphonenumber.so.1: %s\n", dlerror());
		return 1;
	}
	// Load functions
	num_format_fn = (int (*)(char *, char *, enum phone_format, char *)) dlsym(lib_astphonenumber_handle, "num_format");
	is_valid_number_fn = (int (*)(char*, char*, int)) dlsym(lib_astphonenumber_handle, "is_valid_number");
	get_region_fn = (int (*)(char*, char* , char *)) dlsym(lib_astphonenumber_handle, "get_region");
	get_country_code_fn = (int (*)(char*, char*)) dlsym(lib_astphonenumber_handle, "get_country_code");
	get_number_type_fn = (enum phone_type (*)(char*, char*)) dlsym(lib_astphonenumber_handle, "get_number_type");
	get_country_code_for_region_fn = (int (*)(char*)) dlsym(lib_astphonenumber_handle, "get_country_code_for_region");
	if ((error = dlerror()) != NULL){
		ast_log(LOG_WARNING, "Unable to load functions on libastphonenumber.so.1: %s\n", error);
		return 1;
	}

	/* Register all functions */
	ast_custom_function_register(&format_num_function);
	ast_custom_function_register(&valid_num_function);
	ast_custom_function_register(&info_num_function);
	ast_custom_function_register(&info_region_function);

	return 0;
}

AST_MODULE_INFO_STANDARD(ASTERISK_GPL_KEY, "Format a number");
