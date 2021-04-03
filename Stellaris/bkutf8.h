#ifndef _BKUTF8_H
#define _BKUTF8_H

#include <string>
#include <string>

int ConvertSingleUTF8Character(wchar_t &dst, const unsigned char *src, unsigned int size, unsigned int &bytes_used);

//获取UTF8转换为Unicode的字符数量（注意不是内存大小）。不包含terminate字符（'\0'）。
unsigned int UniFromUTF8_Size(const char *str, unsigned int len);
inline unsigned int UniFromUTF8_Size(const std::string &str) { return UniFromUTF8_Size(str.data(), (unsigned int)str.size()); }
//获取UTF8转换为Unicode的std字符串。
std::wstring UniFromUTF8(const char *str, unsigned int len);
inline std::wstring UniFromUTF8(const std::string &str) { return UniFromUTF8(str.data(), (unsigned int)str.size()); }
//将UTF8转换为Unicode。不会自动添加terminate字符（'\0'）。
bool UniFromUTF8(wchar_t *dst, const char *str, unsigned int srclen);

//将UTF8转换为UTF16。不会自动添加terminate字符（'\0'）。
bool UTF16FromUTF8(char16_t *dst, const char *str, unsigned int srclen);
unsigned int UTF16FromUTF8_Size(const char *str, unsigned int len);
std::u16string UTF16FromUTF8(const char *str, unsigned int len);
inline std::u16string UTF16FromUTF8(const std::string &str) { return UTF16FromUTF8(str.data(), (unsigned int)str.size()); }

bool IsValidUTF8(const char *str, unsigned int len);
inline bool IsValidUTF8(const std::string &str) { return IsValidUTF8(str.data(), (unsigned int)str.size()); }

//获取Unicode转换为UTF8的字符数量（注意不是内存大小）。不包含terminate字符（'\0'）。
unsigned int UniToUTF8_Size(const wchar_t *str, unsigned int len);
inline unsigned int UniToUTF8_Size(const std::wstring &str) { return UniToUTF8_Size(str.data(), (unsigned int)str.size()); }
//获取Unicode转换为UTF8的std字符串。
std::string UniToUTF8(const wchar_t *str, unsigned int len, bool addBOM = false);
inline std::string UniToUTF8(const std::wstring &str, bool addBOM = false) { return UniToUTF8(str.data(), (unsigned int)str.size()); }
//将Unicode转换为UTF8。不会自动添加terminate字符（'\0'）。
void UniToUTF8(char *dst, const wchar_t *str, unsigned int len);

//获取Unicode转换为UTF8的字符数量（注意不是内存大小）。不包含terminate字符（'\0'）。
unsigned int UTF16ToUTF8_Size(const char16_t *str, unsigned int len);
inline unsigned int UTF16ToUTF8_Size(const std::u16string &str) { return UTF16ToUTF8_Size(str.data(), (unsigned int)str.size()); }
//获取Unicode转换为UTF8的std字符串。
std::string UTF16ToUTF8(const char16_t *str, unsigned int len, bool addBOM = false);
inline std::string UTF16ToUTF8(const std::u16string &str, bool addBOM = false) { return UTF16ToUTF8(str.data(), (unsigned int)str.size()); }
//将Unicode转换为UTF8。不会自动添加terminate字符（'\0'）。
void UTF16ToUTF8(char *dst, const char16_t *str, unsigned int len);

//获取UTF16转换为Unicode的字符数量（注意不是内存大小）。不包含terminate字符（'\0'）。
unsigned int UniFromUTF16_Size(const char16_t *str, unsigned int len);
//获取UTF16转换为Unicode的std字符串。
std::wstring UniFromUTF16(const char16_t *str, unsigned int len);
inline std::wstring UniFromUTF16(const std::u16string &str) { return UniFromUTF16(str.data(), str.size()); }
//将UTF16转换为Unicode。不会自动添加terminate字符（'\0'）。
bool UniFromUTF16(wchar_t *dst, const char16_t *str, unsigned int len);

//获取Unicode转换为UTF16的字符数量（注意不是内存大小）。不包含terminate字符（'\0'）。
unsigned int UniToUTF16_Size(const wchar_t *str, unsigned int len);
inline unsigned int UniToUTF16_Size(const std::wstring &str) { return UniToUTF16_Size(str.data(), (unsigned int)str.size()); }
//将Unicode转换为UTF16。不会自动添加terminate字符（'\0'）。
void UniToUTF16(char16_t *dst, const wchar_t *str, unsigned int len);
inline void UniToUTF16(char16_t *dst, const std::wstring &str) { return UniToUTF16(dst, str.data(), (unsigned int)str.size()); }
std::u16string UniToUTF16(const wchar_t *str, unsigned int len);
inline std::u16string UniToUTF16(const std::wstring &str) { return UniToUTF16(str.data(), (unsigned int)str.size()); }

//获取Unicode转换为UTF7的字符数量（注意不是内存大小）。不包含terminate字符（'\0'）。
inline unsigned int UniToUTF7_Size(const wchar_t *str, unsigned int len) { (void)str; return len; }
inline unsigned int UniToUTF7_Size(const std::wstring &str) { return UniToUTF7_Size(str.data(), (unsigned int)str.size()); }
//将Unicode转换为UTF7。不会自动添加terminate字符（'\0'）。
void UniToUTF7(char* dst, const wchar_t* str, unsigned int len);
std::string UniToUTF7(const wchar_t* str, unsigned int len);
inline std::string UniToUTF7(const std::wstring &str, bool addBOM = false) { return UniToUTF7(str.data(), (unsigned int)str.size()); }


//获取UTF7转换为Unicode的字符数量（注意不是内存大小）。不包含terminate字符（'\0'）。
inline unsigned int UniFromUTF7_Size(const char *str, unsigned int len) { (void)str; return len; }
inline unsigned int UniFromUTF7_Size(const std::string &str) { return UniFromUTF7_Size(str.data(), (unsigned int)str.size()); }
//获取UTF7转换为Unicode的std字符串。
void UniFromUTF7(wchar_t* dst, const char* str, unsigned int len);
std::wstring UniFromUTF7(const char *str, unsigned int len);
std::u16string UTF16FromUTF7(const char *str, unsigned int len);
inline std::wstring UniFromUTF7(const std::string &str) { return UniFromUTF7(str.data(), (unsigned int)str.size()); }
inline std::u16string UTF16FromUTF7(const std::string &str) { return UTF16FromUTF7(str.data(), (unsigned int)str.size()); }

//获取UTF16转换为UTF7的字符数量（注意不是内存大小）。不包含terminate字符（'\0'）。
inline unsigned int UTF16ToUTF7_Size(const char16_t *str, unsigned int len) { (void)str; return len; }
inline unsigned int UTF16ToUTF7_Size(const std::u16string &str) { return UTF16ToUTF7_Size(str.data(), (unsigned int)str.size()); }
//将UTF16转换为UTF7。不会自动添加terminate字符（'\0'）。
void UTF16ToUTF7(char *dst, const char16_t *str, unsigned int len);
std::string UTF16ToUTF7(const char16_t *str, unsigned int len);
inline std::string UTF16ToUTF7(const std::u16string &str, bool addBOM = false) { return UTF16ToUTF7(str.data(), (unsigned int)str.size()); }



//将UTF8转换为Unicode。不会自动添加terminate字符（'\0'）。

#endif