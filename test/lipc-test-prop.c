/*
 * [open]lipc - lipc-test-prop.c
 * Copyright (c) 2016 Arkadiusz Bokowy
 *
 * This file is a part of openlipc.
 *
 * This project is licensed under the terms of the MIT license.
 *
 */

#include "openlipc.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


static int prop_int = 0xDEAD;
static char prop_str[64] = "Yes! Yes! Yes!";
static int prop_s_size = 0;

LIPCcode getter(LIPC *lipc, const char *property, void *value, void *data) {
	(void)lipc;

	int i = strcmp(property, "int") == 0;
	int s = strcmp(property, "str") == 0;
	int _data = (long int)(data);

	assert(i || s);

	if (i) {
		assert(_data == 0x1111);
		LIPC_GETTER_VTOI(value) = prop_int;
	}

	if (s) {

		if (!prop_s_size || *(int *)data < 500) {
			assert(++prop_s_size == 1);
			*(int *)data = 500;
			return LIPC_ERROR_BUFFER_TOO_SMALL;
		}

		assert(prop_s_size == 1);
		strncpy(LIPC_GETTER_VTOS(value), prop_str, *(int *)data);

	}

	return LIPC_OK;
}

LIPCcode setter(LIPC *lipc, const char *property, void *value, void *data) {
	(void)lipc;

	int i = strcmp(property, "int") == 0;
	int s = strcmp(property, "str") == 0;
	int _data = (long int)(data);

	assert(i || s);

	if (i) {
		assert(_data == 0x1111);
		prop_int = LIPC_SETTER_VTOI(value);
	}

	if (s) {
		assert(_data == 0x2222);
		strncpy(prop_str, LIPC_SETTER_VTOS(value), sizeof(prop_str) - 1);
	}

	return LIPC_OK;
}

int main(void) {

	LIPC *lipc;
	char *value_s;
	int value_i;

	lipc = LipcOpen("com.example");

	assert(LipcRegisterIntProperty(lipc, "int", getter, setter, (void *)0x1111) == LIPC_OK);
	assert(LipcRegisterStringProperty(lipc, "str", getter, setter, (void *)0x2222) == LIPC_OK);

	/* get default property values */

	assert(LipcGetIntProperty(lipc, "com.example", "int", &value_i) == LIPC_OK);
	assert(value_i == prop_int);

	assert(LipcGetStringProperty(lipc, "com.example", "str", &value_s) == LIPC_OK);
	assert(strcmp(value_s, prop_str) == 0);
	LipcFreeString(value_s);

	/* set property values */

	assert(LipcSetIntProperty(lipc, "com.example", "int", 0xBEEF) == LIPC_OK);
	assert(prop_int == 0xBEEF);

	assert(LipcSetStringProperty(lipc, "com.example", "str", "No!") == LIPC_OK);
	assert(strcmp(prop_str, "No!") == 0);

	/* get list of registered properties */

	assert(LipcGetProperties(lipc, "com.example", &value_s) == LIPC_OK);
	assert(strcmp(value_s, "str Str rw int Int rw ") == 0);
	LipcFreeString(value_s);

	/* get not registered property */

	assert(LipcGetIntProperty(lipc, "com.example", "xxx", &value_i) == LIPC_ERROR_NO_SUCH_PROPERTY);

	int tmp;
	assert(LipcUnregisterProperty(lipc, "int", NULL) == LIPC_OK);
	assert(LipcUnregisterProperty(lipc, "str", (void *)&tmp) == LIPC_OK);
	assert(tmp == 0x2222);

	/* test access mode setting */

	assert(LipcRegisterIntProperty(lipc, "int", getter, NULL, NULL) == LIPC_OK);
	assert(LipcRegisterStringProperty(lipc, "str", NULL, setter, NULL) == LIPC_OK);

	assert(LipcGetProperties(lipc, "com.example", &value_s) == LIPC_OK);
	assert(strcmp(value_s, "str Str w int Int r ") == 0);
	LipcFreeString(value_s);

	value_s = (char *)0xDEAD;
	assert(LipcSetIntProperty(lipc, "com.example", "int", 0x0C0C) == LIPC_ERROR_ACCESS_NOT_ALLOWED);
	assert(LipcGetStringProperty(lipc, "com.example", "str", &value_s) == LIPC_ERROR_ACCESS_NOT_ALLOWED);
	assert(value_s == (char *)0xDEAD);

	assert(LipcUnregisterProperty(lipc, "int", NULL) == LIPC_OK);
	assert(LipcUnregisterProperty(lipc, "str", NULL) == LIPC_OK);

	LipcClose(lipc);

	return EXIT_SUCCESS;
}
