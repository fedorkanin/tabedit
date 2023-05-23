#include "integer.hpp"

using json = nlohmann::json;
json Integer::toJSON() const {
    json j;
    j["type"]  = "integer";
    j["value"] = value_;
    return j;
}