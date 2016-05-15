/*
 * [open]lipc - lipc-test-event.c
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


static int event_count = 0;


LIPCcode event(LIPC *lipc, const char *name, LIPCevent *event, void *data) {

	int value_i;
	char *value_s;

	assert(strcmp(LipcGetServiceName(lipc), "com.example") == 0);
	assert(strcmp(name, "event") == 0);

	assert(strcmp(LipcGetServiceName(lipc), LipcGetEventSource(event)) == 0);
	assert(strcmp(name, LipcGetEventName(event)) == 0);
	assert((long int)data == 0xABCD);

	if (event_count == 0) {

		LipcGetIntParam(event, &value_i);
		assert(value_i == 0xDEAD);
		LipcGetStringParam(event, &value_s);
		assert(strcmp(value_s, "OK") == 0);
		LipcGetIntParam(event, &value_i);
		assert(value_i == 0xE220);

		LipcRewindParams(event);
		LipcGetIntParam(event, &value_i);
		assert(value_i == 0xDEAD);

	}

	event_count++;
	return LIPC_OK;
}

int main(void) {

	LIPC *lipc;

	lipc = LipcOpen("com.example");

	assert(LipcSubscribeExt(lipc, "com.example", "event", event, (void *)0xABCD) == LIPC_OK);

	LipcCreateAndSendEventWithParameters(lipc, "event", "%d%s%d", 0xDEAD, "OK", 0xE220);
	assert(event_count == 1);

	LipcClose(lipc);

	return EXIT_SUCCESS;
}
