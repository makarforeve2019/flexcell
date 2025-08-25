
// Generated from newflexcell/ANTLR/Formula.g4 by ANTLR 4.10

#pragma once


#include "antlr4-runtime.h"
#include "FormulaListener.h"


/**
 * This class provides an empty implementation of FormulaListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  FormulaBaseListener : public FormulaListener {
public:

  virtual void enterFormula(FormulaParser::FormulaContext * /*ctx*/) override { }
  virtual void exitFormula(FormulaParser::FormulaContext * /*ctx*/) override { }

  virtual void enterErrorExpr(FormulaParser::ErrorExprContext * /*ctx*/) override { }
  virtual void exitErrorExpr(FormulaParser::ErrorExprContext * /*ctx*/) override { }

  virtual void enterFuncExpr(FormulaParser::FuncExprContext * /*ctx*/) override { }
  virtual void exitFuncExpr(FormulaParser::FuncExprContext * /*ctx*/) override { }

  virtual void enterGtExpr(FormulaParser::GtExprContext * /*ctx*/) override { }
  virtual void exitGtExpr(FormulaParser::GtExprContext * /*ctx*/) override { }

  virtual void enterTrueExpr(FormulaParser::TrueExprContext * /*ctx*/) override { }
  virtual void exitTrueExpr(FormulaParser::TrueExprContext * /*ctx*/) override { }

  virtual void enterNumberExpr(FormulaParser::NumberExprContext * /*ctx*/) override { }
  virtual void exitNumberExpr(FormulaParser::NumberExprContext * /*ctx*/) override { }

  virtual void enterGteExpr(FormulaParser::GteExprContext * /*ctx*/) override { }
  virtual void exitGteExpr(FormulaParser::GteExprContext * /*ctx*/) override { }

  virtual void enterMinusExpr(FormulaParser::MinusExprContext * /*ctx*/) override { }
  virtual void exitMinusExpr(FormulaParser::MinusExprContext * /*ctx*/) override { }

  virtual void enterMultExpr(FormulaParser::MultExprContext * /*ctx*/) override { }
  virtual void exitMultExpr(FormulaParser::MultExprContext * /*ctx*/) override { }

  virtual void enterParenExpr(FormulaParser::ParenExprContext * /*ctx*/) override { }
  virtual void exitParenExpr(FormulaParser::ParenExprContext * /*ctx*/) override { }

  virtual void enterPlusExpr(FormulaParser::PlusExprContext * /*ctx*/) override { }
  virtual void exitPlusExpr(FormulaParser::PlusExprContext * /*ctx*/) override { }

  virtual void enterStringExpr(FormulaParser::StringExprContext * /*ctx*/) override { }
  virtual void exitStringExpr(FormulaParser::StringExprContext * /*ctx*/) override { }

  virtual void enterLteExpr(FormulaParser::LteExprContext * /*ctx*/) override { }
  virtual void exitLteExpr(FormulaParser::LteExprContext * /*ctx*/) override { }

  virtual void enterEqExpr(FormulaParser::EqExprContext * /*ctx*/) override { }
  virtual void exitEqExpr(FormulaParser::EqExprContext * /*ctx*/) override { }

  virtual void enterLtExpr(FormulaParser::LtExprContext * /*ctx*/) override { }
  virtual void exitLtExpr(FormulaParser::LtExprContext * /*ctx*/) override { }

  virtual void enterCellRefExpr(FormulaParser::CellRefExprContext * /*ctx*/) override { }
  virtual void exitCellRefExpr(FormulaParser::CellRefExprContext * /*ctx*/) override { }

  virtual void enterDivExpr(FormulaParser::DivExprContext * /*ctx*/) override { }
  virtual void exitDivExpr(FormulaParser::DivExprContext * /*ctx*/) override { }

  virtual void enterFalseExpr(FormulaParser::FalseExprContext * /*ctx*/) override { }
  virtual void exitFalseExpr(FormulaParser::FalseExprContext * /*ctx*/) override { }

  virtual void enterRangeExpr(FormulaParser::RangeExprContext * /*ctx*/) override { }
  virtual void exitRangeExpr(FormulaParser::RangeExprContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

