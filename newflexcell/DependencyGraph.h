// DependencyGraph.h
#ifndef FORMULAS_PARSER_DEPENDENCY_GRAPH_H
#define FORMULAS_PARSER_DEPENDENCY_GRAPH_H

#include "Cell.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class DependencyGraph {
public:
    void addDependency(CellRef from, CellRef to);
    void addRangeDependency(CellRef from, RangeRef range);
    void removeDependencies(CellRef from);
    std::vector<CellRef> getAffectedCells(CellRef ref) const;
    const std::unordered_map<RangeRef, std::vector<CellRef>, std::hash<RangeRef>>& getRangeDependencies() const;

private:
    std::unordered_map<CellRef, std::unordered_set<CellRef>, std::hash<CellRef>> forward_deps_;
    std::unordered_map<CellRef, std::unordered_set<CellRef>, std::hash<CellRef>> backward_deps_;
    std::unordered_map<RangeRef, std::vector<CellRef>, std::hash<RangeRef>> range_dependencies_;
};

#endif // FORMULAS_PARSER_DEPENDENCY_GRAPH_H