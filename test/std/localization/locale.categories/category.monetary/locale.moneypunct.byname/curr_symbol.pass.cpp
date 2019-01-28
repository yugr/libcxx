//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// XFAIL: apple-darwin
//
// NetBSD does not support LC_MONETARY at the moment
// XFAIL: netbsd

// REQUIRES: locale.en_US.UTF-8
// REQUIRES: locale.fr_FR.UTF-8
// REQUIRES: locale.ru_RU.UTF-8
// REQUIRES: locale.zh_CN.UTF-8

// <locale>

// class moneypunct_byname<charT, International>

// string_type curr_symbol() const;

#include <locale>
#include <limits>
#include <cassert>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

class Fnf
    : public std::moneypunct_byname<char, false>
{
public:
    explicit Fnf(const std::string& nm, std::size_t refs = 0)
        : std::moneypunct_byname<char, false>(nm, refs) {}
};

class Fnt
    : public std::moneypunct_byname<char, true>
{
public:
    explicit Fnt(const std::string& nm, std::size_t refs = 0)
        : std::moneypunct_byname<char, true>(nm, refs) {}
};

class Fwf
    : public std::moneypunct_byname<wchar_t, false>
{
public:
    explicit Fwf(const std::string& nm, std::size_t refs = 0)
        : std::moneypunct_byname<wchar_t, false>(nm, refs) {}
};

class Fwt
    : public std::moneypunct_byname<wchar_t, true>
{
public:
    explicit Fwt(const std::string& nm, std::size_t refs = 0)
        : std::moneypunct_byname<wchar_t, true>(nm, refs) {}
};

#if defined(_CS_GNU_LIBC_VERSION)
static bool glibc_version_less_than(char const* version) {
    std::string test_version = std::string("glibc ") + version;

    size_t n = confstr(_CS_GNU_LIBC_VERSION, nullptr, (size_t)0);
    char *current_version = new char[n];
    confstr(_CS_GNU_LIBC_VERSION, current_version, n);

    bool result = strverscmp(current_version, test_version.c_str()) < 0;
    delete[] current_version;
    return result;
}
#endif

int main()
{
    {
        Fnf f("C", 1);
        assert(f.curr_symbol() == std::string());
    }
    {
        Fnt f("C", 1);
        assert(f.curr_symbol() == std::string());
    }
    {
        Fwf f("C", 1);
        assert(f.curr_symbol() == std::wstring());
    }
    {
        Fwt f("C", 1);
        assert(f.curr_symbol() == std::wstring());
    }

    {
        Fnf f(LOCALE_en_US_UTF_8, 1);
        assert(f.curr_symbol() == "$");
    }
    {
        Fnt f(LOCALE_en_US_UTF_8, 1);
        assert(f.curr_symbol() == "USD ");
    }
    {
        Fwf f(LOCALE_en_US_UTF_8, 1);
        assert(f.curr_symbol() == L"$");
    }
    {
        Fwt f(LOCALE_en_US_UTF_8, 1);
        assert(f.curr_symbol() == L"USD ");
    }

    {
        Fnf f(LOCALE_fr_FR_UTF_8, 1);
        assert(f.curr_symbol() == " \u20ac");
    }
    {
        Fnt f(LOCALE_fr_FR_UTF_8, 1);
        assert(f.curr_symbol() == " EUR");
    }
    {
        Fwf f(LOCALE_fr_FR_UTF_8, 1);
        assert(f.curr_symbol() == L" \u20ac");
    }
    {
        Fwt f(LOCALE_fr_FR_UTF_8, 1);
        assert(f.curr_symbol() == L" EUR");
    }

    {
        Fnf f(LOCALE_ru_RU_UTF_8, 1);
#if defined(_CS_GNU_LIBC_VERSION)
        // GLIBC <= 2.23 uses currency_symbol="<U0440><U0443><U0431>"
        // GLIBC >= 2.24 uses currency_symbol="<U20BD>"
        // See also: http://www.fileformat.info/info/unicode/char/20bd/index.htm
        if (!glibc_version_less_than("2.24"))
          assert(f.curr_symbol() == " \u20BD");
        else
          assert(f.curr_symbol() == " \xD1\x80\xD1\x83\xD0\xB1");
#else
        assert(f.curr_symbol() == " \xD1\x80\xD1\x83\xD0\xB1");
#endif
    }
    {
        Fnt f(LOCALE_ru_RU_UTF_8, 1);
        assert(f.curr_symbol() == " RUB");
    }
    {
        Fwf f(LOCALE_ru_RU_UTF_8, 1);
#if defined(_CS_GNU_LIBC_VERSION)
        if (!glibc_version_less_than("2.24"))
          assert(f.curr_symbol() == L" \u20BD");
        else
          assert(f.curr_symbol() == L" \x440\x443\x431");
#else
        assert(f.curr_symbol() == L" \x440\x443\x431");
#endif
    }

    {
        Fwt f(LOCALE_ru_RU_UTF_8, 1);
        assert(f.curr_symbol() == L" RUB");
    }

    {
        Fnf f(LOCALE_zh_CN_UTF_8, 1);
        assert(f.curr_symbol() == "\xEF\xBF\xA5");
    }
    {
        Fnt f(LOCALE_zh_CN_UTF_8, 1);
        assert(f.curr_symbol() == "CNY ");
    }
    {
        Fwf f(LOCALE_zh_CN_UTF_8, 1);
        assert(f.curr_symbol() == L"\xFFE5");
    }
    {
        Fwt f(LOCALE_zh_CN_UTF_8, 1);
        assert(f.curr_symbol() == L"CNY ");
    }
}
