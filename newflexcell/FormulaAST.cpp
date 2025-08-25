// FormulaAST.cpp
#include "FormulaAST.h"
#include "Evaluator.h"
#include <algorithm>
#include <sstream>

// Конструктор
FormulaAST::FormulaAST(std::shared_ptr<ASTNode> root)
        : root_(std::move(root)) {
}

// Сбор зависимостей ячеек
static void collectCellDependencies(const std::shared_ptr<ASTNode>& node, std::vector<CellRef>& result) {
    if (!node) return;
    if (auto* cellNode = dynamic_cast<CellRefNode*>(node.get())) {
        result.push_back(cellNode->ref);
    }
    else if (auto* opNode = dynamic_cast<OperatorNode*>(node.get())) {
        collectCellDependencies(opNode->left, result);
        collectCellDependencies(opNode->right, result);
    }
    else if (auto* funcNode = dynamic_cast<FunctionNode*>(node.get())) {
        for (const auto& arg : funcNode->args) {
            collectCellDependencies(arg, result);
        }
    }
}

std::vector<CellRef> FormulaAST::getCellDependencies() const {
    std::vector<CellRef> result;
    collectCellDependencies(root_, result);
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

// Сбор диапазонов
static void collectRangeDependencies(const std::shared_ptr<ASTNode>& node, std::vector<RangeRef>& result) {
    if (!node) return;
    if (auto* rangeNode = dynamic_cast<RangeNode*>(node.get())) {
        result.push_back(rangeNode->range);
    }
    else if (auto* opNode = dynamic_cast<OperatorNode*>(node.get())) {
        collectRangeDependencies(opNode->left, result);
        collectRangeDependencies(opNode->right, result);
    }
    else if (auto* funcNode = dynamic_cast<FunctionNode*>(node.get())) {
        for (const auto& arg : funcNode->args) {
            collectRangeDependencies(arg, result);
        }
    }
}

std::vector<RangeRef> FormulaAST::getRangeDependencies() const {
    std::vector<RangeRef> result;
    collectRangeDependencies(root_, result);
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

// Вычисление формулы
Result FormulaAST::evaluate(Evaluator& eval) const {
    if (!root_) {
        return Result{Error{ErrorCode::VALUE, "Empty AST"}};
    }
    return eval.evaluate(*this);
}

static std::string nodeToString(const std::shared_ptr<ASTNode>& node) {
    if (auto c = std::dynamic_pointer_cast<ConstantNode>(node)) {
        if (std::holds_alternative<double>(c->value))
            return std::to_string(std::get<double>(c->value));
        if (std::holds_alternative<std::string>(c->value))
            return "\"" + std::get<std::string>(c->value) + "\"";
        if (std::holds_alternative<bool>(c->value))
            return std::get<bool>(c->value) ? "TRUE" : "FALSE";
    }

    if (auto ref = std::dynamic_pointer_cast<CellRefNode>(node)) {
        char col = 'A' + ref->ref.col - 1;
        return std::string(1, col) + std::to_string(ref->ref.row);
    }

    if (auto range = std::dynamic_pointer_cast<RangeNode>(node)) {
        char col1 = 'A' + range->range.start.col - 1;
        char col2 = 'A' + range->range.end.col - 1;
        return std::string(1, col1) + std::to_string(range->range.start.row) +
               ":" + std::string(1, col2) + std::to_string(range->range.end.row);
    }

    if (auto op = std::dynamic_pointer_cast<OperatorNode>(node)) {
        std::string opStr;
        switch (op->type) {
            case OperatorType::PLUS:  opStr = "+"; break;
            case OperatorType::MINUS: opStr = "-"; break;
            case OperatorType::MULT:  opStr = "*"; break;
            case OperatorType::DIV:   opStr = "/"; break;
            case OperatorType::GT:    opStr = ">"; break;
            case OperatorType::LT:    opStr = "<"; break;
            case OperatorType::EQ:    opStr = "="; break;
            case OperatorType::GTE:   opStr = ">="; break;
            case OperatorType::LTE:   opStr = "<="; break;
        }
        return "(" + nodeToString(op->left) + " " + opStr + " " + nodeToString(op->right) + ")";
    }

    if (auto f = std::dynamic_pointer_cast<FunctionNode>(node)) {
        std::ostringstream oss;
        oss << f->name << "(";
        for (size_t i = 0; i < f->args.size(); ++i) {
            oss << nodeToString(f->args[i]);
            if (i + 1 < f->args.size())
                oss << ", ";
        }
        oss << ")";
        return oss.str();
    }

    return "[UnknownNode]";
}

std::string FormulaAST::toString() const {
    return nodeToString(root_);
}

void ConstantNode::Print(std::ostream& out, int indent) const {
    out << std::string(indent, ' ') << "ConstantNode: ";
    std::visit([&out](const auto& val) { out << val; }, value);
    out << "\n";
}

void CellRefNode::Print(std::ostream& out, int indent) const {
    out << std::string(indent, ' ') << "CellRefNode: "
        << static_cast<char>('A' + ref.col - 1) << ref.row << "\n";
}

void RangeNode::Print(std::ostream& out, int indent) const {
    out << std::string(indent, ' ') << "RangeNode: "
        << static_cast<char>('A' + range.start.col - 1) << range.start.row << ":"
        << static_cast<char>('A' + range.end.col - 1) << range.end.row << "\n";
}

void OperatorNode::Print(std::ostream& out, int indent) const {
    out << std::string(indent, ' ') << "OperatorNode: ";
    switch (type) {
        case OperatorType::PLUS: out << "+"; break;
        case OperatorType::MINUS: out << "-"; break;
        case OperatorType::MULT: out << "*"; break;
        case OperatorType::DIV: out << "/"; break;
            // TODO: Добавить остальные операторы
        default: out << "?"; break;
    }
    out << "\n";
    left->Print(out, indent + 2);
    right->Print(out, indent + 2);
}

void FunctionNode::Print(std::ostream& out, int indent) const {
    out << std::string(indent, ' ') << "FunctionNode: " << name << "\n";
    for (const auto& arg : args) {
        arg->Print(out, indent + 2);
    }
}
