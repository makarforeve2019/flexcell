#include "FormulaASTBuilder.h"
#include <memory>
#include <cstdlib>
#include <iostream>
#include <typeinfo>

std::any FormulaASTBuilder::visitFormula(FormulaParser::FormulaContext* ctx) {
    try {
        auto expr_result = ctx->expr()->accept(this);
        std::cerr << "[visitFormula] ctx->expr() type: " << expr_result.type().name() << "\n";

        auto node = std::any_cast<std::shared_ptr<ASTNode>>(expr_result);
        return std::make_shared<FormulaAST>(node);
    } catch (const std::bad_any_cast& e) {
        std::cerr << "[visitFormula] bad_any_cast: " << e.what() << "\n";
        throw;
    }
}

std::any FormulaASTBuilder::visitNumberExpr(FormulaParser::NumberExprContext* ctx) {
    double val = std::stod(ctx->NUMBER()->getText());
    auto node = std::make_shared<ConstantNode>(val);
    std::cerr << "[visitNumberExpr] parsed number: " << val << "\n";
    return std::static_pointer_cast<ASTNode>(node);
}

std::any FormulaASTBuilder::visitCellRefExpr(FormulaParser::CellRefExprContext* ctx) {
    std::string cellText = ctx->CELL_REF()->getText();
    char col = cellText[0]; // A-Z
    uint32_t colIndex = col - 'A' + 1;
    uint32_t rowIndex = std::stoi(cellText.substr(1));
    auto node = std::make_shared<CellRefNode>(CellRef{rowIndex, colIndex});
    std::cerr << "[visitCellRefExpr] parsed ref: " << cellText << " => row " << rowIndex << ", col " << colIndex << "\n";
    return std::static_pointer_cast<ASTNode>(node);
}

std::any FormulaASTBuilder::visitPlusExpr(FormulaParser::PlusExprContext* ctx) {
    try {
        auto lhs_raw = ctx->expr(0)->accept(this);
        auto rhs_raw = ctx->expr(1)->accept(this);
        std::cerr << "[visitPlusExpr] lhs type: " << lhs_raw.type().name() << ", rhs type: " << rhs_raw.type().name() << "\n";

        auto lhs = std::any_cast<std::shared_ptr<ASTNode>>(lhs_raw);
        auto rhs = std::any_cast<std::shared_ptr<ASTNode>>(rhs_raw);
        return std::static_pointer_cast<ASTNode>(
                std::make_shared<OperatorNode>(OperatorType::PLUS, lhs, rhs) );
    } catch (const std::bad_any_cast& e) {
        std::cerr << "[visitPlusExpr] bad_any_cast: " << e.what() << "\n";
        throw;
    }
}

std::any FormulaASTBuilder::visitMultExpr(FormulaParser::MultExprContext* ctx) {
    try {
        auto lhs_raw = ctx->expr(0)->accept(this);
        auto rhs_raw = ctx->expr(1)->accept(this);
        std::cerr << "[visitMultExpr] lhs type: " << lhs_raw.type().name() << ", rhs type: " << rhs_raw.type().name() << "\n";

        auto lhs = std::any_cast<std::shared_ptr<ASTNode>>(lhs_raw);
        auto rhs = std::any_cast<std::shared_ptr<ASTNode>>(rhs_raw);
        return std::static_pointer_cast<ASTNode>(
                std::make_shared<OperatorNode>(OperatorType::MULT, lhs, rhs) );
    } catch (const std::bad_any_cast& e) {
        std::cerr << "[visitMultExpr] bad_any_cast: " << e.what() << "\n";
        throw;
    }
}

std::any FormulaASTBuilder::visitDivExpr(FormulaParser::DivExprContext* ctx) {
    try {
        auto lhs_raw = ctx->expr(0)->accept(this);
        auto rhs_raw = ctx->expr(1)->accept(this);
        std::cerr << "[visitDivExpr] lhs type: " << lhs_raw.type().name() << ", rhs type: " << rhs_raw.type().name() << "\n";

        auto lhs = std::any_cast<std::shared_ptr<ASTNode>>(lhs_raw);
        auto rhs = std::any_cast<std::shared_ptr<ASTNode>>(rhs_raw);
        return std::static_pointer_cast<ASTNode>(
                std::make_shared<OperatorNode>(OperatorType::DIV, lhs, rhs) );
    } catch (const std::bad_any_cast& e) {
        std::cerr << "[visitDivExpr] bad_any_cast: " << e.what() << "\n";
        throw;
    }
}

std::any FormulaASTBuilder::visitParenExpr(FormulaParser::ParenExprContext* ctx) {
    auto result = ctx->expr()->accept(this);
    std::cerr << "[visitParenExpr] returning expr type: " << result.type().name() << "\n";
    return result;
}
