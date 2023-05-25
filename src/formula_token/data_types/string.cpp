#include "string.hpp"

void to_json(nlohmann::json& j, const String& p) {
    j = nlohmann::json("\"" + *p.value_ptr_ + "\"");
}