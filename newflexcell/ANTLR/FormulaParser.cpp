
// Generated from newflexcell/ANTLR/Formula.g4 by ANTLR 4.10


#include "FormulaListener.h"
#include "FormulaVisitor.h"

#include "FormulaParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct FormulaParserStaticData final {
  FormulaParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  FormulaParserStaticData(const FormulaParserStaticData&) = delete;
  FormulaParserStaticData(FormulaParserStaticData&&) = delete;
  FormulaParserStaticData& operator=(const FormulaParserStaticData&) = delete;
  FormulaParserStaticData& operator=(FormulaParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag formulaParserOnceFlag;
FormulaParserStaticData *formulaParserStaticData = nullptr;

void formulaParserInitialize() {
  assert(formulaParserStaticData == nullptr);
  auto staticData = std::make_unique<FormulaParserStaticData>(
    std::vector<std::string>{
      "formula", "expr"
    },
    std::vector<std::string>{
      "", "", "", "", "", "'TRUE'", "'FALSE'", "", "", "'+'", "'-'", "'*'", 
      "'/'", "'='", "'>'", "'<'", "'>='", "'<='", "'('", "')'", "','"
    },
    std::vector<std::string>{
      "", "NUMBER", "STRING", "CELL_REF", "RANGE", "TRUE", "FALSE", "ERROR", 
      "ID", "PLUS", "MINUS", "MULT", "DIV", "EQ", "GT", "LT", "GTE", "LTE", 
      "LPAREN", "RPAREN", "COMMA", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,21,67,2,0,7,0,2,1,7,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,5,1,
  	15,8,1,10,1,12,1,18,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,1,1,3,1,33,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,62,8,1,10,
  	1,12,1,65,9,1,1,1,0,1,2,2,0,2,0,0,82,0,4,1,0,0,0,2,32,1,0,0,0,4,5,5,13,
  	0,0,5,6,3,2,1,0,6,7,5,0,0,1,7,1,1,0,0,0,8,9,6,1,-1,0,9,10,5,8,0,0,10,
  	11,5,18,0,0,11,16,3,2,1,0,12,13,5,20,0,0,13,15,3,2,1,0,14,12,1,0,0,0,
  	15,18,1,0,0,0,16,14,1,0,0,0,16,17,1,0,0,0,17,19,1,0,0,0,18,16,1,0,0,0,
  	19,20,5,19,0,0,20,33,1,0,0,0,21,33,5,3,0,0,22,33,5,4,0,0,23,33,5,1,0,
  	0,24,33,5,2,0,0,25,33,5,5,0,0,26,33,5,6,0,0,27,33,5,7,0,0,28,29,5,18,
  	0,0,29,30,3,2,1,0,30,31,5,19,0,0,31,33,1,0,0,0,32,8,1,0,0,0,32,21,1,0,
  	0,0,32,22,1,0,0,0,32,23,1,0,0,0,32,24,1,0,0,0,32,25,1,0,0,0,32,26,1,0,
  	0,0,32,27,1,0,0,0,32,28,1,0,0,0,33,63,1,0,0,0,34,35,10,18,0,0,35,36,5,
  	13,0,0,36,62,3,2,1,18,37,38,10,17,0,0,38,39,5,16,0,0,39,62,3,2,1,18,40,
  	41,10,16,0,0,41,42,5,17,0,0,42,62,3,2,1,17,43,44,10,15,0,0,44,45,5,14,
  	0,0,45,62,3,2,1,16,46,47,10,14,0,0,47,48,5,15,0,0,48,62,3,2,1,15,49,50,
  	10,13,0,0,50,51,5,9,0,0,51,62,3,2,1,14,52,53,10,12,0,0,53,54,5,10,0,0,
  	54,62,3,2,1,13,55,56,10,11,0,0,56,57,5,11,0,0,57,62,3,2,1,12,58,59,10,
  	10,0,0,59,60,5,12,0,0,60,62,3,2,1,11,61,34,1,0,0,0,61,37,1,0,0,0,61,40,
  	1,0,0,0,61,43,1,0,0,0,61,46,1,0,0,0,61,49,1,0,0,0,61,52,1,0,0,0,61,55,
  	1,0,0,0,61,58,1,0,0,0,62,65,1,0,0,0,63,61,1,0,0,0,63,64,1,0,0,0,64,3,
  	1,0,0,0,65,63,1,0,0,0,4,16,32,61,63
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  formulaParserStaticData = staticData.release();
}

}

FormulaParser::FormulaParser(TokenStream *input) : FormulaParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

FormulaParser::FormulaParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  FormulaParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *formulaParserStaticData->atn, formulaParserStaticData->decisionToDFA, formulaParserStaticData->sharedContextCache, options);
}

FormulaParser::~FormulaParser() {
  delete _interpreter;
}

const atn::ATN& FormulaParser::getATN() const {
  return *formulaParserStaticData->atn;
}

std::string FormulaParser::getGrammarFileName() const {
  return "Formula.g4";
}

const std::vector<std::string>& FormulaParser::getRuleNames() const {
  return formulaParserStaticData->ruleNames;
}

const dfa::Vocabulary& FormulaParser::getVocabulary() const {
  return formulaParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView FormulaParser::getSerializedATN() const {
  return formulaParserStaticData->serializedATN;
}


//----------------- FormulaContext ------------------------------------------------------------------

FormulaParser::FormulaContext::FormulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaParser::FormulaContext::EQ() {
  return getToken(FormulaParser::EQ, 0);
}

FormulaParser::ExprContext* FormulaParser::FormulaContext::expr() {
  return getRuleContext<FormulaParser::ExprContext>(0);
}

tree::TerminalNode* FormulaParser::FormulaContext::EOF() {
  return getToken(FormulaParser::EOF, 0);
}


size_t FormulaParser::FormulaContext::getRuleIndex() const {
  return FormulaParser::RuleFormula;
}

void FormulaParser::FormulaContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFormula(this);
}

void FormulaParser::FormulaContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFormula(this);
}


std::any FormulaParser::FormulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitFormula(this);
  else
    return visitor->visitChildren(this);
}

FormulaParser::FormulaContext* FormulaParser::formula() {
  FormulaContext *_localctx = _tracker.createInstance<FormulaContext>(_ctx, getState());
  enterRule(_localctx, 0, FormulaParser::RuleFormula);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(4);
    match(FormulaParser::EQ);
    setState(5);
    expr(0);
    setState(6);
    match(FormulaParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

FormulaParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t FormulaParser::ExprContext::getRuleIndex() const {
  return FormulaParser::RuleExpr;
}

void FormulaParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ErrorExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::ErrorExprContext::ERROR() {
  return getToken(FormulaParser::ERROR, 0);
}

FormulaParser::ErrorExprContext::ErrorExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::ErrorExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterErrorExpr(this);
}
void FormulaParser::ErrorExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitErrorExpr(this);
}

std::any FormulaParser::ErrorExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitErrorExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FuncExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::FuncExprContext::ID() {
  return getToken(FormulaParser::ID, 0);
}

tree::TerminalNode* FormulaParser::FuncExprContext::LPAREN() {
  return getToken(FormulaParser::LPAREN, 0);
}

std::vector<FormulaParser::ExprContext *> FormulaParser::FuncExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::FuncExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::FuncExprContext::RPAREN() {
  return getToken(FormulaParser::RPAREN, 0);
}

std::vector<tree::TerminalNode *> FormulaParser::FuncExprContext::COMMA() {
  return getTokens(FormulaParser::COMMA);
}

tree::TerminalNode* FormulaParser::FuncExprContext::COMMA(size_t i) {
  return getToken(FormulaParser::COMMA, i);
}

FormulaParser::FuncExprContext::FuncExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::FuncExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncExpr(this);
}
void FormulaParser::FuncExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncExpr(this);
}

std::any FormulaParser::FuncExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitFuncExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- GtExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::GtExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::GtExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::GtExprContext::GT() {
  return getToken(FormulaParser::GT, 0);
}

FormulaParser::GtExprContext::GtExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::GtExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGtExpr(this);
}
void FormulaParser::GtExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGtExpr(this);
}

std::any FormulaParser::GtExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitGtExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TrueExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::TrueExprContext::TRUE() {
  return getToken(FormulaParser::TRUE, 0);
}

FormulaParser::TrueExprContext::TrueExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::TrueExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTrueExpr(this);
}
void FormulaParser::TrueExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTrueExpr(this);
}

std::any FormulaParser::TrueExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitTrueExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NumberExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::NumberExprContext::NUMBER() {
  return getToken(FormulaParser::NUMBER, 0);
}

FormulaParser::NumberExprContext::NumberExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::NumberExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumberExpr(this);
}
void FormulaParser::NumberExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumberExpr(this);
}

std::any FormulaParser::NumberExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitNumberExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- GteExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::GteExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::GteExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::GteExprContext::GTE() {
  return getToken(FormulaParser::GTE, 0);
}

FormulaParser::GteExprContext::GteExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::GteExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGteExpr(this);
}
void FormulaParser::GteExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGteExpr(this);
}

std::any FormulaParser::GteExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitGteExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MinusExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::MinusExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::MinusExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::MinusExprContext::MINUS() {
  return getToken(FormulaParser::MINUS, 0);
}

FormulaParser::MinusExprContext::MinusExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::MinusExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMinusExpr(this);
}
void FormulaParser::MinusExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMinusExpr(this);
}

std::any FormulaParser::MinusExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitMinusExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MultExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::MultExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::MultExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::MultExprContext::MULT() {
  return getToken(FormulaParser::MULT, 0);
}

FormulaParser::MultExprContext::MultExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::MultExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMultExpr(this);
}
void FormulaParser::MultExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMultExpr(this);
}

std::any FormulaParser::MultExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitMultExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::ParenExprContext::LPAREN() {
  return getToken(FormulaParser::LPAREN, 0);
}

FormulaParser::ExprContext* FormulaParser::ParenExprContext::expr() {
  return getRuleContext<FormulaParser::ExprContext>(0);
}

tree::TerminalNode* FormulaParser::ParenExprContext::RPAREN() {
  return getToken(FormulaParser::RPAREN, 0);
}

FormulaParser::ParenExprContext::ParenExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::ParenExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParenExpr(this);
}
void FormulaParser::ParenExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParenExpr(this);
}

std::any FormulaParser::ParenExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitParenExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PlusExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::PlusExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::PlusExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::PlusExprContext::PLUS() {
  return getToken(FormulaParser::PLUS, 0);
}

FormulaParser::PlusExprContext::PlusExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::PlusExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPlusExpr(this);
}
void FormulaParser::PlusExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPlusExpr(this);
}

std::any FormulaParser::PlusExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitPlusExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- StringExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::StringExprContext::STRING() {
  return getToken(FormulaParser::STRING, 0);
}

FormulaParser::StringExprContext::StringExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::StringExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStringExpr(this);
}
void FormulaParser::StringExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStringExpr(this);
}

std::any FormulaParser::StringExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitStringExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LteExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::LteExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::LteExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::LteExprContext::LTE() {
  return getToken(FormulaParser::LTE, 0);
}

FormulaParser::LteExprContext::LteExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::LteExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLteExpr(this);
}
void FormulaParser::LteExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLteExpr(this);
}

std::any FormulaParser::LteExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitLteExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EqExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::EqExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::EqExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::EqExprContext::EQ() {
  return getToken(FormulaParser::EQ, 0);
}

FormulaParser::EqExprContext::EqExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::EqExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEqExpr(this);
}
void FormulaParser::EqExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEqExpr(this);
}

std::any FormulaParser::EqExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitEqExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LtExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::LtExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::LtExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::LtExprContext::LT() {
  return getToken(FormulaParser::LT, 0);
}

FormulaParser::LtExprContext::LtExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::LtExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLtExpr(this);
}
void FormulaParser::LtExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLtExpr(this);
}

std::any FormulaParser::LtExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitLtExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CellRefExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::CellRefExprContext::CELL_REF() {
  return getToken(FormulaParser::CELL_REF, 0);
}

FormulaParser::CellRefExprContext::CellRefExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::CellRefExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCellRefExpr(this);
}
void FormulaParser::CellRefExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCellRefExpr(this);
}

std::any FormulaParser::CellRefExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitCellRefExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DivExprContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::DivExprContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::DivExprContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::DivExprContext::DIV() {
  return getToken(FormulaParser::DIV, 0);
}

FormulaParser::DivExprContext::DivExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::DivExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDivExpr(this);
}
void FormulaParser::DivExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDivExpr(this);
}

std::any FormulaParser::DivExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitDivExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FalseExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::FalseExprContext::FALSE() {
  return getToken(FormulaParser::FALSE, 0);
}

FormulaParser::FalseExprContext::FalseExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::FalseExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFalseExpr(this);
}
void FormulaParser::FalseExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFalseExpr(this);
}

std::any FormulaParser::FalseExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitFalseExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RangeExprContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::RangeExprContext::RANGE() {
  return getToken(FormulaParser::RANGE, 0);
}

FormulaParser::RangeExprContext::RangeExprContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::RangeExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRangeExpr(this);
}
void FormulaParser::RangeExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRangeExpr(this);
}

std::any FormulaParser::RangeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaVisitor*>(visitor))
    return parserVisitor->visitRangeExpr(this);
  else
    return visitor->visitChildren(this);
}

FormulaParser::ExprContext* FormulaParser::expr() {
   return expr(0);
}

FormulaParser::ExprContext* FormulaParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  FormulaParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  FormulaParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 2;
  enterRecursionRule(_localctx, 2, FormulaParser::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(32);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case FormulaParser::ID: {
        _localctx = _tracker.createInstance<FuncExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(9);
        match(FormulaParser::ID);
        setState(10);
        match(FormulaParser::LPAREN);
        setState(11);
        expr(0);
        setState(16);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == FormulaParser::COMMA) {
          setState(12);
          match(FormulaParser::COMMA);
          setState(13);
          expr(0);
          setState(18);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(19);
        match(FormulaParser::RPAREN);
        break;
      }

      case FormulaParser::CELL_REF: {
        _localctx = _tracker.createInstance<CellRefExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(21);
        match(FormulaParser::CELL_REF);
        break;
      }

      case FormulaParser::RANGE: {
        _localctx = _tracker.createInstance<RangeExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(22);
        match(FormulaParser::RANGE);
        break;
      }

      case FormulaParser::NUMBER: {
        _localctx = _tracker.createInstance<NumberExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(23);
        match(FormulaParser::NUMBER);
        break;
      }

      case FormulaParser::STRING: {
        _localctx = _tracker.createInstance<StringExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(24);
        match(FormulaParser::STRING);
        break;
      }

      case FormulaParser::TRUE: {
        _localctx = _tracker.createInstance<TrueExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(25);
        match(FormulaParser::TRUE);
        break;
      }

      case FormulaParser::FALSE: {
        _localctx = _tracker.createInstance<FalseExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(26);
        match(FormulaParser::FALSE);
        break;
      }

      case FormulaParser::ERROR: {
        _localctx = _tracker.createInstance<ErrorExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(27);
        match(FormulaParser::ERROR);
        break;
      }

      case FormulaParser::LPAREN: {
        _localctx = _tracker.createInstance<ParenExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(28);
        match(FormulaParser::LPAREN);
        setState(29);
        expr(0);
        setState(30);
        match(FormulaParser::RPAREN);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(63);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(61);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<EqExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(34);

          if (!(precpred(_ctx, 18))) throw FailedPredicateException(this, "precpred(_ctx, 18)");
          setState(35);
          match(FormulaParser::EQ);
          setState(36);
          expr(18);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<GteExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(37);

          if (!(precpred(_ctx, 17))) throw FailedPredicateException(this, "precpred(_ctx, 17)");
          setState(38);
          match(FormulaParser::GTE);
          setState(39);
          expr(18);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<LteExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(40);

          if (!(precpred(_ctx, 16))) throw FailedPredicateException(this, "precpred(_ctx, 16)");
          setState(41);
          match(FormulaParser::LTE);
          setState(42);
          expr(17);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<GtExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(43);

          if (!(precpred(_ctx, 15))) throw FailedPredicateException(this, "precpred(_ctx, 15)");
          setState(44);
          match(FormulaParser::GT);
          setState(45);
          expr(16);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LtExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(46);

          if (!(precpred(_ctx, 14))) throw FailedPredicateException(this, "precpred(_ctx, 14)");
          setState(47);
          match(FormulaParser::LT);
          setState(48);
          expr(15);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<PlusExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(49);

          if (!(precpred(_ctx, 13))) throw FailedPredicateException(this, "precpred(_ctx, 13)");
          setState(50);
          match(FormulaParser::PLUS);
          setState(51);
          expr(14);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<MinusExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(52);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(53);
          match(FormulaParser::MINUS);
          setState(54);
          expr(13);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<MultExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(55);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(56);
          match(FormulaParser::MULT);
          setState(57);
          expr(12);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<DivExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(58);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(59);
          match(FormulaParser::DIV);
          setState(60);
          expr(11);
          break;
        }

        default:
          break;
        } 
      }
      setState(65);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool FormulaParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 1: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool FormulaParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 18);
    case 1: return precpred(_ctx, 17);
    case 2: return precpred(_ctx, 16);
    case 3: return precpred(_ctx, 15);
    case 4: return precpred(_ctx, 14);
    case 5: return precpred(_ctx, 13);
    case 6: return precpred(_ctx, 12);
    case 7: return precpred(_ctx, 11);
    case 8: return precpred(_ctx, 10);

  default:
    break;
  }
  return true;
}

void FormulaParser::initialize() {
  std::call_once(formulaParserOnceFlag, formulaParserInitialize);
}
