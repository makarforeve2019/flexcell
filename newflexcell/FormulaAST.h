// FormulaAST.h
#ifndef FORMULAS_PARSER_FORMULA_AST_H
#define FORMULAS_PARSER_FORMULA_AST_H

#include "Error.h"
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <ostream>

// Структура для адреса ячейки
struct CellRef {
    uint32_t row;
    uint32_t col;

    bool operator==(const CellRef& other) const {
        return row == other.row && col == other.col;
    }

    bool operator<(const CellRef& other) const {
        return row < other.row || (row == other.row && col < other.col);
    }
};

// Структура для диапазона
struct RangeRef {
    CellRef start;
    CellRef end;

    bool operator==(const RangeRef& other) const {
        return start == other.start && end == other.end;
    }

    bool operator<(const RangeRef& other) const {
        return start < other.start || (start == other.start && end < other.end);
    }
};

// Специализация std::hash
namespace std {
    template<>
    struct hash<CellRef> {
        std::size_t operator()(const CellRef& ref) const {
            return (static_cast<std::size_t>(ref.row) << 32) ^ ref.col;
        }
    };

    template<>
    struct hash<RangeRef> {
        std::size_t operator()(const RangeRef& range) const {
            std::size_t h1 = std::hash<CellRef>{}(range.start);
            std::size_t h2 = std::hash<CellRef>{}(range.end);
            return h1 ^ (h2 << 1);
        }
    };
}

// Перечисление типов операторов
enum class OperatorType {
    PLUS, MINUS, MULT, DIV, GT, LT, EQ, GTE, LTE
};

// Базовый класс для узлов AST
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void Print(std::ostream& out, int indent = 0) const = 0;
};

// Узел для констант
class ConstantNode : public ASTNode {
public:
    Value value;
    ConstantNode(Value val) : value(val) {}
    void Print(std::ostream& out, int indent = 0) const override; // Добавил объявление
};

// Узел для ячейки
class CellRefNode : public ASTNode {
public:
    CellRef ref;
    CellRefNode(CellRef r) : ref(r) {}
    void Print(std::ostream& out, int indent = 0) const override; // Добавил объявление
};

// Узел для диапазона
class RangeNode : public ASTNode {
public:
    RangeRef range;
    RangeNode(RangeRef r) : range(r) {}
    void Print(std::ostream& out, int indent = 0) const override; // Добавил объявление
};

// Узел для оператора
class OperatorNode : public ASTNode {
public:
    OperatorType type;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    OperatorNode(OperatorType t, std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r)
            : type(t), left(l), right(r) {}
    void Print(std::ostream& out, int indent = 0) const override; // Добавил объявление
};

// Узел для функции
class FunctionNode : public ASTNode {
public:
    std::string name;
    std::vector<std::shared_ptr<ASTNode>> args;
    FunctionNode(std::string n, std::vector<std::shared_ptr<ASTNode>> a)
            : name(n), args(a) {}
    void Print(std::ostream& out, int indent = 0) const override; // Добавил объявление
};

// Класс AST
class FormulaAST {
public:
    FormulaAST(std::shared_ptr<ASTNode> root);
    std::vector<CellRef> getCellDependencies() const;
    std::vector<RangeRef> getRangeDependencies() const;
    Result evaluate(class Evaluator& eval) const;
    std::string toString() const;

    // Я переношу реализацию сюда, чтобы решить ошибку линковки.
    // Метод, определённый внутри класса, автоматически является inline.
    void Print(std::ostream& out, int indent = 0) const {
        if (root_) {
            root_->Print(out, indent);
        } else {
            out << std::string(indent, ' ') << "[Empty AST]\n";
        }
    }

private:
    friend class Evaluator;
    std::shared_ptr<ASTNode> root_;
};

#endif // FORMULAS_PARSER_FORMULA_AST_H