#include "cell_coord.hpp"

#define ALPHABET_SIZE ('Z' - 'A' + 1)

CellCoord::CellCoord(std::string raw_coord) {
    std::pair<int, int> parsed_coord = parseRawCoord(raw_coord);
    col_                             = parsed_coord.first;
    row_                             = parsed_coord.second;
}

std::pair<int, int> CellCoord::parseRawCoord(std::string raw_coord) {
    auto first_digit_it =
        std::find_if(raw_coord.begin(), raw_coord.end(), ::isdigit);

    if (first_digit_it == raw_coord.end())
        throw std::invalid_argument("Invalid cell coordinate");

    std::string col_str(raw_coord.begin(), first_digit_it);
    std::string row_str(first_digit_it, raw_coord.end());

    int col_num = 0;
    for (char c : col_str) col_num = col_num * ALPHABET_SIZE + (c - 'A');

    return {col_num, std::stoi(row_str) - 1};
}

bool CellCoord::isValidCoord(std::string raw_coord) {
    try {
        parseRawCoord(raw_coord);
        return true;
    } catch (std::invalid_argument&) {
        return false;
    }
}

std::string CellCoord::toString() const {
    std::string col_str;
    int         col_num = col_;
    while (col_num > 0) {
        col_num--;
        col_str = static_cast<char>('A' + (col_num % ALPHABET_SIZE)) + col_str;
        col_num /= ALPHABET_SIZE;
    }
    return col_str + std::to_string(row_);
}
