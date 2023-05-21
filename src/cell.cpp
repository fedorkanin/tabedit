#include "cell.hpp"

std::string Cell::toString() const {
    if (hasValue()) return std::visit(ToStringVisitor(), value_.value());
    if (hasFormula()) return "#REF";
    return "";
}