#include "notype.hpp"

using json = nlohmann::json;
json NoType::toJSON() const {
    json j;
    j["type"] = "notype";
    return j;
}