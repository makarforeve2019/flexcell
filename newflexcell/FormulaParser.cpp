// FormulaParser.cpp
#include "FormulaParser.h"
#include <antlr4-runtime.h>
#include "ANTLR/FormulaLexer.h"
#include "ANTLR/FormulaParser.h"
#include "FormulaASTBuilder.h"

// Конструктор
CustomFormulaParser::CustomFormulaParser() {
}

// Деструктор
CustomFormulaParser::~CustomFormulaParser() {
}

// Парсинг формулы
FormulaASTResult CustomFormulaParser::parse(const std::string& formula) {
    try {
        std::cout << "Parsing formula: " << formula << std::endl;

        antlr4::ANTLRInputStream input(formula);
        FormulaLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);

        tokens.fill();
        std::cout << "Tokens:";
        for (auto token : tokens.getTokens()) {
            std::cout << " [" << token->getText() << "]";
        }
        std::cout << std::endl;

        ::FormulaParser parser(&tokens);
        parser.removeErrorListeners();
        parser.setErrorHandler(std::make_shared<antlr4::BailErrorStrategy>());

        std::cout << "Invoking parser.formula()...\n";
        auto tree = parser.formula();
        std::cout << "Parse tree obtained.\n";

        FormulaASTBuilder builder;
        auto astPtr = std::any_cast<std::shared_ptr<FormulaAST>>(tree->accept(&builder));
        std::cout << "AST constructed.\n";

        return FormulaASTResult{*astPtr};
    }
    catch (const antlr4::RuntimeException& e) {
        std::cerr << "ANTLR RuntimeException: " << e.what() << std::endl;
        return FormulaASTResult{Error{ErrorCode::VALUE, "Syntax error: " + std::string(e.what())}};
    }
    catch (const std::bad_any_cast& e) {
        std::cerr << "std::bad_any_cast: " << e.what() << std::endl;
        return FormulaASTResult{Error{ErrorCode::VALUE, "AST conversion failed (any_cast)."}};
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return FormulaASTResult{Error{ErrorCode::VALUE, "Parsing error: " + std::string(e.what())}};
    }
}