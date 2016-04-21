/*
 * [open]lipc - lipc-test-init.c
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


int main(void) {

	LIPC *lipc;
	LIPCcode code;

	assert((lipc = LipcOpenNoName()) != NULL);
	assert(LipcGetServiceName(lipc) == NULL);
	LipcClose(lipc);

	assert((lipc = LipcOpen("com.example")) != NULL);
	assert(strcmp(LipcGetServiceName(lipc), "com.example") == 0);
	LipcClose(lipc);

	code = LIPC_ERROR_UNKNOWN;
	assert((lipc = LipcOpenEx("com.example", &code)) != NULL);
	assert(strcmp(LipcGetServiceName(lipc), "com.example") == 0);
	assert(code == LIPC_OK);
	LipcClose(lipc);

	assert(strcmp(LipcGetErrorString(LIPC_OK), "lipcErrNone") == 0);

	return EXIT_SUCCESS;
}
