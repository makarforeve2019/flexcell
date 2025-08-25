
// Generated from newflexcell/ANTLR/Formula.g4 by ANTLR 4.10

#pragma once


#include "antlr4-runtime.h"
#include "FormulaVisitor.h"


/**
 * This class provides an empty implementation of FormulaVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  FormulaBaseVisitor : public FormulaVisitor {
public:

  virtual std::any visitFormula(FormulaParser::FormulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitErrorExpr(FormulaParser::ErrorExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncExpr(FormulaParser::FuncExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGtExpr(FormulaParser::GtExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTrueExpr(FormulaParser::TrueExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumberExpr(FormulaParser::NumberExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGteExpr(FormulaParser::GteExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMinusExpr(FormulaParser::MinusExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultExpr(FormulaParser::MultExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParenExpr(FormulaParser::ParenExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPlusExpr(FormulaParser::PlusExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStringExpr(FormulaParser::StringExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLteExpr(FormulaParser::LteExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqExpr(FormulaParser::EqExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLtExpr(FormulaParser::LtExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCellRefExpr(FormulaParser::CellRefExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDivExpr(FormulaParser::DivExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFalseExpr(FormulaParser::FalseExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRangeExpr(FormulaParser::RangeExprContext *ctx) override {
    return visitChildren(ctx);
  }


};

