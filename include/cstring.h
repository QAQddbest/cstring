/**
 * Created by OliverDD on 2020/5/26.
 */

#ifndef CSTRING_CSTRING_H
#define CSTRING_CSTRING_H

#include <stdint.h>

typedef void (*cstring_LockFn)(void *mutex, uint8_t lock);

typedef struct CString {
    void *ctx;
    uint32_t size;
    uint32_t capacity;
    void *mutex;
    cstring_LockFn lockFn;
} CString;

/* LOCAL */
CString *_cstring_New(uint32_t capacity, const void *val, ...);

CString *_cstring_Extend(CString *string, uint32_t difference, const void *val, ...);

/* API */
uint8_t cstring_NeedTrdSfy(CString *string, cstring_LockFn lockFn, void *mutex);

#define cstring_New(capacity, ...) _cstring_New(capacity, __VA_ARGS__)

void cstring_Release(CString *string);

CString *cstring_Resize(CString *string, int64_t capacity);

#define cstring_Extend(string, difference, ...) _cstring_Extend(string, difference, __VA_ARGS__);

CString *cstring_Set(CString *string, uint32_t difference, const void *val, ...);

CString *cstring_Clear(CString *string);

#endif //CSTRING_CSTRING_H
