#ifndef MWSRESTAPI_BASE91CONVERTER_H
#define MWSRESTAPI_BASE91CONVERTER_H

#include <unordered_map>
#include <string>
#include <codecvt>
#include <locale>
#include <stdexcept>
#include <iostream>
#include <cstdint>

class Base91Converter {
public:
    static void Initialize();

    static int CharToValue(char32_t c);

    static char32_t ValueToChar(int value);

    static int Base91StringToInt(const std::string& base91Str);

    static std::string IntToBase91String(uint64_t value, int digits);

private:
    static std::unordered_map<char32_t, int> charMap;
    static const char32_t charArray[];
};

#endif // MWSRESTAPI_BASE91CONVERTER_H
