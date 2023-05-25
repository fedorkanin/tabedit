#include "integer.hpp"

void to_json(nlohmann::json& j, const Integer& integer) {
    j = std::to_string(integer.value_);
}