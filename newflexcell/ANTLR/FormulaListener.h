
// Generated from newflexcell/ANTLR/Formula.g4 by ANTLR 4.10

#pragma once


#include "antlr4-runtime.h"
#include "FormulaParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by FormulaParser.
 */
class  FormulaListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterFormula(FormulaParser::FormulaContext *ctx) = 0;
  virtual void exitFormula(FormulaParser::FormulaContext *ctx) = 0;

  virtual void enterErrorExpr(FormulaParser::ErrorExprContext *ctx) = 0;
  virtual void exitErrorExpr(FormulaParser::ErrorExprContext *ctx) = 0;

  virtual void enterFuncExpr(FormulaParser::FuncExprContext *ctx) = 0;
  virtual void exitFuncExpr(FormulaParser::FuncExprContext *ctx) = 0;

  virtual void enterGtExpr(FormulaParser::GtExprContext *ctx) = 0;
  virtual void exitGtExpr(FormulaParser::GtExprContext *ctx) = 0;

  virtual void enterTrueExpr(FormulaParser::TrueExprContext *ctx) = 0;
  virtual void exitTrueExpr(FormulaParser::TrueExprContext *ctx) = 0;

  virtual void enterNumberExpr(FormulaParser::NumberExprContext *ctx) = 0;
  virtual void exitNumberExpr(FormulaParser::NumberExprContext *ctx) = 0;

  virtual void enterGteExpr(FormulaParser::GteExprContext *ctx) = 0;
  virtual void exitGteExpr(FormulaParser::GteExprContext *ctx) = 0;

  virtual void enterMinusExpr(FormulaParser::MinusExprContext *ctx) = 0;
  virtual void exitMinusExpr(FormulaParser::MinusExprContext *ctx) = 0;

  virtual void enterMultExpr(FormulaParser::MultExprContext *ctx) = 0;
  virtual void exitMultExpr(FormulaParser::MultExprContext *ctx) = 0;

  virtual void enterParenExpr(FormulaParser::ParenExprContext *ctx) = 0;
  virtual void exitParenExpr(FormulaParser::ParenExprContext *ctx) = 0;

  virtual void enterPlusExpr(FormulaParser::PlusExprContext *ctx) = 0;
  virtual void exitPlusExpr(FormulaParser::PlusExprContext *ctx) = 0;

  virtual void enterStringExpr(FormulaParser::StringExprContext *ctx) = 0;
  virtual void exitStringExpr(FormulaParser::StringExprContext *ctx) = 0;

  virtual void enterLteExpr(FormulaParser::LteExprContext *ctx) = 0;
  virtual void exitLteExpr(FormulaParser::LteExprContext *ctx) = 0;

  virtual void enterEqExpr(FormulaParser::EqExprContext *ctx) = 0;
  virtual void exitEqExpr(FormulaParser::EqExprContext *ctx) = 0;

  virtual void enterLtExpr(FormulaParser::LtExprContext *ctx) = 0;
  virtual void exitLtExpr(FormulaParser::LtExprContext *ctx) = 0;

  virtual void enterCellRefExpr(FormulaParser::CellRefExprContext *ctx) = 0;
  virtual void exitCellRefExpr(FormulaParser::CellRefExprContext *ctx) = 0;

  virtual void enterDivExpr(FormulaParser::DivExprContext *ctx) = 0;
  virtual void exitDivExpr(FormulaParser::DivExprContext *ctx) = 0;

  virtual void enterFalseExpr(FormulaParser::FalseExprContext *ctx) = 0;
  virtual void exitFalseExpr(FormulaParser::FalseExprContext *ctx) = 0;

  virtual void enterRangeExpr(FormulaParser::RangeExprContext *ctx) = 0;
  virtual void exitRangeExpr(FormulaParser::RangeExprContext *ctx) = 0;


};

