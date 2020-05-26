#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cstring.h"

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
uint8_t cstring_NeedTrdSfy(CString *string, cstring_LockFn lockFn, void *mutex) {
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

CString *_cstring_New(uint32_t capacity, const void *val, ...) {
    // Initialize struct CString
    CString *string = malloc(sizeof(CString));
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

void cstring_Release(CString *string) {
    if (string == NULL) {
        return;
    }
    _Lock(string);
    free(string->ctx);
    free(string);
    _Unlock(string);
}

CString *cstring_Resize(CString *string, int64_t capacity) {
    if (string == NULL) {
        return NULL;
    }
    _Lock(string);
    string->ctx = realloc(string->ctx, capacity);
    string->capacity = capacity;
    (*(char **) (&string->ctx))[capacity] = '\0';
    string->size = strlen(string->ctx);
    _Unlock(string);
    return string;
}

CString *_cstring_Extend(CString *string, uint32_t difference, const void *val, ...) {
    if (string == NULL) {
        return NULL;
    }
    if (difference != 0)
        cstring_Resize(string, (int64_t) (string->capacity) + difference);
    // Assignment
    va_list args;
    va_start(args, val);
    _Lock(string);
    vsprintf(((*(char **) (&string->ctx)) + string->size), val, args);
    string->size = strlen(string->ctx);
    _Unlock(string);
    va_end(args);
    return string;
}

CString *cstring_Set(CString *string, uint32_t difference, const void *val, ...){
    if (string == NULL) {
        return NULL;
    }
    if (difference != 0)
        cstring_Resize(string, (int64_t) (string->capacity) + difference);
    // Assignment
    va_list args;
    va_start(args, val);
    _Lock(string);
    vsprintf(string->ctx, val, args);
    string->size = strlen(string->ctx);
    _Unlock(string);
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
CString *cstring_Clear(CString *string){
    if (string == NULL) {
        return NULL;
    }
    _Lock(string);
    memset(string->ctx, 0, string->capacity);
    string->size=0;
    _Unlock(string);
    return string;
}
