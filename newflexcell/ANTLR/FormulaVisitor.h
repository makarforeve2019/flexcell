
// Generated from newflexcell/ANTLR/Formula.g4 by ANTLR 4.10

#pragma once


#include "antlr4-runtime.h"
#include "FormulaParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by FormulaParser.
 */
class  FormulaVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by FormulaParser.
   */
    virtual std::any visitFormula(FormulaParser::FormulaContext *context) = 0;

    virtual std::any visitErrorExpr(FormulaParser::ErrorExprContext *context) = 0;

    virtual std::any visitFuncExpr(FormulaParser::FuncExprContext *context) = 0;

    virtual std::any visitGtExpr(FormulaParser::GtExprContext *context) = 0;

    virtual std::any visitTrueExpr(FormulaParser::TrueExprContext *context) = 0;

    virtual std::any visitNumberExpr(FormulaParser::NumberExprContext *context) = 0;

    virtual std::any visitGteExpr(FormulaParser::GteExprContext *context) = 0;

    virtual std::any visitMinusExpr(FormulaParser::MinusExprContext *context) = 0;

    virtual std::any visitMultExpr(FormulaParser::MultExprContext *context) = 0;

    virtual std::any visitParenExpr(FormulaParser::ParenExprContext *context) = 0;

    virtual std::any visitPlusExpr(FormulaParser::PlusExprContext *context) = 0;

    virtual std::any visitStringExpr(FormulaParser::StringExprContext *context) = 0;

    virtual std::any visitLteExpr(FormulaParser::LteExprContext *context) = 0;

    virtual std::any visitEqExpr(FormulaParser::EqExprContext *context) = 0;

    virtual std::any visitLtExpr(FormulaParser::LtExprContext *context) = 0;

    virtual std::any visitCellRefExpr(FormulaParser::CellRefExprContext *context) = 0;

    virtual std::any visitDivExpr(FormulaParser::DivExprContext *context) = 0;

    virtual std::any visitFalseExpr(FormulaParser::FalseExprContext *context) = 0;

    virtual std::any visitRangeExpr(FormulaParser::RangeExprContext *context) = 0;


};

