// Evaluator.cpp
#include "Evaluator.h"
#include "FormulaEngine.h"
#include <cmath>
#include <functional>
#include <unordered_map>

// Конструктор
Evaluator::Evaluator(const FormulaEngine& engine)
        : engine_(engine) {
}

// Вспомогательная функция для вычисления узла
static Result evaluateNode(const std::shared_ptr<ASTNode>& node, const Evaluator& eval) {
    if (!node) {
        return Result{Error{ErrorCode::VALUE, "Null node in AST"}};
    }

    if (auto* constNode = dynamic_cast<ConstantNode*>(node.get())) {
        return Result{constNode->value};
    }

    if (auto* cellNode = dynamic_cast<CellRefNode*>(node.get())) {
        auto result = eval.engine_.getCellValue(cellNode->ref);
        if (!result.is_success()) {
            return Result{result.get_error()};
        }
        return Result{result.get_value()};
    }

    if (auto* rangeNode = dynamic_cast<RangeNode*>(node.get())) {
        return Result{Error{ErrorCode::VALUE, "Range cannot be evaluated directly"}};
    }

    if (auto* opNode = dynamic_cast<OperatorNode*>(node.get())) {
        auto leftResult = evaluateNode(opNode->left, eval);
        if (!leftResult.is_success()) {
            return Result{leftResult.get_error()};
        }
        auto rightResult = evaluateNode(opNode->right, eval);
        if (!rightResult.is_success()) {
            return Result{rightResult.get_error()};
        }

        if (!std::holds_alternative<double>(leftResult.get_value()) ||
            !std::holds_alternative<double>(rightResult.get_value())) {
            return Result{Error{ErrorCode::VALUE, "Operator requires numeric operands"}};
        }

        double left = std::get<double>(leftResult.get_value());
        double right = std::get<double>(rightResult.get_value());

        switch (opNode->type) {
            case OperatorType::PLUS: return Result{left + right};
            case OperatorType::MINUS: return Result{left - right};
            case OperatorType::MULT: return Result{left * right};
            case OperatorType::DIV:
                if (right == 0.0) {
                    return Result{Error{ErrorCode::DIV0, "Division by zero"}};
                }
                return Result{left / right};
            case OperatorType::GT: return Result{left > right};
            case OperatorType::LT: return Result{left < right};
            case OperatorType::EQ: return Result{left == right};
            case OperatorType::GTE: return Result{left >= right};
            case OperatorType::LTE: return Result{left <= right};
            default:
                return Result{Error{ErrorCode::VALUE, "Unknown operator"}};
        }
    }

    if (auto* funcNode = dynamic_cast<FunctionNode*>(node.get())) {
        static const std::unordered_map<std::string, std::function<Result(const std::vector<std::shared_ptr<ASTNode>>&, const Evaluator&)>> functions = {
                {"SUM", [](const auto& args, const auto& eval) -> Result {
                    double sum = 0.0;
                    for (const auto& arg : args) {
                        auto result = evaluateNode(arg, eval);
                        if (!result.is_success()) {
                            return Result{result.get_error()};
                        }
                        if (std::holds_alternative<double>(result.get_value())) {
                            sum += std::get<double>(result.get_value());
                        } else {
                            return Result{Error{ErrorCode::VALUE, "SUM requires numeric arguments"}};
                        }
                    }
                    return Result{sum};
                }},
                {"IF", [](const auto& args, const auto& eval) -> Result {
                    if (args.size() != 3) {
                        return Result{Error{ErrorCode::VALUE, "IF requires exactly 3 arguments"}};
                    }
                    auto condResult = evaluateNode(args[0], eval);
                    if (!condResult.is_success()) {
                        return Result{condResult.get_error()};
                    }
                    if (!std::holds_alternative<bool>(condResult.get_value())) {
                        return Result{Error{ErrorCode::VALUE, "IF condition must be boolean"}};
                    }
                    bool condition = std::get<bool>(condResult.get_value());
                    return evaluateNode(condition ? args[1] : args[2], eval);
                }},
                {"ROUND", [](const auto& args, const auto& eval) -> Result {
                    if (args.size() != 2) {
                        return Result{Error{ErrorCode::VALUE, "ROUND requires exactly 2 arguments"}};
                    }
                    auto valueResult = evaluateNode(args[0], eval);
                    auto digitsResult = evaluateNode(args[1], eval);
                    if (!valueResult.is_success() || !digitsResult.is_success()) {
                        return Result{valueResult.is_success() ? digitsResult.get_error() : valueResult.get_error()};
                    }
                    if (!std::holds_alternative<double>(valueResult.get_value()) ||
                        !std::holds_alternative<double>(digitsResult.get_value())) {
                        return Result{Error{ErrorCode::VALUE, "ROUND requires numeric arguments"}};
                    }
                    double value = std::get<double>(valueResult.get_value());
                    double digits = std::get<double>(digitsResult.get_value());
                    return Result{std::round(value * std::pow(10.0, digits)) / std::pow(10.0, digits)};
                }},
                {"OFFSET", [](const auto& args, const auto& eval) -> Result {
                    if (args.size() < 3 || args.size() > 5) {
                        return Result{Error{ErrorCode::VALUE, "OFFSET requires 3 to 5 arguments"}};
                    }
                    if (auto* refNode = dynamic_cast<CellRefNode*>(args[0].get())) {
                        CellRef ref = refNode->ref;
                        auto rowOffsetResult = evaluateNode(args[1], eval);
                        auto colOffsetResult = evaluateNode(args[2], eval);
                        if (!rowOffsetResult.is_success() || !colOffsetResult.is_success()) {
                            return Result{rowOffsetResult.is_success() ? colOffsetResult.get_error() : rowOffsetResult.get_error()};
                        }
                        if (!std::holds_alternative<double>(rowOffsetResult.get_value()) ||
                            !std::holds_alternative<double>(colOffsetResult.get_value())) {
                            return Result{Error{ErrorCode::VALUE, "OFFSET offsets must be numeric"}};
                        }
                        int rowOffset = static_cast<int>(std::get<double>(rowOffsetResult.get_value()));
                        int colOffset = static_cast<int>(std::get<double>(colOffsetResult.get_value()));
                        CellRef newRef{static_cast<uint32_t>(static_cast<int>(ref.row) + rowOffset),
                                       static_cast<uint32_t>(static_cast<int>(ref.col) + colOffset)};
                        if (newRef.row < 1 || newRef.col < 1) {
                            return Result{Error{ErrorCode::NA, "OFFSET resulted in invalid cell reference"}};
                        }
                        return eval.engine_.getCellValue(newRef);
                    }
                    return Result{Error{ErrorCode::VALUE, "OFFSET first argument must be a cell reference"}};
                }}
        };

        auto it = functions.find(funcNode->name);
        if (it == functions.end()) {
            return Result{Error{ErrorCode::NAME, "Unknown function: " + funcNode->name}};
        }
        return it->second(funcNode->args, eval);
    }

    return Result{Error{ErrorCode::VALUE, "Unknown node type"}};
}

// Вычисление формулы
Result Evaluator::evaluate(const FormulaAST& ast) const {
    return evaluateNode(ast.root_, *this);
}