/*
 * [open]lipc - lipc-test-hasharray.c
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
	LIPCha *ha;

	assert((lipc = LipcOpenNoName()) != NULL);

	assert((ha = LipcHasharrayNew(lipc)) != NULL);
	assert(LipcHasharrayFree(ha, 1) == LIPC_OK);

	/* add new hash map to the array */

	assert((ha = LipcHasharrayNew(lipc)) != NULL);
	assert(LipcHasharrayGetHashCount(ha) == 0);

	int index = 0xDEAD;
	assert(LipcHasharrayAddHash(ha, &index) == LIPC_OK);
	assert(LipcHasharrayGetHashCount(ha) == 1);
	assert(index == 0);

	/* add values to the hash map structure */

	const char *key_int = "Int";
	const char string[] = "Value";
	const unsigned char blob[] = { 1, 2, 0, 4, 5 };
	assert(LipcHasharrayPutInt(ha, 0, key_int, 0xB00B) == LIPC_OK);
	assert(LipcHasharrayPutString(ha, 0, "Key", string) == LIPC_OK);
	assert(LipcHasharrayPutBlob(ha, 0, "Doom", blob, sizeof(blob)) == LIPC_OK);

	/* get the number of added hash values */

	const char *keys[3];
	size_t count = 0;

	assert(LipcHasharrayKeys(ha, 0, NULL, &count) == LIPC_OK);
	assert(count == 3);

	assert(LipcHasharrayKeys(ha, 0, keys, &count) == LIPC_OK);
	assert(strcmp(keys[0], key_int) == 0);
	assert(strcmp(keys[2], "Doom") == 0);
	/* check if key is stored internally */
	assert(keys[0] != key_int);

	/* check data types */

	LIPCHasharrayType type;
	size_t size;

	assert(LipcHasharrayCheckKey(ha, 0, key_int, &type, &size) == LIPC_OK);
	assert(type == LIPC_HASHARRAY_INT);
	assert(size == sizeof(0xB00B));
	assert(LipcHasharrayCheckKey(ha, 0, "Key", &type, &size) == LIPC_OK);
	assert(type == LIPC_HASHARRAY_STRING);
	assert(size == sizeof(string));
	assert(LipcHasharrayCheckKey(ha, 0, "Doom", &type, &size) == LIPC_OK);
	assert(type == LIPC_HASHARRAY_BLOB);
	assert(size == sizeof(blob));

	/* get added values */

	int value_int;
	assert(LipcHasharrayGetInt(ha, 0, key_int, &value_int) == LIPC_OK);
	assert(value_int == 0xB00B);

	char *string_;
	assert(LipcHasharrayGetString(ha, 0, "Key", &string_) == LIPC_OK);
	assert(strcmp(string_, string) == 0);
	/* check if value is stored internally */
	assert(string_ != key_int);

	unsigned char *blob_;
	assert(LipcHasharrayGetBlob(ha, 0, "Doom", &blob_, &size) == LIPC_OK);
	assert(size == sizeof(blob));
	assert(memcmp(blob_, blob, size) == 0);
	/* check if value is stored internally */
	assert((void *)blob_ != (void *)blob);

	LipcHasharrayDestroy(ha);
	LipcClose(lipc);

	return EXIT_SUCCESS;
}
