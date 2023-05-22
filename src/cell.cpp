#include "cell.hpp"

std::string Cell::toString() const {
    // if (hasValue()) return std::visit(ToStringVisitor(), value_.value());
    if (hasValue()) {
        auto value = value_.value();
        return std::visit(ToStringVisitor(), value);
    }
    if (hasFormula()) return "#REF";
    return "";
}