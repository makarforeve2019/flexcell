#include "FormulaEngine.h"
#include <OpenXLSX.hpp>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <queue>
#include <unordered_set>

// Конструктор
FormulaEngine::FormulaEngine()
        : parser_(std::make_unique<CustomFormulaParser>()),
          evaluator_(std::make_unique<Evaluator>(*this)),
          graph_(std::make_unique<DependencyGraph>()),
          cells_(),
          changed_cells_() {
}

// Загружает XLSX-файл
bool FormulaEngine::loadXlsx(std::istream& input) {
    try {
        std::ofstream temp_file("temp.xlsx", std::ios::binary);
        temp_file << input.rdbuf();
        temp_file.close();

        OpenXLSX::XLDocument doc;
        doc.open("temp.xlsx");
        auto worksheet = doc.workbook().worksheet("Sheet1");

        for (uint32_t row = 1; row <= 1000; ++row) {
            for (uint32_t col = 1; col <= 26; ++col) {
                auto cell_ref = OpenXLSX::XLCellReference(row, col);
                auto cell = worksheet.cell(cell_ref);
                if (cell.value().type() == OpenXLSX::XLValueType::Empty && !cell.hasFormula()) continue;

                CellRef ref{row, col};
                uint64_t key = (static_cast<uint64_t>(row) << 32) | col;

                if (cell.hasFormula()) {
                    std::string formula;
                    try {
                        formula = "=" + cell.formula().get();
                    } catch (...) {
                        cells_[key] = Cell();
                        continue;
                    }

                    auto parse_result = parser_->parse(formula);
                    if (parse_result.is_success()) {
                        auto ast = parse_result.get_value();
                        cells_[key] = Cell(std::make_shared<FormulaAST>(ast));
                        for (const auto& dep : ast.getCellDependencies()) graph_->addDependency(ref, dep);
                        for (const auto& range : ast.getRangeDependencies()) graph_->addRangeDependency(ref, range);
                    } else {
                        cells_[key] = Cell();
                    }
                } else {
                    if (cell.value().type() == OpenXLSX::XLValueType::String) cells_[key] = Cell(cell.value().get<std::string>());
                    else if (cell.value().type() == OpenXLSX::XLValueType::Float || cell.value().type() == OpenXLSX::XLValueType::Integer) cells_[key] = Cell(cell.value().get<double>());
                    else if (cell.value().type() == OpenXLSX::XLValueType::Boolean) cells_[key] = Cell(cell.value().get<bool>());
                    else cells_[key] = Cell();
                }
            }
        }
        doc.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception while loading XLSX: " << e.what() << std::endl;
        return false;
    }
}

// Устанавливает значение или формулу
void FormulaEngine::setCellValue(CellRef ref, const Cell::Value& value) {
    uint64_t key = (static_cast<uint64_t>(ref.row) << 32) | ref.col;
    graph_->removeDependencies(ref);

    if (std::holds_alternative<std::string>(value) && std::get<std::string>(value)[0] == '=') {
        auto parse_result = parser_->parse(std::get<std::string>(value));
        if (parse_result.is_success()) {
            auto ast = parse_result.get_value();
            cells_[key] = Cell(std::make_shared<FormulaAST>(ast));
            for (const auto& dep : ast.getCellDependencies()) graph_->addDependency(ref, dep);
            for (const auto& range : ast.getRangeDependencies()) graph_->addRangeDependency(ref, range);
        } else {
            cells_[key] = Cell();
        }
    } else {
        cells_[key] = Cell(value);
    }
}

// Получает значение ячейки
Result FormulaEngine::getCellValue(CellRef ref) const {
    uint64_t key = (static_cast<uint64_t>(ref.row) << 32) | ref.col;
    auto it = cells_.find(key);
    if (it == cells_.end()) return Result{Cell::Value{0.0}};

    const Cell& cell = it->second;
    if (cell.formula) {
        if (cell.cached_value.has_value()) {
            if(std::holds_alternative<Error>(*cell.cached_value))
                return Result{std::get<Error>(*cell.cached_value)};
            else
                return Result{std::get<Cell::Value>(*cell.cached_value)};
        }
        auto result = evaluator_->evaluate(*cell.formula);
        // Мы не кэшируем здесь, кэширование происходит только при явном пересчете
        return result;
    }
    return Result{cell.value};
}

void FormulaEngine::recalculateAll() {
    changed_cells_.clear();
    for (auto& pair : cells_) {
        Cell& cell = pair.second;
        if (cell.formula) {
            auto result = evaluator_->evaluate(*cell.formula);
            if (result.is_success()) {
                cell.cached_value = result.get_value();
            } else {
                cell.cached_value = result.get_error();
            }
        }
    }
}

std::vector<std::pair<CellRef, std::variant<Cell::Value, Error>>> FormulaEngine::recalculate(CellRef ref) {
    changed_cells_.clear();
    std::queue<CellRef> cells_to_recalc;
    std::unordered_set<CellRef, std::hash<CellRef>> processed_cells;

    cells_to_recalc.push(ref);
    processed_cells.insert(ref);

    while (!cells_to_recalc.empty()) {
        CellRef current_ref = cells_to_recalc.front();
        cells_to_recalc.pop();

        auto it = cells_.find((static_cast<uint64_t>(current_ref.row) << 32) | current_ref.col);
        if (it != cells_.end()) {
            if (it->second.formula) {
                auto result = evaluator_->evaluate(*it->second.formula);
                if (result.is_success()) {
                    it->second.cached_value = result.get_value();
                    changed_cells_.emplace_back(current_ref, result.get_value());
                } else {
                    it->second.cached_value = result.get_error();
                    changed_cells_.emplace_back(current_ref, result.get_error());
                }
            }
        }

        auto affected = graph_->getAffectedCells(current_ref);
        for (const auto& affected_ref : affected) {
            if (processed_cells.find(affected_ref) == processed_cells.end()) {
                cells_to_recalc.push(affected_ref);
                processed_cells.insert(affected_ref);
            }
        }
    }
    return changed_cells_;
}


void FormulaEngine::serializeChangedCells(std::ostream& output) const {
    output << "[";
    for (size_t i = 0; i < changed_cells_.size(); ++i) {
        const auto& [ref, value_or_error] = changed_cells_[i];
        output << "{ \"row\": " << ref.row << ", \"col\": " << ref.col << ", \"value\": ";
        if(std::holds_alternative<Error>(value_or_error)) {
            output << "\"#ERROR!\"";
        } else {
            const auto& value = std::get<Cell::Value>(value_or_error);
            if (std::holds_alternative<double>(value)) output << std::get<double>(value);
            else if (std::holds_alternative<std::string>(value)) output << "\"" << std::get<std::string>(value) << "\"";
            else if (std::holds_alternative<bool>(value)) output << (std::get<bool>(value) ? "true" : "false");
        }
        output << "}";
        if (i < changed_cells_.size() - 1) output << ", ";
    }
    output << "]";
}

const std::unordered_map<uint64_t, Cell>& FormulaEngine::getAllCells() const {
    return cells_;
}