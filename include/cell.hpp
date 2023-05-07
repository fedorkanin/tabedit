#include "data_types/abstract_data_type.hpp"

class Cell {
   private:
    std::unique_ptr<AbstractDataType> data_;
};
