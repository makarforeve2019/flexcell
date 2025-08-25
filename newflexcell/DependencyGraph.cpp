// DependencyGraph.cpp
#include "DependencyGraph.h"
#include <algorithm>

void DependencyGraph::addDependency(CellRef from, CellRef to) {
    forward_deps_[from].insert(to);
    backward_deps_[to].insert(from);
}

void DependencyGraph::addRangeDependency(CellRef from, RangeRef range) {
    range_dependencies_[range].push_back(from);
}

void DependencyGraph::removeDependencies(CellRef from) {
    for (const auto& to : forward_deps_[from]) {
        backward_deps_[to].erase(from);
        if (backward_deps_[to].empty()) {
            backward_deps_.erase(to);
        }
    }
    forward_deps_.erase(from);

    for (auto& [range, cells] : range_dependencies_) {
        cells.erase(std::remove(cells.begin(), cells.end(), from), cells.end());
        if (cells.empty()) {
            range_dependencies_.erase(range);
        }
    }
}

std::vector<CellRef> DependencyGraph::getAffectedCells(CellRef ref) const {
    std::vector<CellRef> result;
    auto it = backward_deps_.find(ref);
    if (it != backward_deps_.end()) {
        result.insert(result.end(), it->second.begin(), it->second.end());
    }
    return result;
}

const std::unordered_map<RangeRef, std::vector<CellRef>, std::hash<RangeRef>>& DependencyGraph::getRangeDependencies() const {
    return range_dependencies_;
}