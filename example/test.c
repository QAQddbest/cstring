/**
 * Created by OliverDD on 2020/5/26.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "cstring.h"

int main(){
    CString *string1 = c_cstring_New(12, "hello");
    CString *string2 = c_cstring_New(4, "");
    CString *string3 = c_cstring_New(10, "%d %s", 10, "sss");

    printf("string1: %s\n", string1->ctx);
    printf("string2: %s\n", string2->ctx);
    printf("string3: %s\n", string3->ctx);

    c_cstring_Extend(string1, 0, " world");
    c_cstring_Set(string2, 4, "12345678");
    c_cstring_Clear(string3);

    printf("string1: %s\n", string1->ctx);
    printf("string2: %s\n", string2->ctx);
    printf("string3: %s\n", string3->ctx);

    c_cstring_Release(string1);
    c_cstring_Release(string2);
    c_cstring_Release(string3);
    return 0;
}