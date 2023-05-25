#include "abstract_data_type.hpp"

ADT AbstractDataType::parsePrimitive(std::string raw_value) {
    // remove leading and trailing whitespace
    raw_value.erase(0, raw_value.find_first_not_of(' '));
    raw_value.erase(raw_value.find_last_not_of(' ') + 1);

    /// @todo: error handling in stod may not be preferable
    if (raw_value[0] == '"' && raw_value[raw_value.size() - 1] == '"')
        return String(raw_value.substr(1, raw_value.size() - 2));
    else if (raw_value.find('.') != std::string::npos) {
        auto result = std::stod(raw_value);
        if (std::to_string(result) != raw_value)
            throw std::runtime_error("Cannot parse double: " + raw_value);
        return Double(result);
    } else {
        auto result = std::stoll(raw_value);
        if (std::to_string(result) != raw_value)
            throw std::runtime_error("Cannot parse integer: " + raw_value);
        return Integer(result);
    }
}