#include <iostream>

#include "data_types/abstract_data_type.hpp"
#include "data_types/double.hpp"
#include "data_types/integer.hpp"
#include "formula.hpp"

int main() {
    std::unique_ptr<AbstractDataType> a = std::make_unique<Integer>(1);
    std::unique_ptr<AbstractDataType> b = std::make_unique<Double>(2.0);

    std::cout << a->getSum(*b)->toString() << std::endl;

    return EXIT_SUCCESS;
}