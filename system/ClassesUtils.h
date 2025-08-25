//
// Created by Alex on 18.04.2024.
//

#ifndef MWSRESTAPI_CLASSESUTILS_H
#define MWSRESTAPI_CLASSESUTILS_H


#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>
#include <variant>
#include <codecvt>
#include "TrcrdFM.h"
#include "Tbov67STRUCT.h"
#include "Tbov15STRUCT.h"
#include "iconv.h"
#include <random>
#include <unordered_map>


class TCellValueAtr;

typedef TCellValueAtr *PCellValueAtr;

class TQueryUni;

class TF1FlexCelFormulas;

class TStyle;

class TFormLogicF1;

struct TEkonBPS;
typedef struct TEkonBPS *PEkonBPS;

struct Tftb;
typedef struct Tftb *Pftb;

struct TAtrRow;

struct TKntrkt;
typedef struct TKntrkt *PKntrkt;

struct TDgvr15;
typedef struct TDgvr15 *PDgvr15;

struct TTorgi;
typedef struct TTorgi *PTorgi;


//inline DatabaseType GetDBType() {
//    // Здесь может быть логика выбора базы данных.
//    // Например, можно читать из файла настроек или переменной окружения.
//    // Возвращаем фиксированное значение для демонстрации:
//    return Postgre;  // или Postgre, в зависимости от вашей ситуации
//}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Base91Converter/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//
//class Base91Converter {
//public:
//    static void Initialize() {
//        for (int i = 0; i < 10; ++i) {
//            charMap[U'0' + i] = i; // цифры '0' - '9'
//        }
//        for (int i = 0; i < 26; ++i) {
//            charMap[U'A' + i] = 10 + i; // заглавные латинские буквы 'A' - 'Z'
//            charMap[U'a' + i] = 36 + i; // строчные латинские буквы 'a' - 'z'
//        }
//        const char32_t specialChars[] = U"!#$%&()*+,-./:;<=>?@[]^_`{|}~"; // Исключены двойные и одинарные кавычки, 29 специальных символов
//        for (int i = 0; i < 29; ++i) { // 29 специальных символов
//            charMap[specialChars[i]] = 62 + i; // специальные символы
//        }
//    }
//
//    static int CharToValue(char32_t c) {
//        auto it = charMap.find(c);
//        if (it != charMap.end()) {
//            return it->second;
//        } else {
//            std::cerr << "Invalid character in the input string: " << std::hex << c << std::endl;
//            throw std::invalid_argument("Invalid character in the input string");
//        }
//    }
//
//    static char32_t ValueToChar(int value) {
//        if (value >= 0 && value < 91) { // Убедимся, что значение в пределах 0-90
//            return charArray[value];
//        } else {
//            throw std::invalid_argument("Value out of range: " + std::to_string(value));
//        }
//    }
//
//    static int Base91StringToInt(const std::string& base91Str) {
//        if (base91Str.empty() || base91Str[0] != '$') {
//            throw std::invalid_argument("Input string must start with '$'");
//        }
//
//        // Конвертация std::string в std::u32string
//        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
//        std::u32string base91Part = convert.from_bytes(base91Str.substr(1));
//
//        int result = 0;
//
//        for (char32_t c : base91Part) {
//            result = result * 91 + CharToValue(c);
//        }
//
//        return result;
//    }
//
//    static std::string IntToBase91String(uint64_t value, int digits) {
//        if (value < 0) {
//            throw std::invalid_argument("Negative values are not supported");
//        }
//
//        std::u32string u32result;
//        while (value > 0) {
//            int remainder = value % 91;
//            try {
//                u32result = ValueToChar(remainder) + u32result;
//            } catch (const std::exception& e) {
//                std::cerr << "Error: " << e.what() << std::endl;
//                throw;
//            }
//            value /= 91;
//        }
//
//        // Добавляем ведущие нули до заданного количества цифр
//        while (u32result.length() < static_cast<size_t>(digits)) {
//            u32result = U"0" + u32result;
//        }
//
//        // Конвертация std::u32string в std::string
//        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
//        return convert.to_bytes(u32result);
//    }
//
//private:
//    static std::unordered_map<char32_t, int> charMap;
//    static const char32_t charArray[];
//};
//
//// Инициализация статических переменных
//std::unordered_map<char32_t, int> Base91Converter::charMap;
//const char32_t Base91Converter::charArray[] = U"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+,-./:;<=>?@[]^_`{|}~";
//

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Other functions/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// Функция для генерации случайной строки заданной длины
inline std::string generateRandomString(size_t length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1); // Диапазон от 0 до размера строки - 1

    std::string randomString;
    randomString.reserve(length); // Резервируем память для строки

    for (size_t i = 0; i < length; ++i) {
        size_t index = distribution(generator);

        // Проверка индекса
        if (index >= characters.size()) {
            throw std::runtime_error("Generated index is out of range");
        }

        randomString += characters[index];
    }

    return randomString;
}

// Вспомогательная функция для дублирования строк
inline std::string DupeString(const std::string &str, int count) {
    std::string result;
    for (int i = 0; i < count; ++i) {
        result += str;
    }
    return result;
}

inline std::vector<std::string> extractValues(const std::string &input) {
    // Удаляем скобки
    std::string trimmed = input.substr(1, input.size() - 2);

    std::vector<std::string> values;
    std::stringstream ss(trimmed);
    std::string item;

    // Разделяем строку по запятым и добавляем в вектор
    while (std::getline(ss, item, ',')) {
        values.push_back(item);
    }

    return values;
}

//inline std::string formatDouble(float value) {
//    std::ostringstream ss;
//    ss.imbue(std::locale("C"));
//    ss << std::fixed << std::setprecision(6) << value; // Форматирование числа с плавающей точкой
//    return ss.str();
//}

inline std::string formatDouble(double value) {
    std::ostringstream ss;
    ss.imbue(std::locale("C"));
    ss << std::fixed << std::setprecision(8)
       << value; // Форматирование числа типа double с точностью до 8 знаков после запятой
    return ss.str();
}

inline long HexToLong(const std::string &hexStr) {
    return std::stoull(hexStr, nullptr, 16);
}

inline std::string LongToHex(long value) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << value;
    return ss.str();
}


inline bool TryStrToFloat(const std::string &str, double &value) {
    std::istringstream iss(str);
    iss.imbue(std::locale("C"));
    iss >> value;
    return !iss.fail() && iss.eof();
}

inline bool AnsiStartsText(const std::string &subStr, const std::string &str) {
    if (subStr.size() > str.size()) return false;

    return std::equal(subStr.begin(), subStr.end(), str.begin(),
                      [](char a, char b) {
                          return std::tolower(a) == std::tolower(b);
                      });
}

inline std::string char16ToUTF8(const char16_t *input) {
    // Создаем конвертер из UTF-16 в UTF-8
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;

    // Используем конвертер для преобразования
    std::string output = converter.to_bytes(input);

    return output;
}

//inline const char16_t* utf8_to_utf16(const std::string& input).c_str(); {
//    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
//    std::u16string u16str = converter.from_bytes(input);
//    return u16str.c_str();
//}

inline std::u16string stringToChar16(const std::string &input) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    return converter.from_bytes(input);
}

// Преобразование string строки в u16string
inline std::u16string utf8_to_utf16(const std::string &utf8) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    std::u16string sws = converter.from_bytes(utf8);
    return sws;
}


inline std::u16string intToU16String(int value) {
    // Преобразуем int в std::string
    std::string numberAsString = std::to_string(value);

    // Создаем преобразователь для перекодировки из UTF-8 в UTF-16
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;

    // Преобразуем std::string (UTF-8) в std::u16string (UTF-16)
    std::u16string result = converter.from_bytes(numberAsString);

    return result;
}

inline int u16StringToInt(const std::u16string &u16str) {
    // Создаем преобразователь для перекодировки из UTF-16 в UTF-8
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;

    // Преобразуем std::u16string (UTF-16) в std::string (UTF-8)
    std::string numberAsString = converter.to_bytes(u16str);

    // Преобразуем строку в число
    int result = std::stoi(numberAsString);

    return result;
}

inline std::string u16StringToString(const std::u16string &u16str) {
    // Создаем преобразователь для перекодировки из UTF-16 в UTF-8
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;

    // Преобразуем std::u16string (UTF-16) в std::string (UTF-8)
    std::string result = converter.to_bytes(u16str);

    return result;
}

// Функция для обрезки пробелов с начала и конца строки
inline std::string Trim(const std::string &str) {
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

    if (start >= end) {  // Проверяем, не пересеклись ли указатели
        return "";  // Возвращаем пустую строку, если строка состояла только из пробелов
    }

    return std::string(start, end);  // Создаем новую строку с обрезанными пробелами
}

// Функция преобразования целого числа в строку
inline std::string IntToStr(int value) {
    return std::to_string(value);
}

// Функция преобразования вещественного числа в строку с фиксированным количеством знаков после запятой
inline std::string FloatToStr(double value, int precision = 6) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

inline int FormatWithPrecision(int tocnost) {
    switch (tocnost) {
        case 0: return 2; // По умолчанию (как задано в форме)
        case 1: return 0; // Как целое
        case 2: return 1; // До 1-й десятой
        case 3: return 2; // До 2-х десятых
        case 4: return 3; // До 3-х десятых
        case 5: return 4; // До 4-х десятых
        case 6: return 5; // До 5-и десятых
        case 7: return 6; // До 6-и десятых
        case 8: return 7; // До 7-и десятых
        case 9: return 8; // До 8-и десятых
        case 10: return 9; // До 9-и десятых
        case 11: return 2; // До 2-х десятичных - всегда 2 цифры
        case 12: return 3; // До 3-х десятичных - всегда 3 цифры
        default: return 0; // По умолчанию
    }
}
//// Функция преобразования вещественного числа в строку
//inline std::string FloatToStr(double value) {
//    std::ostringstream oss;
//    oss << std::fixed << value;
//    return oss.str();
//}

// Функция преобразования строки в целое число
inline int StrToInt(const std::string &str) {
    return std::stoi(str);
}

//// Функция для преобразования символа в значение
//inline int CharToValue(char32_t c) {
//    static const std::unordered_map<char32_t, int> charMap = []() {
//        std::unordered_map<char32_t, int> m;
//        for (int i = 0; i < 10; ++i) {
//            m[U'0' + i] = i; // цифры '0' - '9'
//        }
//        for (int i = 0; i < 26; ++i) {
//            m[U'A' + i] = 10 + i; // заглавные латинские буквы 'A' - 'Z'
//            m[U'a' + i] = 36 + i; // строчные латинские буквы 'a' - 'z'
//        }
//        const char32_t specialChars[] = U"!#$%&()*+,-./:;<=>?@[]^_`{|}~"; // Исключены двойные и одинарные кавычки, 29 специальных символов
//        for (int i = 0; i < 29; ++i) { // 29 специальных символов
//            m[specialChars[i]] = 62 + i; // специальные символы
//        }
//        return m;
//    }();
//
//    auto it = charMap.find(c);
//    if (it != charMap.end()) {
//        return it->second;
//    } else {
//        std::cerr << "Invalid character in the input string: " << std::hex << c << std::endl;
//        throw std::invalid_argument("Invalid character in the input string");
//    }
//}
//
//// Функция для преобразования значения в символ
//inline char32_t ValueToChar(int value) {
//    static const char32_t charArray[] = U"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+,-./:;<=>?@[]^_`{|}~";
//    size_t arraySize = sizeof(charArray) / sizeof(char32_t) - 1; // Размер массива
//    if (value >= 0 && value < 91) { // Убедимся, что значение в пределах 0-90
//        return charArray[value];
//    } else {
//        throw std::invalid_argument("Value out of range: " + std::to_string(value) + ", arraySize: " + std::to_string(arraySize));
//    }
//}
//
//// Функция для преобразования строки из 91-ричной системы в int
//inline int Base91StringToInt(const std::string& base91Str) {
//    if (base91Str.empty() || base91Str[0] != '$') {
//        throw std::invalid_argument("Input string must start with '$'");
//    }
//
//    // Конвертация std::string в std::u32string
//    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
//    std::u32string base91Part = convert.from_bytes(base91Str.substr(1));
//
//    int result = 0;
//
//    for (char32_t c : base91Part) {
//        result = result * 91 + CharToValue(c);
//    }
//
//    return result;
//}
//
//// Функция для преобразования int в строку 91-ричной системы с фиксированным количеством символов
//inline std::string IntToBase91String(uint64_t value, int digits) {
//    if (value < 0) {
//        throw std::invalid_argument("Negative values are not supported");
//    }
//
//    std::u32string u32result;
//    while (value > 0) {
//        int remainder = value % 91;
//        try {
//            u32result = ValueToChar(remainder) + u32result;
//        } catch (const std::exception& e) {
//            std::cerr << "Error: " << e.what() << std::endl;
//            throw;
//        }
//        value /= 91;
//    }
//
//    // Добавляем ведущие нули до заданного количества цифр
//    while (u32result.length() < static_cast<size_t>(digits)) {
//        u32result = U"0" + u32result;
//    }
//
//    // Конвертация std::u32string в std::string
//    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
//    return convert.to_bytes(u32result);
//}


inline int HexStringToInt(const std::string &hexStr) {
    // Проверка, начинается ли строка с символа $
    if (hexStr.empty() || hexStr[0] != '$') {
        throw std::invalid_argument("Input string must start with '$'");
    }

    // Получение части строки, которая представляет шестнадцатеричное число
    std::string hexPart = hexStr.substr(1);

    // Конвертация шестнадцатеричной строки в int
    int result;
    std::stringstream ss;
    ss << std::hex << hexPart;
    ss >> result;

    if (ss.fail()) {
        throw std::runtime_error("Failed to convert hex string to int");
    }

    return result;
}

// Функция преобразования строки в вещественное число
inline double StrToFloat(const std::string &str) {
//    std::istringstream iss(str);
//    iss.imbue(std::locale("C"));
//    double result;
//    iss >> result;
//    return result;
    // Преобразование строки в число
    std::istringstream iss(str);
    iss.imbue(std::locale("C"));
    double result;
    iss >> result;

    // Определение количества знаков после запятой в строке
    auto pos = str.find('.');
    int precision = 0;
    if (pos != std::string::npos) {
        precision = str.size() - pos - 1;
    } else {
        pos = str.find(',');
        precision = 0;
        if (pos != std::string::npos) {
            precision = str.size() - pos - 1;
        }
    }


    // Округление числа до заданного количества знаков после запятой
    double scale = std::pow(10, precision);
    result = std::round(result * scale) / scale;

    return result;
}


// Функция округления вещественного числа до ближайшего целого
inline int Round(double value) {
    return std::lround(value);
}

// Функция отсечения дробной части вещественного числа
inline int Trunc(double value) {
    return static_cast<int>(value);
}

inline std::string IntToHex(int value) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << value;
    return ss.str();
}

inline std::string IntToHex(uint64_t value, int digits) {
    std::stringstream stream;
    stream << std::hex << std::setw(digits) << std::setfill('0') << value;
    return stream.str();
}

inline char Chr(unsigned char x) {
    return static_cast<char>(x);
}

inline int Ord(char c) {
    return static_cast<int>(c);
}


inline int CurrentYear() {
    // Получаем текущее локальное время
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    // Извлекаем дату
    boost::gregorian::date date = now.date();
    // Возвращаем год
    return date.year();
}

/////////////////////////////Из Math/////////////////////////////////////////////////

// Константа ExtendedResolution
const double ExtendedResolution = std::numeric_limits<double>::epsilon();

inline bool SameValue(const double A, const double B, double Epsilon) {
    if (Epsilon == 0.0) {
        Epsilon = std::max(std::min(std::abs(A), std::abs(B)) * ExtendedResolution, ExtendedResolution);
    }
    if (A > B) {
        return (A - B) <= Epsilon;
    } else {
        return (B - A) <= Epsilon;
    }
}

// Функция проверяет, является ли значение A "нулевым" с заданной точностью Epsilon
inline bool IsZero(double A, double Epsilon = 0.0) {
    if (Epsilon == 0.0) {
        Epsilon = ExtendedResolution;
    }
    return std::fabs(A) <= Epsilon;
}

/////////////////////////////Из StrUtils/////////////////////////////////////////////

inline bool AnsiContainsText(const std::string &AText, const std::string &ASubText) {
    auto it = std::search(
            AText.begin(), AText.end(),
            ASubText.begin(), ASubText.end(),
            [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
    );
    return (it != AText.end());
}

inline std::string AnsiLeftStr(const std::string &AText, int ACount) {
    return AText.substr(0, ACount);
}

inline std::string AnsiRightStr(const std::string &AText, int ACount) {
    if (ACount > static_cast<int>(AText.length())) {
        return AText;
    }
    return AText.substr(AText.length() - ACount);
}

//inline std::string AnsiReplaceStr(const std::string& AText, const std::string& AFromText, const std::string& AToText) {
//    std::string result = AText;
//    size_t start_pos = 0;
//    while((start_pos = result.find(AFromText, start_pos)) != std::string::npos) {
//        result.replace(start_pos, AFromText.length(), AToText);
//        start_pos += AToText.length(); // Обрабатываем возможное перекрытие при замене
//    }
//    return result;
//}
inline std::string AnsiReplaceStr(std::string str, const std::string &oldStr, const std::string &newStr) {
    size_t pos = 0;
    while ((pos = str.find(oldStr, pos)) != std::string::npos) {
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
    return str;
}

/////////////////////////////Из System//////////////////////////////////////////////

inline int Pos(std::string subStr, const std::string &str, int offset = 0) {
    // Проверяем, что смещение находится в пределах строки
    if (offset < 0 || offset > str.size()) {
        return -1; // Возвращаем -1, если смещение неверно
    }

    // Ищем подстроку в строке начиная с offset
    size_t pos = str.find(subStr, offset);
    if (pos != std::string::npos) {
        return static_cast<int>(pos); // Возвращаем найденный индекс
    }
    return -1; // Если подстрока не найдена
}

template<typename T>
inline void Val(const std::string &s, T &v, int &code) {
    try {
        size_t idx;
        v = std::stoi(s, &idx);
        if (idx != s.size()) {
            code = 1; // Не вся строка использована для преобразования
        } else {
            code = 0; // Успешное преобразование
        }
    } catch (const std::invalid_argument &) {
        code = 2; // Строка не содержит валидную числовую информацию
    } catch (const std::out_of_range &) {
        code = 3; // Число слишком большое/маленькое для int
    }
}

inline std::string Copy(const std::string &s, int index, int count) {
    if (index < 1) {
        throw std::out_of_range("Index should be at least 1");
    }
    // Поскольку в Delphi индексация начинается с 1, необходимо скорректировать это для C++
    size_t zeroBasedIndex = static_cast<size_t>(index - 1);

    if (zeroBasedIndex > s.length()) {
        // Если индекс начала выходит за пределы строки, возвращаем пустую строку
        return "";
    }

    // Вырезаем подстроку. Если количество символов, которое нужно скопировать, больше,
    // чем оставшаяся часть строки, substr автоматически вернёт до конца строки.
    return s.substr(zeroBasedIndex, static_cast<size_t>(count));
}

inline std::wstring Copy(const std::wstring &str, int start, int length) {
    return str.substr(start - 1, length);
}

// Функция для копирования подстроки, аналог функции copy из Delphi
inline std::u16string Copy(const std::u16string &str, int start, int length) {
    return str.substr(start - 1, length);
}


// Преобразование UTF-16 строки в UTF-8
inline std::string utf16_to_utf8(const std::u16string &utf16) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    return converter.to_bytes(utf16);
}

inline std::string StringReplace(const std::string &original, const std::string &from, const std::string &to) {
    std::string result = original;
    size_t startPos = 0;

    // Поиск начальной позиции подстроки
    while ((startPos = result.find(from, startPos)) != std::string::npos) {
        // Замена найденной подстроки
        result.replace(startPos, from.length(), to);
        startPos += to.length(); // Перемещение вперед в результате, чтобы избежать зацикливания
    }

    return result;
}

//inline int PosEx(const std::string& subStr, const std::string& str, int offset = 1) {
//    // В Delphi индексация начинается с 1, в C++ с 0, поэтому необходимо корректировать смещение
//    size_t cppOffset = static_cast<size_t>(offset - 1);
//
//    if (cppOffset >= str.size()) {
//        return 0; // Если смещение больше длины строки, возвращаем 0, как и в Delphi
//    }
//
//    // Ищем подстроку в строке, начиная с указанного смещения
//    size_t pos = str.find(subStr, cppOffset);
//
//    if (pos == std::string::npos) {
//        return 0; // Если подстрока не найдена, возвращаем 0
//    }
//
//    // Возвращаем позицию подстроки, корректируя её обратно к индексации, начинающейся с 1
//    return static_cast<int>(pos + 1);
//}
inline size_t PosEx(const std::string &substr, const std::string &str, size_t offset) {
    size_t pos = str.find(substr, offset);
    return (pos == std::string::npos) ? 0 : pos + 1;
}

/////////////////////////////Из SysUtils/////////////////////////////////////////////
inline std::string AnsiUpperCase(const std::string &input) {
    std::locale loc;
    std::string output = input;

    std::transform(output.begin(), output.end(), output.begin(),
                   [&loc](char c) { return std::toupper(c, loc); });

    return output;
}



/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////TDateTime///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

class TDateTime {
private:
    boost::posix_time::ptime dateTime;

public:

    // Конструктор по умолчанию
    TDateTime() : dateTime(boost::posix_time::second_clock::local_time()) {}

    // Конструктор, принимающий ptime
    TDateTime(const boost::posix_time::ptime &dt) : dateTime(dt) {}

    TDateTime(const boost::gregorian::date &date) : dateTime(boost::posix_time::ptime(date)) {}

    boost::posix_time::ptime getDateTime() const {
        return dateTime;
    }

    // Конструктор из строки вида "yyyy-mm-dd hh:mm:ss"
    TDateTime(const std::string &dateTimeStr) {
        std::istringstream dateStream(dateTimeStr);
        dateStream.imbue(
                std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%d %H:%M:%S")));
        dateStream >> dateTime;
        if (dateStream.fail()) {
            throw std::runtime_error("Failed to parse date string");
        }
    }

    // Конструктор, принимающий целое число
    TDateTime(int daysFrom1899) {
        if (daysFrom1899 == 0) {
            // Инициализация даты "30.12.1899"
            dateTime = boost::posix_time::ptime(boost::gregorian::date(1899, 12, 30));
        } else {
            // Инициализация текущей даты или другой логики
            dateTime = boost::posix_time::second_clock::local_time();
        }
    }

    // Метод для преобразования даты в формат "dd.mm.yyyy"
    std::string ToFormattedDate() const {
        std::ostringstream ss;
        boost::gregorian::date d = dateTime.date();
        ss << std::setw(2) << std::setfill('0') << d.day().as_number() << "."
           << std::setw(2) << std::setfill('0') << d.month().as_number() << "."
           << d.year();
        return ss.str();
    }

    // Возвращает дату и время в виде строки
    std::string ToString() const {
        std::ostringstream ss;
        boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S");
        ss.imbue(std::locale(std::locale::classic(), facet));
        ss << dateTime;
        return ss.str();
    }

    // Метод для преобразования строки формата "dd.mm.yyyy" в TDateTime
    static TDateTime FromFormattedDateString(const std::string &dateStr) {
        std::istringstream dateStream(dateStr);
        std::tm tm = {};
        char delimiter;
        dateStream >> std::setw(2) >> tm.tm_mday >> delimiter
                   >> std::setw(2) >> tm.tm_mon >> delimiter
                   >> std::setw(4) >> tm.tm_year;
        tm.tm_mon -= 1; // std::tm месяцы начинаются с 0
        tm.tm_year -= 1900; // std::tm года начинаются с 1900
        if (dateStream.fail()) {
            throw std::runtime_error("Failed to parse date string");
        }
        boost::gregorian::date d(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        return TDateTime(boost::posix_time::ptime(d));
    }

    // Добавляет дни к текущей дате
    void AddDays(int days) {
        dateTime += boost::gregorian::days(days);
    }

    // Добавляет секунды к текущему времени
    void AddSeconds(int seconds) {
        dateTime += boost::posix_time::seconds(seconds);
    }

    // Возвращает разницу между двумя TDateTime в секундах
    long TimeDifference(const TDateTime &other) const {
        boost::posix_time::time_duration td = dateTime - other.dateTime;
        return td.total_seconds();
    }

    // Разделяет значение TDateTime на год, месяц и день
    void DecodeDate(int &year, int &month, int &day) const {
        boost::gregorian::date d = dateTime.date();
        year = d.year();
        month = d.month();
        day = d.day();
    }

    // Разделяет значение TDateTime на час, минуту, секунду и миллисекунду
    void DecodeTime(int &hour, int &min, int &sec, int &msec) const {
        boost::posix_time::time_duration td = dateTime.time_of_day();
        hour = td.hours();
        min = td.minutes();
        sec = td.seconds();
        msec = td.total_milliseconds() -
               (td.hours() * 3600000 + td.minutes() * 60000 + td.seconds() * 1000);
    }

    // Операторы сравнения
    bool operator>(const TDateTime &other) const {
        return dateTime > other.dateTime;
    }

    bool operator<(const TDateTime &other) const {
        return dateTime < other.dateTime;
    }

    bool operator>=(const TDateTime &other) const {
        return dateTime >= other.dateTime;
    }

    bool operator<=(const TDateTime &other) const {
        return dateTime <= other.dateTime;
    }

    bool operator==(const TDateTime &other) const {
        return dateTime == other.dateTime;
    }

    bool operator!=(const TDateTime &other) const {
        return dateTime != other.dateTime;
    }

    boost::gregorian::date getDate() const {
        return dateTime.date();
    }

    // Оператор сложения двух TDateTime
    TDateTime operator+(const TDateTime &other) const {
        // Вычисляем разницу во времени между текущим dateTime и other.dateTime
        boost::posix_time::time_duration duration =
                other.dateTime - boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
        // Добавляем эту разницу к текущему dateTime
        return TDateTime(dateTime + duration);
    }
};

//// Эта функция строку с форматом "%Y-%m-%d %H:%M:%S" преобразует в объект TDateTime
//inline TDateTime StrToDateYYYYMMDD(const std::string& dateStr) {
//    std::string format;
//    if (dateStr.find(' ') == std::string::npos) {
//        // Если строка содержит только дату
//        format = "%Y-%m-%d";  // Формат только для даты
//    } else {
//        // Если строка содержит дату и время
//        format = "%Y-%m-%d %H:%M:%S";  // Формат для даты и времени
//    }
//
//    boost::posix_time::ptime parsedDateTime;
//    std::istringstream dateStream(dateStr);
//    dateStream.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet(format)));
//    dateStream >> parsedDateTime;
//
//    if (dateStream.fail()) {
//        throw std::runtime_error("Failed to parse date string");
//    }
//
//    return TDateTime(parsedDateTime);
//}
//// Эта функция преобразует объект TDateTime в строку с форматом "%Y-%m-%d %H:%M:%S"
//inline std::string DateToStrYYYYMMDD(const TDateTime& dateTime) {
//    std::ostringstream ss;
//    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S");
//    ss.imbue(std::locale(std::locale::classic(), facet));
//    ss << dateTime.getDateTime();
//    return ss.str();
//}

// Функция для вычисления разницы в часах между двумя объектами TDateTime
inline double HoursBetween(const TDateTime &dt1, const TDateTime &dt2) {
    long secondsDiff = dt1.TimeDifference(dt2); // Получаем разницу в секундах
    return std::abs(secondsDiff) / 3600.0; // Переводим секунды в часы и берем модуль разности
}

inline TDateTime UnixTimestampToDateTime(double unixTimestamp) {
    // Вычисляем количество секунд и миллисекунд в unixTimestamp
    long seconds = static_cast<long>(unixTimestamp);
    long microseconds = static_cast<long>((unixTimestamp - seconds) * 1000000);

    // Преобразуем количество секунд с начала эпохи Unix в boost::posix_time::ptime
    boost::posix_time::ptime unixEpoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime dateTime =
            unixEpoch + boost::posix_time::seconds(seconds) + boost::posix_time::microseconds(microseconds);

    // Создаем объект TDateTime с преобразованным значением
    return TDateTime(dateTime);
}

inline long long DateTimeToUnixTimestamp(const TDateTime &dateTime) {
    // Получаем текущее время в формате boost::posix_time::ptime
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime time = dateTime.getDateTime();

    // Вычисляем разницу во времени между заданным dateTime и началом эпохи Unix
    boost::posix_time::time_duration diff = time - epoch;

    // Преобразуем разницу в секунды
    long long seconds = diff.total_seconds();

    // Возвращаем количество секунд в виде long long
    return seconds;
}


// Функция Now для получения текущего времени
inline static TDateTime Now() {
    return TDateTime(boost::posix_time::second_clock::local_time());
}

// Возвращает год из объекта TDateTime
inline int YearOf(const TDateTime &dt) {
    return dt.getDateTime().date().year();
}

// Возвращает месяц из объекта TDateTime
inline int MonthOf(const TDateTime &dt) {
    return dt.getDateTime().date().month();
}

// Возвращает день из объекта TDateTime
inline int DayOf(const TDateTime &dt) {
    return dt.getDateTime().date().day();
}

// Функция вычисления разницы в днях между двумя датами
inline int DaysBetween(const TDateTime &start, const TDateTime &end) {
    boost::gregorian::date_duration dd = end.getDate() - start.getDate();
    return dd.days();
}

// Функция для определения количества дней в месяце
inline int DaysInAMonth(int year, int month) {
    return boost::gregorian::gregorian_calendar::end_of_month_day(year, month);
}

// Глобальная функция для создания TDateTime
inline TDateTime EncodeDateTime(int year, int month, int day,
                                int hour, int minute, int second, int millisecond) {
    try {
        boost::gregorian::date date(year, month, day);
        boost::posix_time::time_duration time(hour, minute, second, millisecond);
        boost::posix_time::ptime dateTime(date, time);
        return TDateTime(dateTime);
    } catch (std::exception &e) {
        //throw std::runtime_error("Invalid date or time: " + std::string(e.what()));
    }
}

// Функция для создания времени
inline TDateTime EncodeTime(int hour, int min, int sec, int msec) {
    if (hour < 0 || hour > 23 || min < 0 || min > 59 ||
        sec < 0 || sec > 59 || msec < 0 || msec > 999) {
        throw std::invalid_argument("Неверно указано время: значение за пределами допустимого диапазона.");
    }

    boost::posix_time::time_duration td(hour, min, sec, msec);
    boost::posix_time::ptime dt(boost::gregorian::date(1400, 1, 1), td);  // Используем фиктивную дату
    return TDateTime(dt);
}

// Функция для преобразования TDateTime в строку
inline std::string DateTimeToStr(const TDateTime &dateTime) {
    if (dateTime == 0)
        return "";
    return dateTime.ToString();
}

// Функция для преобразования строки в TDateTime
inline TDateTime StrToDateTime(const std::string &dateTimeStr) {
    return TDateTime(dateTimeStr);
}

inline TDateTime GetLastDayDate(int aYear, int aKv, int aMes) {
    int month;
    if (aMes == 0) {
        if (aKv == 0) {
            month = 12; // Если квартал не указан, используем последний месяц года
        } else {
            month = aKv * 3; // Последний месяц указанного квартала
        }
    } else {
        month = aMes;
    }

    // Создаем дату для первого дня выбранного месяца
    boost::gregorian::date firstDayOfMonth(aYear, month, 1);

    // Получаем последний день месяца
    boost::gregorian::date lastDayOfMonth = firstDayOfMonth.end_of_month();

    // Возвращаем TDateTime, инициализированный последним днем месяца
    return TDateTime(boost::posix_time::ptime(lastDayOfMonth));
}

// Функция для получения даты из значения TDateTime
inline TDateTime DateOf(const TDateTime &AValue) {
    boost::gregorian::date onlyDate = AValue.getDate(); // Получаем только дату
    return TDateTime(onlyDate); // Создаем новый объект TDateTime, используя дату
}

inline double ConvertToDouble(const TDateTime &dt) {
    boost::posix_time::ptime epoch(boost::gregorian::date(1899, 12, 30));
    boost::posix_time::time_duration diff = dt.getDateTime() - epoch;
    return diff.total_seconds() / (24 * 60 * 60); // переводим в дни
}

inline TDateTime IncDay(const TDateTime &AValue, int ANumberOfDays = 1) {
    TDateTime result = AValue; // Создаем копию переданной даты
    result.AddDays(ANumberOfDays); // Увеличиваем дату на ANumberOfDays дней
    return result;
}

//#ifdef _WIN32
////#include <windows.h>
////
////std::string GetTempPath() {
////    char tmp[MAX_PATH];
////    DWORD len = ::GetTempPathA(MAX_PATH, tmp);  // Получаем временный путь
////    if (len == 0) {
////        return "";  // В случае ошибки возвращаем пустую строку
////    }
////
////    char longPath[MAX_PATH];
////    DWORD longLen = ::GetLongPathNameA(tmp, longPath, MAX_PATH);  // Преобразуем в полный путь
////    if (longLen == 0 || longLen > MAX_PATH) {
////        return tmp;  // Возвращаем короткий путь, если не удалось преобразовать
////    }
////
////    return std::string(longPath);
////}
//
//#else
//#include <cstdlib>
//
//std::string GetTempPath() {
//    const char* path = std::getenv("TMPDIR");  // Проверяем переменную окружения TMPDIR
//    if (!path) {
//        path = "/tmp";  // Запасной вариант, если TMPDIR не задана
//    }
//    return std::string(path);
//}
//
//#endif

/////////////////////////////Из SysUtils/////////////////////////////////////////////

inline static TDateTime EncodeDate(unsigned short year, unsigned short month, unsigned short day) {
    try {
        boost::gregorian::date d(year, month, day);
        return TDateTime(boost::posix_time::ptime(d));
    } catch (std::exception &e) {
        // Обработка ошибки, например, некорректная дата
        std::cerr << "Invalid date: " << e.what() << std::endl;
        return TDateTime(); // Возвращаем "нулевую" дату или выбрасываем исключение
    }
}

inline void DecodeDate(const TDateTime &dateTime, int &year, int &month, int &day) {
    boost::gregorian::date d = dateTime.getDateTime().date();
    year = d.year();
    month = d.month();
    day = d.day();
}

inline static TDateTime IncMonth(const TDateTime &dateTime, int numberOfMonths = 1) {
    boost::posix_time::ptime pt = dateTime.getDateTime();
    boost::gregorian::date_duration dd(0);  // Для добавления дней, если нужно
    boost::gregorian::months ms(numberOfMonths);
    boost::gregorian::date new_date = pt.date() + ms;
    return TDateTime(boost::posix_time::ptime(new_date, pt.time_of_day()));
}

inline static std::string FormatDateTime(const std::string &format, const TDateTime &dateTime) {
    std::ostringstream ss;
    boost::posix_time::time_facet *facet = new boost::posix_time::time_facet(format.c_str());
    ss.imbue(std::locale(std::locale::classic(), facet));
    ss << dateTime.getDateTime();
    return ss.str();
}

inline static TDateTime Date() {
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::gregorian::date today = now.date();
    return TDateTime(boost::posix_time::ptime(today)); // Возвращаем ptime только с датой
}

// Функция для получения номера квартала по номеру месяца
inline int GetNKv(int month) {
    if (month < 1 || month > 12) {
        std::cerr << "Invalid month: " << month << ". Month must be between 1 and 12." << std::endl;
        return -1;  // Возвращаем ошибку, если номер месяца некорректен
    }
    return (month - 1) / 3 + 1;  // Вычисляем квартал
}

///////////////////////////////////////////////////////////////////////////////////
// Базовый класс для объектов, которые можно хранить в TStringList
class TObject {
public:
    virtual ~TObject() {}
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////TList///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



template<typename T>
class TList {

public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Конструкторы
    TList() = default;

    // Добавление элемента в конец
    void Add(const T &item) {
        items.push_back(item);
    }

    // Вставка элемента по индексу
    void Insert(std::size_t index, const T &item) {
        if (index <= items.size()) {
            items.insert(items.begin() + index, item);
        } else {
            throw std::out_of_range("Индекс выходит за пределы списка");
        }
    }

    // Удаление элемента по индексу
    void Delete(std::size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
        } else {
            throw std::out_of_range("Индекс выходит за пределы списка");
        }
    }

    // Очистка списка
    void Clear() {
        for (T &item: items) {
            delete item;  // Освобождение памяти, выделенной для объектов
        }
        items.clear();  // Очистка самого списка
    }


    // Получение количества элементов
    std::size_t Count() const {
        return items.size();
    }

    // Получение или установка элемента по индексу
    T &operator[](std::size_t index) {
        if (index < items.size()) {
            return items[index];
        } else {
            throw std::out_of_range("Индекс выходит за пределы списка");
        }
    }

    const T &operator[](std::size_t index) const {
        if (index < items.size()) {
            return items[index];
        } else {
            throw std::out_of_range("Индекс выходит за пределы списка");
        }
    }

    // Итераторы
    iterator begin() { return items.begin(); }

    iterator end() { return items.end(); }

    const_iterator begin() const { return items.begin(); }

    const_iterator end() const { return items.end(); }

    // Сортировка списка с использованием пользовательской функции сравнения
    void Sort(std::function<bool(const T &, const T &)> compare) {
        std::sort(items.begin(), items.end(), compare);
    }

    // Поиск элемента
    iterator Find(const T &value) {
        return std::find(items.begin(), items.end(), value);
    }

    // Разворот списка
    void Reverse() {
        std::reverse(items.begin(), items.end());
    }

    // Проверка, пуст ли список
    bool IsEmpty() const {
        return items.empty();
    }

    void SetCount(size_t count) {
        items.resize(count);
    }

    size_t GetCount() const {
        return items.size();
    }

    // Обмен значениями двух элементов по индексам
    void Exchange(int index1, int index2) {
        if (index1 >= 0 && index1 < Count() && index2 >= 0 && index2 < Count()) {
            std::swap(items[index1], items[index2]);
        }
    }

    std::vector<T> items;
};


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////TStringList/////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


// Функция для сравнения строк без учёта регистра
struct CaseInsensitiveCompare {
    bool operator()(const std::string &a, const std::string &b) const {
        return std::lexicographical_compare(
                a.begin(), a.end(), b.begin(), b.end(),
                [](char ac, char bc) { return std::tolower(ac) < std::tolower(bc); }
        );
    }
};



//class TQueryUni : public TObject {};

using MyVariantSL = std::variant<
        std::shared_ptr<int>, std::shared_ptr<TObject>,
        int, TCellValueAtr *, TQueryUni *,
        std::shared_ptr<TF1FlexCelFormulas>, PrcrdFM,
        std::shared_ptr<Tbov67>, Pbov15,
        PEkonBPS, Pftb,
        std::shared_ptr<TList<TObject>>,
        std::shared_ptr<TList<PKntrkt>>,
        std::shared_ptr<TList<PDgvr15>>,
        std::shared_ptr<TList<PTorgi>>,
        std::shared_ptr<TStyle>,
        TFormLogicF1 *
>;

class TStringList {
public:
    std::map<std::string, MyVariantSL> items;
    bool ownsObjects;
    char delimiter = ',';  // Значение по умолчанию

    TStringList(bool owns = true) : ownsObjects(owns), Strings(*this), Objects(*this), Names(*this), Values(*this) {}

    virtual ~TStringList() {
        //Clear(); // Убедитесь, что очищается память, если это необходимо
    }

//    template<typename T>
//    T* getObjectByKey(const std::string& key) const {
//        auto it = items.find(key);
//        if (it != items.end()) {
//            try {
//                return std::get<T*>(it->second);
//            } catch (const std::bad_variant_access&) {
//                // Запись лога ошибки или другая обработка
//                std::cerr << "Bad variant access: the type does not match" << std::endl;
//            }
//        }
//        return nullptr;
//    }
    template<typename T>
    T getObjectByKey(const std::string &key) const {
        auto it = items.find(key);
        if (it != items.end()) {
            try {
                // Важно: std::get<T> напрямую возвращает тип T
                return std::get<T>(it->second);
            } catch (const std::bad_variant_access &) {
                // Запись лога ошибки или другая обработка
                std::cerr << "Bad variant access: the type does not match" << std::endl;
            }
        }
        return nullptr; // Возвращаем nullptr, который соответствует типу T (уже указатель)
    }

    void Add(const std::string &item) {
        items[item] = std::shared_ptr<TObject>(nullptr);
    }

    void SetObject(const std::string &key, const MyVariantSL &newObj) {
        items[key] = newObj;
    }

    void AddObject(const std::string &key, const MyVariantSL &obj) {
        items[key] = obj;
    }

    void AddObject(const std::string &key, std::shared_ptr<TList<PKntrkt>> listPtr) {
        items[key] = listPtr;
    }

    void AddObject(const std::string &key, std::shared_ptr<TStyle> stylePtr) {
        items[key] = stylePtr;
    }

    void AddObject(const std::string &key, TFormLogicF1 *FormLogicF1) {
        items[key] = FormLogicF1;
    }

    void AddObject(const std::string &key, std::shared_ptr<TList<PDgvr15>> listPtr) {
        items[key] = listPtr;
    }

    void AddObject(const std::string &key, std::shared_ptr<TList<PTorgi>> listPtr) {
        items[key] = listPtr;
    }

    int AddObject(const std::string &key, PEkonBPS ekonBPS) {
        items[key] = ekonBPS;
        return static_cast<int>(items.size() - 1);
    }

    int AddObject(const std::string &key, std::shared_ptr<TList<TObject>> listPtr) {
        items[key] = listPtr;
        return static_cast<int>(items.size() - 1);
    }

    int AddObject(const std::string &key, Pftb Ftb) {
        items[key] = Ftb;
        return static_cast<int>(items.size() - 1);
    }

    int AddObject(const std::string &key, Pbov15 Pbov15) {
        items[key] = Pbov15;
        return static_cast<int>(items.size() - 1);
    }

    int AddObject(const std::string &key, PrcrdFM object) {
        items[key] = object;
        return static_cast<int>(items.size() - 1);
    }

    int AddObject(const std::string &key, const std::shared_ptr<TF1FlexCelFormulas> &object) {
        items[key] = object;
        return static_cast<int>(items.size() - 1);
    }

    void AddObject(const std::string &key, TQueryUni *queryUni) {
        items[key] = queryUni;
    }

    bool Find(const std::string &S, int &Index) const {
        auto it = items.find(S);
        if (it != items.end()) {
            Index = std::distance(items.begin(), it);
            return true;
        }
        Index = -1;
        return false;
    }

    int IndexOf(const std::string &S) const {
        auto it = items.find(S);
        if (it != items.end()) {
            return std::distance(items.begin(), it);
        }
        return -1;
    }

    size_t Count() const {
        return items.size();
    }

    void Remove(const std::string &key) {
        items.erase(key);
    }

    void Delete(int index) {
        if (index < 0 || index >= static_cast<int>(items.size())) {
            throw std::out_of_range("Index out of range");
        }
        auto it = std::next(items.begin(), index);
        items.erase(it);
    }

    std::string &operator[](size_t index) {
        if (index >= items.size()) {
            throw std::out_of_range("Index out of range");
        }
        auto it = std::next(items.begin(), index);
        return const_cast<std::string &>(it->first);
    }

    const std::string &operator[](size_t index) const {
        if (index >= items.size()) {
            throw std::out_of_range("Index out of range");
        }
        auto it = std::next(items.begin(), index);
        return it->first;
    }

    MyVariantSL &GetObject(size_t index) {
        if (index >= items.size()) {
            throw std::out_of_range("Index out of range");
        }
        auto it = std::next(items.begin(), index);
        return it->second;
    }

    void Clear() {
        items.clear();
    }

    void PrintAll() const {
        for (const auto &item: items) {
            std::cout << "String: " << item.first << std::endl;
        }
    }

    std::string DelimitedText() const {
        std::string result;
        for (auto it = items.begin(); it != items.end(); ++it) {
            result += it->first;
            if (std::next(it) != items.end()) result += delimiter;
        }
        return result;
    }

    void SetDelimitedText(const std::string &text) {
        items.clear();
        std::istringstream iss(text);
        std::string token;
        while (std::getline(iss, token, delimiter)) {
            items[token] = std::shared_ptr<TObject>(nullptr);
        }
    }

    int IndexOfName(const std::string &name) const {
        for (auto it = items.begin(); it != items.end(); ++it) {
            size_t pos = it->first.find('=');
            if (pos != std::string::npos && it->first.substr(0, pos) == name) {
                return std::distance(items.begin(), it);
            }
        }
        return -1;
    }

    int AddObject(const std::string &key, int value) {
        auto intPtr = std::make_shared<int>(value);
        items[key] = intPtr;
        return static_cast<int>(items.size() - 1);
    }

    int AddObject(const std::string &key, TObject *object) {
        auto objPtr = std::make_shared<TObject>(*object);
        items[key] = objPtr;
        return static_cast<int>(items.size() - 1);
    }

    int AddObject(const std::string &key, PCellValueAtr value) {
        items[key] = value;
        return static_cast<int>(items.size() - 1);
    }

    int GetObjectValue(int index) {
        if (index < 0 || index >= static_cast<int>(items.size())) {
            throw std::out_of_range("Index out of range");
        }
        auto it = std::next(items.begin(), index);
        const auto &variant = it->second;
        if (std::holds_alternative<int>(variant)) {
            return std::get<int>(variant);
        } else {
            throw std::runtime_error("Expected integer but got different type");
        }
    }

    class TextProxy {
    private:
        TStringList &list;

    public:
        TextProxy(TStringList &list) : list(list) {}

        TextProxy &operator=(const std::string &newText) {
            list.Clear();
            std::istringstream iss(newText);
            std::string line;
            while (std::getline(iss, line)) {
                list.Add(line);
            }
            return *this;
        }

        operator std::string() const {
            std::ostringstream oss;
            for (auto it = list.items.begin(); it != list.items.end(); ++it) {
                oss << it->first;
                if (std::next(it) != list.items.end()) {
                    oss << "\n";
                }
            }
            return oss.str();
        }
    };

    TextProxy Text{*this};

    class CommaTextProxy {
    private:
        TStringList &list;

    public:
        CommaTextProxy(TStringList &list) : list(list) {}

        CommaTextProxy &operator=(const std::string &text) {
            list.Clear();
            std::istringstream iss(text);
            std::string token;
            while (getline(iss, token, ',')) {
                size_t start = token.find_first_not_of(' ');
                if (start != std::string::npos && token[start] == '"') {
                    std::string fullItem;
                    if (token.back() != '"') {
                        fullItem = token.substr(start + 1);
                        while (getline(iss, token, ',') && token.back() != '"') {
                            fullItem += ',' + token;
                        }
                        fullItem += ',' + token.substr(0, token.size() - 1);
                    } else {
                        fullItem = token.substr(start + 1, token.size() - start - 2);
                    }
                    list.items[fullItem] = std::shared_ptr<TObject>(nullptr);
                } else {
                    list.items[token] = std::shared_ptr<TObject>(nullptr);
                }
            }
            return *this;
        }

        operator std::string() const {
            std::ostringstream oss;
            for (auto it = list.items.begin(); it != list.items.end(); ++it) {
                if (it->first.find(',') != std::string::npos) {
                    oss << "\"" << it->first << "\"";
                } else {
                    oss << it->first;
                }
                if (std::next(it) != list.items.end()) {
                    oss << ",";
                }
            }
            return oss.str();
        }
    };

    CommaTextProxy CommaText{*this};

    class StringsProxy {
    private:
        TStringList &list;

    public:
        StringsProxy(TStringList &list) : list(list) {}

        std::string operator[](int index) const {
            if (index < 0 || index >= static_cast<int>(list.items.size())) {
                throw std::out_of_range("Index out of range");
            }
            return list[index];
        }

        void Set(int index, const std::string &value) {
            if (index < 0 || index >= static_cast<int>(list.items.size())) {
                throw std::out_of_range("Index out of range");
            }
            auto it = std::next(list.items.begin(), index);
            auto node_handle = list.items.extract(it);
            node_handle.key() = value;
            list.items.insert(std::move(node_handle));
        }
    };

    StringsProxy Strings;

    class ObjectsProxy {
    private:
        TStringList &list;

    public:
        ObjectsProxy(TStringList &list) : list(list) {}

        MyVariantSL &Get(int index) {
            if (index < 0 || index >= static_cast<int>(list.items.size())) {
                throw std::out_of_range("Index out of range");
            }
            auto it = std::next(list.items.begin(), index);
            return it->second;
        }

        MyVariantSL operator[](int index) const {
            int sz = list.items.size();
            if (index < 0 || index >= static_cast<int>(list.items.size())) {
                throw std::out_of_range("Index out of range");
            }
            auto it = std::next(list.items.begin(), index);
            return it->second;
        }

        void Set(int index, const MyVariantSL &value) {
            if (index < 0 || index >= static_cast<int>(list.items.size())) {
                throw std::out_of_range("Index out of range");
            }
            auto it = std::next(list.items.begin(), index);
            it->second = value;
        }
    };

    ObjectsProxy Objects;

    class NamesProxy {
    private:
        TStringList &list;

    public:
        NamesProxy(TStringList &list) : list(list) {}

        std::string operator[](int index) const {
            if (index < 0 || index >= static_cast<int>(list.items.size())) {
                throw std::out_of_range("Index out of range");
            }
            auto it = std::next(list.items.begin(), index);
            const std::string &str = it->first;
            size_t equalPos = str.find('=');
            if (equalPos != std::string::npos) {
                return str.substr(0, equalPos);
            }
            return "";
        }
    };

    NamesProxy Names;

    class ValuesProxy {
    private:
        TStringList &list;

    public:
        ValuesProxy(TStringList &list) : list(list) {}

        std::string operator[](int index) const {
            if (index < 0 || index >= static_cast<int>(list.items.size())) {
                throw std::out_of_range("Index out of range");
            }
            auto it = std::next(list.items.begin(), index);
            const std::string &str = it->first;
            size_t equalPos = str.find('=');
            if (equalPos != std::string::npos) {
                return str.substr(equalPos + 1);
            }
            return "";
        }

        std::string operator[](const std::string &key) const {
            auto it = list.items.find(key);
            if (it != list.items.end()) {
                const std::string &str = it->first;
                size_t equalPos = str.find('=');
                if (equalPos != std::string::npos) {
                    return str.substr(equalPos + 1);
                }
            }
            return "";
        }
    };

    ValuesProxy Values;

    std::string GetName(int index) const {
        return Names[index];
    }

    std::string GetValue(int index) const {
        return Values[index];
    }
};

class TSampleObject : public TObject {
public:
    TSampleObject() {}

    ~TSampleObject() {}
};

class ToStringVisitor {
public:
    std::string operator()(int i) const {
        return std::to_string(i);
    }

    std::string operator()(const std::string &str) const {
        return str;
    }

    std::string operator()(const TDateTime &dt) const {
        return dt.ToString();
    }
};

typedef std::variant<int, std::string, TDateTime, float> MyVariant;

inline std::string VariantToString(const MyVariant &value) {
    return std::visit(ToStringVisitor(), value);
}



/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////TDictionary/////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

template<typename TKey, typename TValue>
class TDictionary {
private:
    std::unordered_map<TKey, TValue> map;

public:
    // Добавление пары ключ-значение
    void Add(const TKey &key, const TValue &value) {
        auto result = map.insert(std::make_pair(key, value));
        if (!result.second) {
            //throw std::runtime_error("Key already exists in the dictionary");
        }
    }

    // Проверка на наличие значения
    bool ContainsValue(const TValue &value) const {
        for (const auto &pair: map) {
            if (pair.second == value) {
                return true;
            }
        }
        return false;
    }

    // Удаление элемента по ключу
    void Remove(const TKey &key) {
        auto count = map.erase(key);
        if (count == 0) {
            //throw std::runtime_error("Key not found in the dictionary");
        }
    }

    // Получение значения по ключу
    TValue &Get(const TKey &key) {
        auto it = map.find(key);
        if (it == map.end()) {
            //throw std::runtime_error("Key not found in the dictionary");
        }
        return it->second;
    }

    // Проверка на наличие ключа
    bool ContainsKey(const TKey &key) const {
        return map.find(key) != map.end();
    }

    // Очистка словаря
    void Clear() {
        map.clear();
    }

    // Получение размера словаря
    size_t Count() const {
        return map.size();
    }

    // Пытается получить значение по ключу. Возвращает true, если ключ найден.
    bool TryGetValue(const TKey &key, TValue &value) {
        auto it = map.find(key);
        if (it != map.end()) {
            value = it->second;
            return true;
        }
        return false;
    }
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////TStack//////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class TStack {
private:
    std::vector<T> items; // Используем std::vector для хранения элементов стека

public:
    void push(const T &item) {
        items.push_back(item); // Добавляем элемент в конец вектора
    }

    T pop() {
        if (items.empty()) {
            throw std::out_of_range("Stack is empty"); // Если стек пуст, выбрасываем исключение
        }
        T top = items.back(); // Получаем последний элемент
        items.pop_back(); // Удаляем последний элемент
        return top; // Возвращаем последний элемент
    }

    T &top() {
        if (items.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return items.back(); // Возвращаем ссылку на последний элемент
    }

    bool empty() const {
        return items.empty(); // Возвращает true, если стек пуст
    }

    size_t size() const {
        return items.size(); // Возвращает количество элементов в стеке
    }
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////TFormatSettings/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

class TFormatSettings {
private:
    std::vector<std::string> longMonthNames;
    std::vector<std::string> longDayNames;

public:
    TFormatSettings() {
        // Инициализация русскими названиями месяцев
        longMonthNames = {
                "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
                "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
        };

        // Инициализация русскими названиями дней недели
        longDayNames = {
                "Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"
        };
    }

    // Функция для установки названий месяцев
    void setLongMonthNames(const std::vector<std::string> &names) {
        if (names.size() == 12) {
            longMonthNames = names;
        }
    }

    // Функция для получения названия месяца по номеру
    std::string LongMonthNames(int month) {
        if (month > 0 && month <= 12) {
            return longMonthNames[month - 1];
        }
        return ""; // Возвращаем пустую строку, если номер месяца некорректен
    }

    // Функция для установки названий дней недели
    void setLongDayNames(const std::vector<std::string> &names) {
        if (names.size() == 7) {
            longDayNames = names;
        }
    }

    // Функция для получения названия дня недели по номеру
    std::string getLongDayName(int day) {
        if (day >= 0 && day < 7) {
            return longDayNames[day];
        }
        return ""; // Возвращаем пустую строку, если номер дня некорректен
    }
};

#endif //MWSRESTAPI_CLASSESUTILS_H
