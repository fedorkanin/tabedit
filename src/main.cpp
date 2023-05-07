#include <iostream>

#include "primitives/abstract_primitive.hpp"
#include "primitives/double.hpp"
#include "primitives/integer.hpp"

int main() {
    std::unique_ptr<AbstractPrimitive> dbl     = std::make_unique<Double>(1.0);
    std::unique_ptr<AbstractPrimitive> integer = std::make_unique<Integer>(1);

    std::cout << dbl->getSum(*integer)->toString() << std::endl;
}