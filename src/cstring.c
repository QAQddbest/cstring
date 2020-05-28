#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cstring.h"

#ifdef C_CSTRING_MUL_THREAD
#define Lock(string) _Lock(string)
#define Unlock(string) _Unlock(string)
#else
#define Lock(string) ((void)0)
#define Unlock(string) ((void)0)
#endif

/**
 * Local function. Used for thread safety
 * @param string CString object
 */
static inline void _Lock(CString *string) {
    if (string->lockFn) {
        string->lockFn(string->mutex, 1);
    }
}

/**
 * Local function. Used for thread safety
 * @param string CString object
 */
static inline void _Unlock(CString *string) {
    if (string->lockFn) {
        string->lockFn(string->mutex, 0);
    }
}

/**
 * Called when thread safety is needed.
 * @param string CString object
 * @param lockFn Lock function that look like void (void *, uint8_t)
 * @param mutex Lock mutex
 * @return
 *      0 - successful.
 *      1 - CString object is NULL
 *      2 - failed. The CString object already has it's Lock function and mutex.
 */
uint8_t c_cstring_NeedTrdSfy(CString *string, CStringLockFn lockFn, void *mutex) {
    if (string == NULL) {
        return 1;
    }
    if (string->lockFn != NULL) {
        return 2;
    }
    string->lockFn = lockFn;
    string->mutex = mutex;
    return 0;
}

/**
 * Do not call this function directly. Create a CString object and initialize it.
 * @param capacity CString's capacity
 * @param val string
 * @param ...
 * @return
 *      CString - Successful. The CString object passed in.
 *      NULL - Failed.
 */
CString *_cstring_New(uint32_t capacity, const void *val, ...) {
    // Initialize struct CString
    CString *string = malloc(sizeof(CString));
    if(string == NULL)
        return NULL;
    string->ctx = calloc(capacity + 1, sizeof(char));
    (*(char **) (&string->ctx))[capacity] = '\0';
    string->capacity = capacity;
    string->lockFn = NULL;
    string->mutex = NULL;
    // Assignment
    va_list args;
    va_start(args, val);
    vsprintf(string->ctx, val, args);
    va_end(args);
    string->size = strlen(string->ctx);
    return string;
}

/**
 * Release the CString object
 * @param string CString object
 */
void c_cstring_Release(CString *string) {
    if (string == NULL) {
        return;
    }
    Lock(string);
    free(string->ctx);
    free(string);
    Unlock(string);
}

/**
 * Rearrange CString's capacity
 * @param string CString object
 * @param capacity CString's capacity
 * @return
 *      CString - Successful. The CString object passed in.
 *      NULL - Failed.
 */
CString *c_cstring_Resize(CString *string, int64_t capacity) {
    if (string == NULL) {
        return NULL;
    }
    Lock(string);
    string->ctx = realloc(string->ctx, capacity);
    string->capacity = capacity;
    (*(char **) (&string->ctx))[capacity] = '\0';
    string->size = strlen(string->ctx);
    Unlock(string);
    return string;
}

/**
 * Do not call this function directly.
 * @param string CString's object
 * @param difference difference on current capacity
 * @param val
 * @param ...
 * @return
 *      CString - Successful. The CString object passed in.
 *      NULL - Failed.
 */
CString *_cstring_Extend(CString *string, uint32_t difference, const void *val, ...) {
    if (string == NULL) {
        return NULL;
    }
    if (difference != 0)
        c_cstring_Resize(string, (int64_t) (string->capacity) + difference);
    // Assignment
    va_list args;
    va_start(args, val);
    Lock(string);
    vsprintf(((*(char **) (&string->ctx)) + string->size), val, args);
    string->size = strlen(string->ctx);
    Unlock(string);
    va_end(args);
    return string;
}

/**
 * Set string's content
 * @param string CString's object
 * @param difference difference on current capacity
 * @param val
 * @param ...
 * @return
 *      CString - Successful. The CString object passed in.
 *      NULL - Failed.
 */
CString *c_cstring_Set(CString *string, uint32_t difference, const void *val, ...){
    if (string == NULL) {
        return NULL;
    }
    if (difference != 0)
        c_cstring_Resize(string, (int64_t) (string->capacity) + difference);
    // Assignment
    va_list args;
    va_start(args, val);
    Lock(string);
    vsprintf(string->ctx, val, args);
    string->size = strlen(string->ctx);
    Unlock(string);
    va_end(args);
    return string;
}

/**
 * Clear string's content
 * @param string CString object
 * @return
 *      CString - successful. The CString object passed in.
 *      NULL - Failed.
 */
CString *c_cstring_Clear(CString *string){
    if (string == NULL) {
        return NULL;
    }
    Lock(string);
    memset(string->ctx, 0, string->capacity);
    string->size=0;
    Unlock(string);
    return string;
}

#undef Lock
#undef Unlock