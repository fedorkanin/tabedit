#include "string.hpp"

using json = nlohmann::json;
json String::toJSON() const {
    json j;
    j["type"]  = "string";
    j["value"] = *value_;
    return j;
}
