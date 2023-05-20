#include "cell.hpp"

void Cell::setValue(ADT value) {
    value_ = std::move(value);
    // TODO: handle value and formula simultaneously
    //  formula_ = nullptr;
}

std::string Cell::toString() const {
    if (hasValue()) return std::visit(ToStringVisitor(), value_.value());
    if (hasFormula()) return "#REF";
    return "";
}