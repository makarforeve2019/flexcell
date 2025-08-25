
// Generated from newflexcell/ANTLR/Formula.g4 by ANTLR 4.10

#pragma once


#include "antlr4-runtime.h"




class  FormulaLexer : public antlr4::Lexer {
public:
  enum {
    NUMBER = 1, STRING = 2, CELL_REF = 3, RANGE = 4, TRUE = 5, FALSE = 6, 
    ERROR = 7, ID = 8, PLUS = 9, MINUS = 10, MULT = 11, DIV = 12, EQ = 13, 
    GT = 14, LT = 15, GTE = 16, LTE = 17, LPAREN = 18, RPAREN = 19, COMMA = 20, 
    WS = 21
  };

  explicit FormulaLexer(antlr4::CharStream *input);

  ~FormulaLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

