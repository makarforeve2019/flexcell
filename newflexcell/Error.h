// Error.h
#ifndef FORMULAS_PARSER_ERROR_H
#define FORMULAS_PARSER_ERROR_H

#include <string>
#include <variant>

// Тип для значений ячейки
using Value = std::variant<double, std::string, bool>;

// Перечисление кодов ошибок
enum class ErrorCode {
    VALUE,  // #VALUE! - Синтаксическая ошибка
    NAME,   // #NAME! - Неизвестная функция или ячейка
    DIV0,   // #DIV/0! - Деление на ноль
    NA      // #N/A - Значение недоступно
};

// Структура для ошибки
struct Error {
    ErrorCode code;
    std::string message;
};

// Класс для возврата результата или ошибки
struct Result {
    std::variant<Value, Error> value;

    bool is_success() const {
        return std::holds_alternative<Value>(value);
    }

    Value get_value() const {
        return std::get<Value>(value);
    }

    Error get_error() const {
        return std::get<Error>(value);
    }
};

#endif // FORMULAS_PARSER_ERROR_H