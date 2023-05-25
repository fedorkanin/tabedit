#include "double.hpp"

std::string Double::toString() const {
    double rounded = std::round(value_ * pow(10, DOUBLE_PRECISION)) /
                     pow(10, DOUBLE_PRECISION);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(DOUBLE_PRECISION) << rounded;
    return ss.str();
}

void to_json(nlohmann::json& j, const Double& double_) {
    j = double_.toString();
}
