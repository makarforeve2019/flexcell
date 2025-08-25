// Evaluator.h
#ifndef FORMULAS_PARSER_EVALUATOR_H
#define FORMULAS_PARSER_EVALUATOR_H

#include "Error.h"
#include "FormulaAST.h"
#include "Cell.h"

class FormulaEngine;

class Evaluator {
public:
    explicit Evaluator(const FormulaEngine& engine);
    Result evaluate(const FormulaAST& ast) const;

    // Публичный доступ к engine_
    const FormulaEngine& engine_;

private:
    friend class FormulaAST;
};

#endif // FORMULAS_PARSER_EVALUATOR_H