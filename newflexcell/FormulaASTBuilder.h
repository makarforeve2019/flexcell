#ifndef FORMULA_AST_BUILDER_H
#define FORMULA_AST_BUILDER_H

#include "ANTLR/FormulaBaseVisitor.h"
#include "FormulaAST.h"

class FormulaASTBuilder : public FormulaBaseVisitor {
public:
    std::any visitFormula(FormulaParser::FormulaContext* ctx) override;

    std::any visitNumberExpr(FormulaParser::NumberExprContext* ctx) override;
    std::any visitCellRefExpr(FormulaParser::CellRefExprContext* ctx) override;
    std::any visitPlusExpr(FormulaParser::PlusExprContext* ctx) override;
    std::any visitMultExpr(FormulaParser::MultExprContext* ctx) override;
    std::any visitParenExpr(FormulaParser::ParenExprContext* ctx) override;
    std::any visitDivExpr(FormulaParser::DivExprContext* ctx) override;

    // добавим остальные по мере необходимости
};

#endif // FORMULA_AST_BUILDER_H
