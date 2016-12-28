/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (c) 2016.
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
 * \brief App to format numbers
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

/*** DOCUMENTATION
	<function name="FORMATNUM" language="en_US">
 		<synopsis>
			Get formatted number in +E.164 format.
		</synopsis>
		<syntax argsep=":">
			<parameter name="number" required="true">
				<para>Number to format.</para>
			</parameter>
			<parameter name="country" required="true">
				<para>Country where national number will be reformatted</para>
			</parameter>
		</syntax>
		<description>
			<para>Format a number in +E.164.</para>
		</description>
	</function>
 ***/


static int (*e164_format)(char *, char *, char *);

static void *lib_astphonenumber_handle;

static char *cli_formatnum_e164(struct ast_cli_entry *e, int cmd, struct ast_cli_args *a);
static char *cli_formatnum_info(struct ast_cli_entry *e, int cmd, struct ast_cli_args *a);

static int format_num_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len);

/*! \brief Format Number Cli commands definition */
static struct ast_cli_entry cli_formatnum[] = {
	AST_CLI_DEFINE(cli_formatnum_info, "Format number information"),
	AST_CLI_DEFINE(cli_formatnum_e164, "Format number in +E.164")
};

static struct ast_custom_function format_num_function = {
	.name = "FORMATNUM",
	.read = format_num_func_read,
};

static char *cli_formatnum_info(struct ast_cli_entry *e, int cmd, struct ast_cli_args *a)
{
	switch (cmd) {
	case CLI_INIT:
		e->command = "number format info";
		e->usage =
			"Usage: number format info\n"
			"       Display information about the formatting library\n";
		return NULL;
	case CLI_GENERATE:
		return NULL;
	}

	if (a->argc != 3) {
		return CLI_SHOWUSAGE;
	}

	if (!lib_astphonenumber_handle) {
		ast_cli(a->fd, "Exernal library unloaded !\n");
		return CLI_SUCCESS;
	}

	// TODO add more info
	ast_cli(a->fd, " - Exernal library\n \t * /usr/lib/libastphonenumber.so loaded\n");

	return CLI_SUCCESS;
}

static char *cli_formatnum_e164(struct ast_cli_entry *e, int cmd, struct ast_cli_args *a)
{
	char formatted_num[100];

	switch (cmd) {
	case CLI_INIT:
		e->command = "number format";
		e->usage =
			"Usage: number format <number> <country>\n"
			"       Format the number based on the country\n";
		return NULL;
	case CLI_GENERATE:
		return NULL;
	}

	if (a->argc != 4) {
		return CLI_SHOWUSAGE;
	}

	memset(formatted_num, 0, 100);

	(*e164_format)(a->argv[2], a->argv[3], formatted_num);

	ast_cli(a->fd, " - Original number: %s\n", a->argv[2]);
	ast_cli(a->fd, " - Country: %s\n", a->argv[3]);
	ast_cli(a->fd, " => Formatted number: %s\n", formatted_num);

	return CLI_SUCCESS;
}

static int format_num_func_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len)
{
	char *number, *country;
	char number_formatted[len];

	number = ast_strdupa(data);

	if ((country = strchr(number, ':'))) {
		*country = '\0';
		country++;
	}

	if (ast_strlen_zero(number) || ast_strlen_zero(country)) {
		ast_log(LOG_ERROR, "This function needs a number and a country: number:country\n");
		return 0;
	}

	(*e164_format)(number, country, number_formatted);

	ast_copy_string(buf, number_formatted, len);

	return 0;
}

static int unload_module(void)
{
	/* Unregister CLI commands */
	ast_cli_unregister_multiple(cli_formatnum, ARRAY_LEN(cli_formatnum));

	/* Unregister all functions */
	ast_custom_function_unregister(&format_num_function);

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
	lib_astphonenumber_handle = dlopen("/usr/lib/libastphonenumber.so", RTLD_LAZY);
	if (!lib_astphonenumber_handle){
		ast_log(LOG_WARNING, "Unable to load libastphonenumber.so: %s\n", dlerror());
		return 1;
	}
	// Load functions
	e164_format = (int (*)(char *, char *, char *))dlsym(lib_astphonenumber_handle, "e164_format");
	if ((error = dlerror()) != NULL){
		ast_log(LOG_WARNING, "Unable to load functions on libastphonenumber.so: %s\n", error);
		return 1;
	}

	/* Register all functions */
	ast_custom_function_register(&format_num_function);

	return 0;
}

AST_MODULE_INFO_STANDARD(ASTERISK_GPL_KEY, "Format a number");
