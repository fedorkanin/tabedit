#pragma once

#include "../../../libs/json.hpp"
#include "../../formula_token/data_types/abstract_data_type.hpp"

struct ToJSONVisitor {
    using json = nlohmann::json;
    template <typename T>
    json operator()(const T& t) const {
        json j;
        to_json(j, t);
        return j;
    }
};
