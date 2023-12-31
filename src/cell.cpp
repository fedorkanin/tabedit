#include "cell.hpp"

void Cell::setFormula(std::unique_ptr<Formula> formula) {
    formula_ptr_ = std::move(formula);
}

void Cell::setFormula(std::string raw_formula) {
    setFormula(std::make_unique<Formula>(raw_formula));
}

std::set<CellCoord> Cell::getReferencedCells() const {
    return formula_ptr_ ? formula_ptr_->getReferencedCells()
                        : std::set<CellCoord>();
}

std::string Cell::dump() const {
    std::string result;
    // value and formula if present
    if (hasValue()) result += std::visit(ToStringVisitor(), value_.value());
    if (hasFormula()) result += "=" + formula_ptr_->toString();
    if (result.empty()) result = "Empty cell\n";

    return result;
}

std::string Cell::dumpFull() const {
    std::string result;
    // value and formula if present
    if (hasValue()) result += std::visit(ToStringVisitor(), value_.value());
    if (hasFormula()) {
        result += "=" + formula_ptr_->toString() + "\n";
        for (const auto& token : formula_ptr_->getRPN()) {
            result += token->toString() + " ";
        }
        result += "\n";
    }
    if (result.empty()) result = "Empty cell\n";
    if (hasDependants()) {
        result += " dependants: ";
        for (const auto& dependant : dependants_)
            result += dependant.toString() + " ";
    }

    return result;
}

bool Cell::isDependentOn(const CellCoord& coord) const {
    return formula_ptr_ ? formula_ptr_->isDependentOn(coord) : false;
}

void Cell::addDependant(CellCoord coord) {
    dependants_.emplace(std::move(coord));
}

std::string Cell::toString() const {
    if (hasValue()) {
        auto value = value_.value();
        return std::visit(ToStringVisitor(), value);
    }
    if (hasFormula()) return "#REF";
    return "";
}

void to_json(nlohmann::json& j, const Cell& c) {
    if (c.hasFormula()) {
        j = c.getFormula();
    } else if (c.hasValue()) {
        j = std::visit(ToJSONVisitor(), c.getOptionalValue().value());
    } else {
        j = nullptr;
    }
}
