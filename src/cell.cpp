#include "cell.hpp"

std::set<CellCoord> Cell::getReferencedCells() const {
    return formula_ptr_ ? formula_ptr_->getReferencedCells()
                        : std::set<CellCoord>();
}

std::string Cell::toString() const {
    // if (hasValue()) return std::visit(ToStringVisitor(), value_.value());
    if (hasValue()) {
        auto value = value_.value();
        return std::visit(ToStringVisitor(), value);
    }
    if (hasFormula()) return "#REF";
    return "";
}

using json = nlohmann::json;
json Cell::toJSON() const {
    if (hasFormula()) return {{"formula", formula_ptr_->toJSON()}};
    if (hasValue())
        return {{"value", std::visit(ToJSONVisitor(), value_.value())}};
    throw std::runtime_error("Cell has neither formula nor value");
}
