/**
 * @file openlipc.h
 * @brief Open-source LIPC header file.
 *
 * This file is a part of [open]lipc.
 *
 * @copyright
 * This projected is licensed under the terms of the MIT license.
 *
 * @note
 * This header file is based on the reverse-engineered proprietary library
 * lipc, which is a part of the Kindle firmware. The lipc library itself is
 * copyrighted under the terms of the Amazon Technologies, Inc.
 *
 */

#ifndef OPENLIPC_H
#define OPENLIPC_H

/**
 * @defgroup init Initialization
 * @{ */

/** LIPC library handler. */
typedef void LIPC;

/**
 * Status codes returned by all sorts of lipc library functions.
 *
 * @warning
 * This list was obtained from the LipcGetErrorString() function and may
 * be not complete - be prepared for other values as well. */
typedef enum {
	LIPC_OK = 0,
	LIPC_ERROR_UNKNOWN,                 /* 1 */
	LIPC_ERROR_INTERNAL,                /* 2 */
	LIPC_ERROR_NO_SUCH_SOURCE,          /* 3 */
	LIPC_ERROR_OPERATION_NOT_SUPPORTED, /* 4 */
	LIPC_ERROR_OUT_OF_MEMORY,           /* 5 */
	LIPC_ERROR_SUBSCRIPTION_FAILED,     /* 6 */
	LIPC_ERROR_NO_SUCH_PARAM,           /* 7 */
	LIPC_ERROR_NO_SUCH_PROPERTY,        /* 8 */
	LIPC_ERROR_ACCESS_NOT_ALLOWED,      /* 9 */
	LIPC_ERROR_BUFFER_TOO_SMALL,        /* 10 */
	LIPC_ERROR_INVALID_HANDLE,          /* 11 */
	LIPC_ERROR_INVALID_ARG,             /* 12 */
	LIPC_ERROR_OPERATION_NOT_ALLOWED,   /* 13 */
	LIPC_ERROR_PARAMS_SIZE_EXCEEDED,    /* 14 */
	LIPC_ERROR_TIMED_OUT,               /* 15 */
	LIPC_ERROR_SERVICE_NAME_TOO_LONG,   /* 16 */
	LIPC_ERROR_DUPLICATE_SERVICE_NAME,  /* 17 */
	LIPC_ERROR_INIT_DBUS,               /* 18 */
	LIPC_PROP_ERROR_INVALID_STATE   = 0x100,
	LIPC_PROP_ERROR_NOT_INITIALIZED = 0x101,
	LIPC_PROP_ERROR_INTERNAL        = 0x102,
} LIPCcode;

/**
 * Initialize LIPC library without registering a new service.
 *
 * @return On success the LIPC library handler is returned, which should be
 *   closed with the LipcClose(). Upon error this function returns NULL. */
LIPC *LipcOpenNoName(void);

/**
 * Initialize LIPC library and register a new service.
 *
 * @param service The service name which should be registered. The name has
 *   to be a fully qualified dot-separated identifier, e.g. "org.MyService".
 * @return On success the LIPC library handler is returned, which should be
 *   closed with the LipcClose(). Upon error this function returns NULL. */
LIPC *LipcOpen(const char *service);

/**
 * Initialize LIPC library and register a new service.
 *
 * This function is an extended version of the LipcOpen().
 *
 * @param service The service name which should be registered. The name has
 *   to be a fully qualified dot-separated identifier, e.g. "org.MyService".
 * @param code If not NULL, the status code will be stored in this argument.
 * @return On success the LIPC library handler is returned, which should be
 *   closed with the LipcClose(). Upon error this function returns NULL. */
LIPC *LipcOpenEx(const char *service, LIPCcode *code);

/**
 * Close the LIPC handler and release all associated resources.
 *
 * @param lipc LIPC library handler. */
void LipcClose(LIPC *lipc);

/**
 * Get the service name associated with the LIPC handler.
 *
 * @param lipc LIPC library handler.
 * @return The service name which was registered during LIPC opening or NULL
 *   if the handler was obtained by the call to the LipcOpenNoName(). */
const char *LipcGetServiceName(LIPC *lipc);

/**
 * Get status code in the string format.
 *
 * @param code The status code.
 * @return String with the human-readable status. */
const char *LipcGetErrorString(LIPCcode code);

/** @}
 ***/

/**
 * @defgroup props Properties
 * @{ */

/**
 * Get property access timeout.
 *
 * @note
 * The timeout value can be set via the file "/var/local/system/lipctimeout",
 * otherwise it is set to the default 10000 ms.
 *
 * @param lipc LIPC library handler.
 * @return The timeout value in milliseconds. */
int LipcGetPropAccessTimeout(LIPC *lipc);

/**
 * Get the value of the integer property.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param property The property name.
 * @param value The address where the integer value will be stored.
 * @return The status code. */
LIPCcode LipcGetIntProperty(LIPC *lipc, const char *service,
                            const char *property, int *value);

/**
 * Set the value of the integer property.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param property The property name.
 * @param value The new value to set.
 * @return The status code. */
LIPCcode LipcSetIntProperty(LIPC *lipc, const char *service,
                            const char *property, int value);

/**
 * Get the value of the string property.
 *
 * The memory for the returned string is allocated internally by the library
 * and should be freed with the LipcFreeString().
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param property The property name.
 * @param value The address where the pointer to the string will be stored.
 * @return The status code. */
LIPCcode LipcGetStringProperty(LIPC *lipc, const char *service,
                               const char *property, char **value);

/**
 * Set the value of the string property.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param property The property name.
 * @param value The new value to set.
 * @return The status code. */
LIPCcode LipcSetStringProperty(LIPC *lipc, const char *service,
                               const char *property, const char *value);

/**
 * Convenience macro for getting all properties of given service.
 *
 * The list of properties is formated as a space-delimited string, where every
 * element is in the format as follows: "<property> <type> <access> ". Note,
 * that at the end of the string, there is always a hanging space character.
 *
 * The access mode of the property can be either read-only: r, write-only: w,
 * or both: rw.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param value The address where the pointer to the string will stored.
 * @return The status code. */
#define LipcGetProperties(lipc, service, value) \
	LipcGetStringProperty(lipc, service, "_properties", value)

/**
 * Free memory allocated by the LIPC.
 *
 * @param string The string pointer. */
void LipcFreeString(char *string);

/**
 * Property getter/setter callback function.
 *
 * This callback function can be used either for getting attributes or for
 * setting ones. However, there is a slight difference in both action, also
 * there is a difference approach when getter is called for string property
 * vs integer property.
 *
 * When the getter is called for the integer property, one has to set the
 * address pointed by the *data to the desired integer value. The address
 * pointed by the *size parameter should not be modified. In fact the size
 * parameter points to the mode string passed during property creation. It
 * seems to be a bug-prone "design", but it might be as well some hidden
 * feature.
 *
 * When the getter is called for the string property, address pointed by the
 * *data has preallocated the size available in the *size parameter. If this
 * size is too small for copping desired string value, one should return the
 * LIPC_ERROR_BUFFER_TOO_SMALL code and pass the required buffer size in the
 * *size argument. Then, the callback function will be called once more with
 * the requested amount of space in the *data buffer.
 *
 * When the setter is called, the memory pointed by the *data contains the
 * requested value which should be set for the property. The size parameter
 * should not be modified - it points to the mode string.
 *
 * @param lipc LIPC library handler.
 * @param property The property name.
 * @param data Pointer to the memory area for data storage.
 * @param size The size of the memory area.
 * @return The status code. */
typedef LIPCcode (*LipcPropCallback)(LIPC *lipc, const char *property,
                                     void *data, int *size);

/**
 * Register new integer property.
 *
 * @warning
 * The memory pointer passed via the mode parameter is saved in the library.
 * Be aware when passing memory allocated on the stack!
 *
 * @param lipc LIPC library handler.
 * @param property The property name.
 * @param getter Getter callback if property is readable.
 * @param setter Setter callback if property is writable.
 * @param mode The mode string.
 * @return The status code. */
LIPCcode LipcRegisterIntProperty(LIPC *lipc, const char *property,
                                 LipcPropCallback getter,
                                 LipcPropCallback setter,
                                 char *mode);

/**
 * Register new string property.
 *
 * @warning
 * The memory pointer passed via the mode parameter is saved in the library.
 * Be aware when passing memory allocated on the stack!
 *
 * @param lipc LIPC library handler.
 * @param property The property name.
 * @param getter Getter callback if property is readable.
 * @param setter Setter callback if property is writable.
 * @param mode The mode string.
 * @return The status code. */
LIPCcode LipcRegisterStringProperty(LIPC *lipc, const char *property,
                                    LipcPropCallback getter,
                                    LipcPropCallback setter,
                                    char *mode);

/**
 * Unregister property.
 *
 * If mode is not NULL, the address of the mode string passed during property
 * registration will be stored in the address pointed by the *mode. Via this
 * mechanism one is able to free memory allocated for the mode string - for
 * more informations about the mode string memory management see the warning
 * section in the LipcRegisterIntProperty() documentation.
 *
 * @param lipc LIPC library handler.
 * @param property The property name.
 * @param mode Address where the mode string pointer is returned.
 * @return The status code. */
LIPCcode LipcUnregisterProperty(LIPC *lipc, const char *property, char **mode);

/** @}
 ***/

/**
 * @defgroup events Events
 * @{ */

/** LIPC event handler. */
typedef void LIPCevent;

/**
 * Create a new event object.
 *
 * In order to use this function the LIPC handler has to be opened with the
 * service name given using LipcOpen() or LipcOpenEx().
 *
 * @param lipc LIPC library handler.
 * @param name The event name.
 * @return On success the LIPCevent handler is returned, which should be freed
 *   with the LipcEventFree(). Upon error this function returns NULL. */
LIPCevent *LipcNewEvent(LIPC *lipc, const char *name);

/**
 * Free memory allocated by the LIPC.
 *
 * @param event LIPC event handler. */
void LipcEventFree(LIPCevent *event);

/**
 * Get the source name associated with the event.
 *
 * @param event LIPC event handler.
 * @return The source name from where the event originates. */
const char *LipcGetEventSource(LIPCevent *event);

/**
 * Get the name of the event.
 *
 * @param event LIPC event handler.
 * @return The event name. */
const char *LipcGetEventName(LIPCevent *event);

/**
 * Get the integer parameter from the event.
 *
 * @param event LIPC event handler.
 * @param value The address where the integer value will be stored.
 * @return The status code. */
LIPCcode LipcGetIntParam(LIPCevent *event, int *value);

/**
 * Add the integer parameter to the event.
 *
 * @param event LIPC event handler.
 * @param value The new value to add.
 * @return The status code. */
LIPCcode LipcAddIntParam(LIPCevent *event, int value);

/**
 * Get the string parameter from the event.
 *
 * @note
 * The obtained memory pointer should not be passed to the LipcFreeString()
 * function. It seems, that the memory is managed internally by the library.
 *
 * @param event LIPC event handler.
 * @param value The address where the pointer to the string will be stored.
 * @return The status code. */
LIPCcode LipcGetStringParam(LIPCevent *event, char **value);

/**
 * Add the string parameter to the event.
 *
 * @param event LIPC event handler.
 * @param value The new value to add.
 * @return The status code. */
LIPCcode LipcAddStringParam(LIPCevent *event, const char *value);

/**
 * Event dispatching callback function.
 *
 * @param lipc LIPC library handler.
 * @param name The event name.
 * @param event The LIPC event handler.
 * @param data Data pointer passed during subscription.
 * @return The status code. */
typedef LIPCcode (*LipcEventCallback)(LIPC *lipc, const char *name,
                                      LIPCevent *event, void *data);

/**
 * Set default event callback function.
 *
 * The callback function will be called when one uses the LipcSubscribe()
 * subscription mechanism. Note, that it is not possible to pass user-defined
 * data via this approach - if one wants to pass extra data to the callback
 * function, one should use the LipcSubscribeExt() instead.
 *
 * @param lipc LIPC library handler.
 * @param callback Event dispatching callback function.
 * @return The status code. */
LIPCcode LipcSetEventCallback(LIPC *lipc, LipcEventCallback callback);

/**
 * Subscribe for all events emitted by the service.
 *
 * In order to receive event notifications, one has to set default callback
 * function using the LipcSetEventCallback() function. Using them both, is an
 * equivalent of using the extended version with parameters set as follows:
 * LipcSubscribeExt(lipc, service, NULL, callback, NULL)
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @return The status code. */
LIPCcode LipcSubscribe(LIPC *lipc, const char *service);

/**
 * Subscribe for events emitted by the service.
 *
 * This function is an extended version of the LipcSubscribe(), which allows
 * to set the event name for which one wants to subscribe and the user-defined
 * data passed to the callback function. Passing NULL instead of the event
 * name will subscribe for all events.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param name The event name.
 * @param callback Event dispatching callback function.
 * @param data Data pointer passed to the callback function.
 * @return The status code. */
LIPCcode LipcSubscribeExt(LIPC *lipc, const char *service, const char *name,
                          LipcEventCallback callback, void *data);

/**
 * Unsubscribe from event or events.
 *
 * If the event name is NULL, this function will unsubscribe from events
 * delivered to the callback set with the LipcSetEventCallback().
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param name The event name or NULL.
 * @param data Address where the data pointer is returned or NULL.
 * @return The status code. */
LIPCcode LipcUnsubscribeExt(LIPC *lipc, const char *service,
                            const char *name, void **data);

/** @}
 ***/

/**
 * @defgroup logs Logging
 * @{ */

/**
 * Log debug messages.
 *
 * LIPC defines 8 levels of debug messages. Via this macro one can select the
 * level which should be logged. Valid values are from 1 to 8. */
#define LAB126_LOG_DEBUG (n) ((1 << ((n) - 1)) << 8)
#define LAB126_LOG_INFO      (0x0080 << 16)
#define LAB126_LOG_WARNING   (0x0100 << 16)
#define LAB126_LOG_ERROR     (0x0200 << 16)
#define LAB126_LOG_CRITICAL  (0x0400 << 16)
#define LAB126_LOG_DEBUG_ALL  0x0000FF00
#define LAB126_LOG_ALL        0xFFFFFF00

/** Global logging mask. */
extern int g_lab126_log_mask;

/** Set the LIPC internal logging mask.
 *
 * The logging mask parameter should be the logical OR of the logging flags
 * (LAB126_LOG_* definitions). The current state of the logging mask can be
 * accessed via the g_lab126_log_mask global variable.
 *
 * @param mask The logging mask. */
void LipcSetLlog(int mask);

/** @}
 ***/

#endif /* OPENLIPC_H */
