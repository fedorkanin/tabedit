#include "primitives/abstract_primitive.hpp"

class Cell {
   private:
    std::unique_ptr<AbstractPrimitive> data_;
};
