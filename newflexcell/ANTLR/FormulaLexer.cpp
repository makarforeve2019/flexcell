
// Generated from newflexcell/ANTLR/Formula.g4 by ANTLR 4.10


#include "FormulaLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct FormulaLexerStaticData final {
  FormulaLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  FormulaLexerStaticData(const FormulaLexerStaticData&) = delete;
  FormulaLexerStaticData(FormulaLexerStaticData&&) = delete;
  FormulaLexerStaticData& operator=(const FormulaLexerStaticData&) = delete;
  FormulaLexerStaticData& operator=(FormulaLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag formulalexerLexerOnceFlag;
FormulaLexerStaticData *formulalexerLexerStaticData = nullptr;

void formulalexerLexerInitialize() {
  assert(formulalexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<FormulaLexerStaticData>(
    std::vector<std::string>{
      "NUMBER", "STRING", "CELL_REF", "RANGE", "TRUE", "FALSE", "ERROR", 
      "ID", "PLUS", "MINUS", "MULT", "DIV", "EQ", "GT", "LT", "GTE", "LTE", 
      "LPAREN", "RPAREN", "COMMA", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,21,134,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,1,0,
  	4,0,45,8,0,11,0,12,0,46,1,0,1,0,4,0,51,8,0,11,0,12,0,52,3,0,55,8,0,1,
  	1,1,1,5,1,59,8,1,10,1,12,1,62,9,1,1,1,1,1,1,2,4,2,67,8,2,11,2,12,2,68,
  	1,2,4,2,72,8,2,11,2,12,2,73,1,3,1,3,1,3,1,3,1,4,1,4,1,4,1,4,1,4,1,5,1,
  	5,1,5,1,5,1,5,1,5,1,6,1,6,4,6,93,8,6,11,6,12,6,94,1,7,4,7,98,8,7,11,7,
  	12,7,99,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,1,12,1,12,1,13,1,13,1,14,
  	1,14,1,15,1,15,1,15,1,16,1,16,1,16,1,17,1,17,1,18,1,18,1,19,1,19,1,20,
  	4,20,129,8,20,11,20,12,20,130,1,20,1,20,0,0,21,1,1,3,2,5,3,7,4,9,5,11,
  	6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,18,
  	37,19,39,20,41,21,1,0,6,1,0,48,57,1,0,34,34,1,0,65,90,3,0,33,33,47,47,
  	65,90,2,0,65,90,97,122,3,0,9,10,13,13,32,32,142,0,1,1,0,0,0,0,3,1,0,0,
  	0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,
  	1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,
  	0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,
  	0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,1,44,1,0,0,0,3,56,1,0,0,0,5,66,
  	1,0,0,0,7,75,1,0,0,0,9,79,1,0,0,0,11,84,1,0,0,0,13,90,1,0,0,0,15,97,1,
  	0,0,0,17,101,1,0,0,0,19,103,1,0,0,0,21,105,1,0,0,0,23,107,1,0,0,0,25,
  	109,1,0,0,0,27,111,1,0,0,0,29,113,1,0,0,0,31,115,1,0,0,0,33,118,1,0,0,
  	0,35,121,1,0,0,0,37,123,1,0,0,0,39,125,1,0,0,0,41,128,1,0,0,0,43,45,7,
  	0,0,0,44,43,1,0,0,0,45,46,1,0,0,0,46,44,1,0,0,0,46,47,1,0,0,0,47,54,1,
  	0,0,0,48,50,5,46,0,0,49,51,7,0,0,0,50,49,1,0,0,0,51,52,1,0,0,0,52,50,
  	1,0,0,0,52,53,1,0,0,0,53,55,1,0,0,0,54,48,1,0,0,0,54,55,1,0,0,0,55,2,
  	1,0,0,0,56,60,5,34,0,0,57,59,8,1,0,0,58,57,1,0,0,0,59,62,1,0,0,0,60,58,
  	1,0,0,0,60,61,1,0,0,0,61,63,1,0,0,0,62,60,1,0,0,0,63,64,5,34,0,0,64,4,
  	1,0,0,0,65,67,7,2,0,0,66,65,1,0,0,0,67,68,1,0,0,0,68,66,1,0,0,0,68,69,
  	1,0,0,0,69,71,1,0,0,0,70,72,7,0,0,0,71,70,1,0,0,0,72,73,1,0,0,0,73,71,
  	1,0,0,0,73,74,1,0,0,0,74,6,1,0,0,0,75,76,3,5,2,0,76,77,5,58,0,0,77,78,
  	3,5,2,0,78,8,1,0,0,0,79,80,5,84,0,0,80,81,5,82,0,0,81,82,5,85,0,0,82,
  	83,5,69,0,0,83,10,1,0,0,0,84,85,5,70,0,0,85,86,5,65,0,0,86,87,5,76,0,
  	0,87,88,5,83,0,0,88,89,5,69,0,0,89,12,1,0,0,0,90,92,5,35,0,0,91,93,7,
  	3,0,0,92,91,1,0,0,0,93,94,1,0,0,0,94,92,1,0,0,0,94,95,1,0,0,0,95,14,1,
  	0,0,0,96,98,7,4,0,0,97,96,1,0,0,0,98,99,1,0,0,0,99,97,1,0,0,0,99,100,
  	1,0,0,0,100,16,1,0,0,0,101,102,5,43,0,0,102,18,1,0,0,0,103,104,5,45,0,
  	0,104,20,1,0,0,0,105,106,5,42,0,0,106,22,1,0,0,0,107,108,5,47,0,0,108,
  	24,1,0,0,0,109,110,5,61,0,0,110,26,1,0,0,0,111,112,5,62,0,0,112,28,1,
  	0,0,0,113,114,5,60,0,0,114,30,1,0,0,0,115,116,5,62,0,0,116,117,5,61,0,
  	0,117,32,1,0,0,0,118,119,5,60,0,0,119,120,5,61,0,0,120,34,1,0,0,0,121,
  	122,5,40,0,0,122,36,1,0,0,0,123,124,5,41,0,0,124,38,1,0,0,0,125,126,5,
  	44,0,0,126,40,1,0,0,0,127,129,7,5,0,0,128,127,1,0,0,0,129,130,1,0,0,0,
  	130,128,1,0,0,0,130,131,1,0,0,0,131,132,1,0,0,0,132,133,6,20,0,0,133,
  	42,1,0,0,0,10,0,46,52,54,60,68,73,94,99,130,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  formulalexerLexerStaticData = staticData.release();
}

}

FormulaLexer::FormulaLexer(CharStream *input) : Lexer(input) {
  FormulaLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *formulalexerLexerStaticData->atn, formulalexerLexerStaticData->decisionToDFA, formulalexerLexerStaticData->sharedContextCache);
}

FormulaLexer::~FormulaLexer() {
  delete _interpreter;
}

std::string FormulaLexer::getGrammarFileName() const {
  return "Formula.g4";
}

const std::vector<std::string>& FormulaLexer::getRuleNames() const {
  return formulalexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& FormulaLexer::getChannelNames() const {
  return formulalexerLexerStaticData->channelNames;
}

const std::vector<std::string>& FormulaLexer::getModeNames() const {
  return formulalexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& FormulaLexer::getVocabulary() const {
  return formulalexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView FormulaLexer::getSerializedATN() const {
  return formulalexerLexerStaticData->serializedATN;
}

const atn::ATN& FormulaLexer::getATN() const {
  return *formulalexerLexerStaticData->atn;
}




void FormulaLexer::initialize() {
  std::call_once(formulalexerLexerOnceFlag, formulalexerLexerInitialize);
}
