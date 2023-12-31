#include "cell_coord.hpp"

#define ALPHABET_SIZE ('Z' - 'A' + 1)

CellCoord::CellCoord(std::string raw_coord) {
    std::pair<int, int> parsed_coord = parseRawCoord(raw_coord);
    col_                             = parsed_coord.first;
    row_                             = parsed_coord.second;
}

std::pair<int, int> CellCoord::parseRawCoord(std::string raw_coord) {
    // remove leading and trailing whitespace
    raw_coord.erase(0, raw_coord.find_first_not_of(' '));
    raw_coord.erase(raw_coord.find_last_not_of(' ') + 1);

    auto first_digit_it =
        std::find_if(raw_coord.begin(), raw_coord.end(), ::isdigit);

    if (first_digit_it == raw_coord.end())
        throw std::invalid_argument("Invalid cell coordinate");

    std::string col_str(raw_coord.begin(), first_digit_it);
    if (col_str.empty()) throw std::invalid_argument("Invalid cell coordinate");
    std::string row_str(first_digit_it, raw_coord.end());
    if (row_str.empty()) throw std::invalid_argument("Invalid cell coordinate");

    if (std::to_string(std::stoi(row_str)) != row_str)
        throw std::invalid_argument("Invalid cell coordinate");

    int col_num = 0;
    for (char c : col_str) {
        if (!std::isalpha(c))
            throw std::invalid_argument("Invalid cell coordinate");
        col_num *= ALPHABET_SIZE;
        col_num += std::toupper(c) - 'A' + 1;
    }

    return {col_num - 1, std::stoi(row_str) - 1};
}

bool CellCoord::isValidCoord(std::string raw_coord) {
    try {
        parseRawCoord(raw_coord);
        return true;
    } catch (std::invalid_argument&) {
        return false;
    }
}

bool CellCoord::operator==(const CellCoord& other) const {
    return row_ == other.row_ && col_ == other.col_;
}

bool CellCoord::operator<(const CellCoord& other) const {
    return row_ < other.row_ || (row_ == other.row_ && col_ < other.col_);
}

std::string CellCoord::toString() const {
    std::string col_str;
    int         col_num = col_ + 1;
    while (col_num > 0) {
        col_num--;
        col_str = static_cast<char>('A' + (col_num % ALPHABET_SIZE)) + col_str;
        col_num /= ALPHABET_SIZE;
    }
    return col_str + std::to_string(row_ + 1);
}

std::string CellCoord::getColName(unsigned int col) {
    std::string col_str;
    int         col_num = col + 1;
    while (col_num > 0) {
        col_num--;
        col_str = static_cast<char>('A' + (col_num % ALPHABET_SIZE)) + col_str;
        col_num /= ALPHABET_SIZE;
    }
    return col_str;
}

std::ostream& operator<<(std::ostream& os, const CellCoord& coord) {
    os << coord.toString();
    return os;
}

void to_json(nlohmann::json& j, const CellCoord& p) {
    j = nlohmann::json(p.toString());
}
