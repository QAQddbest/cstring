/**
 * Created by OliverDD on 2020/5/26.
 */

#ifndef C_CSTRING_H
#define C_CSTRING_H

#include <stdint.h>

typedef void (*CStringLockFn)(void *mutex, uint8_t lock);

typedef struct CString {
    void *ctx;
    uint32_t size;
    uint32_t capacity;
    void *mutex;
    CStringLockFn lockFn;
} CString;

/* LOCAL */
CString *_cstring_New(uint32_t capacity, const void *val, ...);

CString *_cstring_Extend(CString *string, uint32_t difference, const void *val, ...);

/* API */
uint8_t c_cstring_NeedTrdSfy(CString *string, CStringLockFn lockFn, void *mutex);

#define c_cstring_New(capacity, ...) _cstring_New(capacity, __VA_ARGS__)

void c_cstring_Release(CString *string);

CString *c_cstring_Resize(CString *string, int64_t capacity);

#define c_cstring_Extend(string, difference, ...) _cstring_Extend(string, difference, __VA_ARGS__);

CString *c_cstring_Set(CString *string, uint32_t difference, const void *val, ...);

CString *c_cstring_Clear(CString *string);



#endif //C_CSTRING_H
