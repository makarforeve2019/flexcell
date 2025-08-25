#include "Base91Converter.h"

// Определение статических переменных
std::unordered_map<char32_t, int> Base91Converter::charMap;
const char32_t Base91Converter::charArray[] = U"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+,-./:;<=>?@[]^_`{|}~";

void Base91Converter::Initialize() {
    for (int i = 0; i < 10; ++i) {
        charMap[U'0' + i] = i; // цифры '0' - '9'
    }
    for (int i = 0; i < 26; ++i) {
        charMap[U'A' + i] = 10 + i; // заглавные латинские буквы 'A' - 'Z'
        charMap[U'a' + i] = 36 + i; // строчные латинские буквы 'a' - 'z'
    }
    const char32_t specialChars[] = U"!#$%&()*+,-./:;<=>?@[]^_`{|}~"; // Исключены двойные и одинарные кавычки, 29 специальных символов
    for (int i = 0; i < 29; ++i) { // 29 специальных символов
        charMap[specialChars[i]] = 62 + i; // специальные символы
    }
}

int Base91Converter::CharToValue(char32_t c) {
    auto it = charMap.find(c);
    if (it != charMap.end()) {
        return it->second;
    } else {
        std::cerr << "Invalid character in the input string: " << std::hex << c << std::endl;
        throw std::invalid_argument("Invalid character in the input string");
    }
}

char32_t Base91Converter::ValueToChar(int value) {
    if (value >= 0 && value < 91) { // Убедимся, что значение в пределах 0-90
        return charArray[value];
    } else {
        throw std::invalid_argument("Value out of range: " + std::to_string(value));
    }
}

int Base91Converter::Base91StringToInt(const std::string& base91Str) {
    if (base91Str.empty() || base91Str[0] != '$') {
        throw std::invalid_argument("Input string must start with '$'");
    }

    // Конвертация std::string в std::u32string
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    std::u32string base91Part = convert.from_bytes(base91Str.substr(1));

    int result = 0;

    for (char32_t c : base91Part) {
        result = result * 91 + CharToValue(c);
    }

    return result;
}

std::string Base91Converter::IntToBase91String(uint64_t value, int digits) {
    if (digits <= 0) {
        throw std::invalid_argument("Digits must be a positive integer");
    }

    std::u32string u32result;
    while (value > 0) {
        int remainder = value % 91;
        try {
            u32result = ValueToChar(remainder) + u32result;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            throw;
        }
        value /= 91;
    }

    // Добавляем ведущие нули до заданного количества цифр
    while (u32result.length() < static_cast<size_t>(digits)) {
        u32result = U"0" + u32result;
    }

    // Конвертация std::u32string в std::string
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(u32result);
}

