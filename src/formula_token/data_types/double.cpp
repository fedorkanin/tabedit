#include "double.hpp"

using json = nlohmann::json;
json Double::toJSON() const {
    json j;
    j["type"]  = "double";
    j["value"] = value_;
    return j;
}