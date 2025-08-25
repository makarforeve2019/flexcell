// Cell.h
#ifndef FORMULAS_PARSER_CELL_H
#define FORMULAS_PARSER_CELL_H

#include "Error.h"
#include "FormulaAST.h"
#include <memory>
#include <optional>
#include <variant>

struct Cell {
    using Value = std::variant<double, std::string, bool>;

    Value value;
    std::shared_ptr<FormulaAST> formula;

    std::optional<std::variant<Value, Error>> cached_value;

    Cell() : value(0.0), formula(nullptr), cached_value(std::nullopt) {} // Конструктор по умолчанию
    Cell(Value val) : value(val), formula(nullptr), cached_value(std::nullopt) {}
    Cell(std::shared_ptr<FormulaAST> f) : value(Value{}), formula(f), cached_value(std::nullopt) {}
};

#endif // FORMULAS_PARSER_CELL_H