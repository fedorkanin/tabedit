#include <iostream>

#include "data_types/abstract_data_type.hpp"
#include "data_types/double.hpp"
#include "data_types/integer.hpp"

int main() {
    std::unique_ptr<AbstractDataType> dbl     = std::make_unique<Double>(1.0);
    std::unique_ptr<AbstractDataType> integer = std::make_unique<Integer>(2);

    std::cout << dbl->getSum(*integer)->toString() << std::endl;
}