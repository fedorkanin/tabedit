# Table Editor Project

This project is a Command Line Interface (CLI) table editor created as a final work for the "Programming and Algorithmization 2" course at CTU (Czech Technical University).

## Overview

This table editor allows users to create and edit tables. The cells in the tables can contain strings, numeric data, cell coordinates, formulas with parentheses and basic arithmetic operations. The table supports dynamic evaluation of the formulas as the table is edited.

## Building the Project

### Dependencies

- C++17
- g++ (7.3 or later for full C++17 support)
- graphviz (for generating class diagrams)
- Doxygen (for generating documentation)

### Build Commands

- `make all` - Build the project and create documentation
- `make compile` - Compile the project
- `make run` - Run the project
- `make clean` - Clean the project
- `make doc` - Generate documentation (requires Doxygen)

## Getting Started

1. Clone the repository
2. Ensure you have all the dependencies installed.
3. Build the project using `make all`.
4. Run the project using `make run`.

## Features

### Cell Value Types
Cells can contain different data types, including strings, integers, and doubles. The type is determined dynamically using std::variant.

### Formulas
Formulas are dynamically evaluated and can contain basic arithmetic operations (+, -, *, /) as well as functions like sin, cos, and abs. They can also include references to other cells in the form of coordinates. 

### Import & Export
The table editor supports JSON import and export functionality through the `json` and `import` commands respectively.

### Commands
There are various commands available for interacting with the table. To view the list of commands, type `help` in the CLI or refer to the help file.

## Polymorphism
The code makes extensive use of polymorphism:

- **Cell**: `FormulaCell` and `ValueCell` classes are derived from the `Cell` class. They can both be evaluated which is the primary reason for inheritance.

- **Formula and FormulaToken**: `FormulaCell` contains a `Formula`, which is a vector of `FormulaTokens`. `FormulaToken` can be a number, a cell reference, an operator, or a parenthesis.

- **Basic Data Types and Operations**: The code can perform operations on different data types (string, double, and integer) polymorphically. Various patterns were considered (RTTI via dynamic_cast, Double dispatch, Double dispatch with visitor, Visitor via std::variant and std::visit), with the last one being selected for implementation due to its type safety, speed, and extensibility.

- **Operation Class**: An `Operation` class is derived from `FormulaToken` to execute operations. It holds a unary or binary `std::function`, which can be executed on `AbstractDataType` (std::variant of string, double, and int). Operations are created and passed in `Operation` via `OperationFactory`.

- **OperationFactory**: This utilizes Visitor classes to create operations. Currently, it is implemented as a static class but is planned to be converted into a singleton.

For more in-depth details, please refer to `why_visitor.txt` in the static folder.

## Conclusion

Efforts have been made to select the most elegant and efficient solutions for the problems faced during the development of this project. While perfection is subjective, this project surely serves as a learning curve. It is hoped that you enjoy going through the codebase as much as the development process was relished.


