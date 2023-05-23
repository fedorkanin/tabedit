#pragma once

#include "../../formula_token/data_types/abstract_data_type.hpp"
#include "../libs/json.hpp"

struct ToJSONVisitor {
    using json = nlohmann::json;
    template <typename T>
    json operator()(const T& t) const {
        return t.toJSON();
    }
};
