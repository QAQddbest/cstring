# cstring

这是C语言的string库。为了方便使用字符串，我花费了点时间写了这个还不完善的库。

特点:
1. 线程安全。但需要自己实现加锁解锁函数。
2. 使用简单。尽可能将数组操作封装，使用者可以不考虑下标。
3. ...

基于ANSI C99，使用CMake构建。

## 使用
目前可使用如下API:

|返回类型 | 函数名 | 接受参数 |
|-------|-------|---------|
| uint8_t | cstring_NeedTrdSfy | (CString *string, cstring_LockFn lockFn, void *mutex)
| CString * | cstring_New | (capacity, __VA_ARGS__) |
| void | cstring_Release | (CString *string) |
| CString * | cstring_Resize | (CString *string, int64_t capacity) |
| CString * | cstring_Extend | (string, difference, __VA_ARGS__) |
| CString * | cstring_Set | (CString *string, uint32_t difference, const void *val, ...) |
| CString * | cstring_Clear | (CString *string) |
| 待续...    |               |                   |

## 已知问题

暂无

## 后续目标

- [ ] 高字节字符串支持。不止是8字节char型string，可以是16、32和64.
- [ ] 移植嵌入式平台