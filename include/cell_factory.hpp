#include <memory>

#include "formula_cell.hpp"
#include "primitive_cell.hpp"

class CellFactory {
   public:
    // throws exception if cannot parse
    static AbstractDataType parsePrimitive(std::string primitive) {
        if (primitive[0] == '\"' && primitive[primitive.size() - 1] == '\"')
            return String(primitive.substr(1, primitive.size() - 2));
        else if (primitive.find(".") != std::string::npos)
            return Double(std::stod(primitive));
        else
            return Integer(std::stoi(primitive));
    }

    static std::shared_ptr<Cell> createCell(std::string cell,
                                            CellTable*  table) {
        if (cell[0] == '=')
            return std::make_shared<FormulaCell>(cell.substr(1), table);
        else
            return std::make_shared<PrimitiveCell>(
                parsePrimitive(std::move(cell)));
    }
};
