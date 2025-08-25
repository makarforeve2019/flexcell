// FormulaParser.h
#ifndef FORMULAS_PARSER_FORMULA_PARSER_H
#define FORMULAS_PARSER_FORMULA_PARSER_H

#include "Error.h"
#include "FormulaAST.h"
#include <string>

// Результат для FormulaAST
struct FormulaASTResult {
    std::variant<FormulaAST, Error> value;

    bool is_success() const {
        return std::holds_alternative<FormulaAST>(value);
    }

    FormulaAST get_value() const {
        return std::get<FormulaAST>(value);
    }

    Error get_error() const {
        return std::get<Error>(value);
    }
};

// Класс для парсинга формул
class CustomFormulaParser {
public:
    CustomFormulaParser();
    ~CustomFormulaParser();
    FormulaASTResult parse(const std::string& formula);

private:
};

#endif // FORMULAS_PARSER_FORMULA_PARSER_H