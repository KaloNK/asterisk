/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (C) 2013, Digium, Inc.
 *
 * David M. Lee, II <dlee@digium.com>
 *
 * See http://www.asterisk.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!                               DO NOT EDIT                        !!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * This file is generated by a mustache template. Please see the original
 * template in rest-api-templates/res_ari_resource.c.mustache
 */

/*! \file
 *
 * \brief Stasis application resources
 *
 * \author David M. Lee, II <dlee@digium.com>
 */

/*** MODULEINFO
	<depend type="module">res_ari</depend>
	<depend type="module">res_ari_model</depend>
	<depend type="module">res_stasis</depend>
	<support_level>core</support_level>
 ***/

#include "asterisk.h"

ASTERISK_FILE_VERSION(__FILE__, "$Revision$")

#include "asterisk/app.h"
#include "asterisk/module.h"
#include "asterisk/stasis_app.h"
#include "ari/resource_applications.h"
#if defined(AST_DEVMODE)
#include "ari/ari_model_validators.h"
#endif

#define MAX_VALS 128

/*!
 * \brief Parameter parsing callback for /applications.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_applications_list_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_json *body, struct ast_ari_response *response)
{
	struct ast_ari_applications_list_args args = {};
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	ast_ari_applications_list(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_list(response->message,
				ast_ari_validate_application_fn());
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /applications\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /applications\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	return;
}
/*!
 * \brief Parameter parsing callback for /applications/{applicationName}.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_applications_get_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_json *body, struct ast_ari_response *response)
{
	struct ast_ari_applications_get_args args = {};
	struct ast_variable *i;
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	for (i = path_vars; i; i = i->next) {
		if (strcmp(i->name, "applicationName") == 0) {
			args.application_name = (i->value);
		} else
		{}
	}
	ast_ari_applications_get(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
	case 404: /* Application does not exist. */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_application(
				response->message);
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /applications/{applicationName}\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /applications/{applicationName}\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	return;
}
int ast_ari_applications_subscribe_parse_body(
	struct ast_json *body,
	struct ast_ari_applications_subscribe_args *args)
{
	struct ast_json *field;
	/* Parse query parameters out of it */
	field = ast_json_object_get(body, "eventSource");
	if (field) {
		/* If they were silly enough to both pass in a query param and a
		 * JSON body, free up the query value.
		 */
		ast_free(args->event_source);
		if (ast_json_typeof(field) == AST_JSON_ARRAY) {
			/* Multiple param passed as array */
			size_t i;
			args->event_source_count = ast_json_array_size(field);
			args->event_source = ast_malloc(sizeof(*args->event_source) * args->event_source_count);

			if (!args->event_source) {
				return -1;
			}

			for (i = 0; i < args->event_source_count; ++i) {
				args->event_source[i] = ast_json_string_get(ast_json_array_get(field, i));
			}
		} else {
			/* Multiple param passed as single value */
			args->event_source_count = 1;
			args->event_source = ast_malloc(sizeof(*args->event_source) * args->event_source_count);
			if (!args->event_source) {
				return -1;
			}
			args->event_source[0] = ast_json_string_get(field);
		}
	}
	return 0;
}

/*!
 * \brief Parameter parsing callback for /applications/{applicationName}/subscription.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_applications_subscribe_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_json *body, struct ast_ari_response *response)
{
	struct ast_ari_applications_subscribe_args args = {};
	struct ast_variable *i;
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	for (i = get_params; i; i = i->next) {
		if (strcmp(i->name, "eventSource") == 0) {
			/* Parse comma separated list */
			char *vals[MAX_VALS];
			size_t j;

			args.event_source_parse = ast_strdup(i->value);
			if (!args.event_source_parse) {
				ast_ari_response_alloc_failed(response);
				goto fin;
			}

			if (strlen(args.event_source_parse) == 0) {
				/* ast_app_separate_args can't handle "" */
				args.event_source_count = 1;
				vals[0] = args.event_source_parse;
			} else {
				args.event_source_count = ast_app_separate_args(
					args.event_source_parse, ',', vals,
					ARRAY_LEN(vals));
			}

			if (args.event_source_count == 0) {
				ast_ari_response_alloc_failed(response);
				goto fin;
			}

			if (args.event_source_count >= MAX_VALS) {
				ast_ari_response_error(response, 400,
					"Bad Request",
					"Too many values for event_source");
				goto fin;
			}

			args.event_source = ast_malloc(sizeof(*args.event_source) * args.event_source_count);
			if (!args.event_source) {
				ast_ari_response_alloc_failed(response);
				goto fin;
			}

			for (j = 0; j < args.event_source_count; ++j) {
				args.event_source[j] = (vals[j]);
			}
		} else
		{}
	}
	for (i = path_vars; i; i = i->next) {
		if (strcmp(i->name, "applicationName") == 0) {
			args.application_name = (i->value);
		} else
		{}
	}
	if (ast_ari_applications_subscribe_parse_body(body, &args)) {
		ast_ari_response_alloc_failed(response);
		goto fin;
	}
	ast_ari_applications_subscribe(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
	case 400: /* Missing parameter. */
	case 404: /* Application does not exist. */
	case 422: /* Event source does not exist. */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_application(
				response->message);
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /applications/{applicationName}/subscription\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /applications/{applicationName}/subscription\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	ast_free(args.event_source_parse);
	ast_free(args.event_source);
	return;
}
int ast_ari_applications_unsubscribe_parse_body(
	struct ast_json *body,
	struct ast_ari_applications_unsubscribe_args *args)
{
	struct ast_json *field;
	/* Parse query parameters out of it */
	field = ast_json_object_get(body, "eventSource");
	if (field) {
		/* If they were silly enough to both pass in a query param and a
		 * JSON body, free up the query value.
		 */
		ast_free(args->event_source);
		if (ast_json_typeof(field) == AST_JSON_ARRAY) {
			/* Multiple param passed as array */
			size_t i;
			args->event_source_count = ast_json_array_size(field);
			args->event_source = ast_malloc(sizeof(*args->event_source) * args->event_source_count);

			if (!args->event_source) {
				return -1;
			}

			for (i = 0; i < args->event_source_count; ++i) {
				args->event_source[i] = ast_json_string_get(ast_json_array_get(field, i));
			}
		} else {
			/* Multiple param passed as single value */
			args->event_source_count = 1;
			args->event_source = ast_malloc(sizeof(*args->event_source) * args->event_source_count);
			if (!args->event_source) {
				return -1;
			}
			args->event_source[0] = ast_json_string_get(field);
		}
	}
	return 0;
}

/*!
 * \brief Parameter parsing callback for /applications/{applicationName}/subscription.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_applications_unsubscribe_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_json *body, struct ast_ari_response *response)
{
	struct ast_ari_applications_unsubscribe_args args = {};
	struct ast_variable *i;
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	for (i = get_params; i; i = i->next) {
		if (strcmp(i->name, "eventSource") == 0) {
			/* Parse comma separated list */
			char *vals[MAX_VALS];
			size_t j;

			args.event_source_parse = ast_strdup(i->value);
			if (!args.event_source_parse) {
				ast_ari_response_alloc_failed(response);
				goto fin;
			}

			if (strlen(args.event_source_parse) == 0) {
				/* ast_app_separate_args can't handle "" */
				args.event_source_count = 1;
				vals[0] = args.event_source_parse;
			} else {
				args.event_source_count = ast_app_separate_args(
					args.event_source_parse, ',', vals,
					ARRAY_LEN(vals));
			}

			if (args.event_source_count == 0) {
				ast_ari_response_alloc_failed(response);
				goto fin;
			}

			if (args.event_source_count >= MAX_VALS) {
				ast_ari_response_error(response, 400,
					"Bad Request",
					"Too many values for event_source");
				goto fin;
			}

			args.event_source = ast_malloc(sizeof(*args.event_source) * args.event_source_count);
			if (!args.event_source) {
				ast_ari_response_alloc_failed(response);
				goto fin;
			}

			for (j = 0; j < args.event_source_count; ++j) {
				args.event_source[j] = (vals[j]);
			}
		} else
		{}
	}
	for (i = path_vars; i; i = i->next) {
		if (strcmp(i->name, "applicationName") == 0) {
			args.application_name = (i->value);
		} else
		{}
	}
	if (ast_ari_applications_unsubscribe_parse_body(body, &args)) {
		ast_ari_response_alloc_failed(response);
		goto fin;
	}
	ast_ari_applications_unsubscribe(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
	case 400: /* Missing parameter; event source scheme not recognized. */
	case 404: /* Application does not exist. */
	case 409: /* Application not subscribed to event source. */
	case 422: /* Event source does not exist. */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_application(
				response->message);
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /applications/{applicationName}/subscription\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /applications/{applicationName}/subscription\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	ast_free(args.event_source_parse);
	ast_free(args.event_source);
	return;
}

/*! \brief REST handler for /api-docs/applications.json */
static struct stasis_rest_handlers applications_applicationName_subscription = {
	.path_segment = "subscription",
	.callbacks = {
		[AST_HTTP_POST] = ast_ari_applications_subscribe_cb,
		[AST_HTTP_DELETE] = ast_ari_applications_unsubscribe_cb,
	},
	.num_children = 0,
	.children = {  }
};
/*! \brief REST handler for /api-docs/applications.json */
static struct stasis_rest_handlers applications_applicationName = {
	.path_segment = "applicationName",
	.is_wildcard = 1,
	.callbacks = {
		[AST_HTTP_GET] = ast_ari_applications_get_cb,
	},
	.num_children = 1,
	.children = { &applications_applicationName_subscription, }
};
/*! \brief REST handler for /api-docs/applications.json */
static struct stasis_rest_handlers applications = {
	.path_segment = "applications",
	.callbacks = {
		[AST_HTTP_GET] = ast_ari_applications_list_cb,
	},
	.num_children = 1,
	.children = { &applications_applicationName, }
};

static int unload_module(void)
{
	ast_ari_remove_handler(&applications);
	stasis_app_unref();
	return 0;
}

static int load_module(void)
{
	int res = 0;
	stasis_app_ref();
	res |= ast_ari_add_handler(&applications);
	if (res) {
		unload_module();
		return AST_MODULE_LOAD_DECLINE;
	}

	return AST_MODULE_LOAD_SUCCESS;
}

AST_MODULE_INFO(ASTERISK_GPL_KEY, AST_MODFLAG_DEFAULT, "RESTful API module - Stasis application resources",
	.support_level = AST_MODULE_SUPPORT_CORE,
	.load = load_module,
	.unload = unload_module,
	.nonoptreq = "res_ari,res_stasis",
	);
