// FormulaEngine.h
#ifndef FORMULAS_PARSER_FORMULA_ENGINE_H
#define FORMULAS_PARSER_FORMULA_ENGINE_H

#include "Cell.h"
#include "FormulaParser.h"
#include "Evaluator.h"
#include "DependencyGraph.h"
#include <unordered_map>
#include <vector>
#include <iostream>

class FormulaEngine {
public:
    FormulaEngine();
    bool loadXlsx(std::istream& input);
    void setCellValue(CellRef ref, const Cell::Value& value);
    Result getCellValue(CellRef ref) const;

    // ИСПРАВЛЕНИЕ: Метод теперь возвращает вектор, содержащий либо значение, либо ошибку.
    std::vector<std::pair<CellRef, std::variant<Cell::Value, Error>>> recalculate(CellRef ref);
//    @brief Запускает полный пересчет всех формул на листе.
    void recalculateAll();

    void serializeChangedCells(std::ostream& output) const;
    const std::unordered_map<uint64_t, Cell>& getAllCells() const;

private:
    std::unordered_map<uint64_t, Cell> cells_;
    std::unique_ptr<CustomFormulaParser> parser_;
    std::unique_ptr<Evaluator> evaluator_;
    std::unique_ptr<DependencyGraph> graph_;

    // ИСПРАВЛЕНИЕ: Тип этого поля теперь соответствует возвращаемому типу recalculate.
    std::vector<std::pair<CellRef, std::variant<Cell::Value, Error>>> changed_cells_;
};

#endif // FORMULAS_PARSER_FORMULA_ENGINE_H