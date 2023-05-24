#include "table_editor.hpp"

void TableEditor::signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Received interrupt, exiting..." << std::endl;
        exit(0);
    }
}

void TableEditor::displayInPager(const std::string& filename) {
    std::stringstream command;
    command << "less " << filename;
    system(command.str().c_str());
}

void TableEditor::processLessCommand() {
    std::ofstream outputFile("table.txt");
    if (outputFile.is_open()) {
        outputFile << table;
        outputFile.close();
        displayInPager("table.txt");
    } else {
        std::cout << "Failed to open file for writing." << std::endl;
    }
}

void TableEditor::processExitCommand() { exit(0); }

void TableEditor::processJsonCommand() {
    std::cout << nlohmann::json(table) << std::endl;
}

void TableEditor::processPrintCommand(const std::string& command) {
    std::string cellAddress = command.substr(6);
    CellCoord   coord(cellAddress);
    try {
        std::shared_ptr<Cell> cell = table.at(coord);
        if (cell) {
            std::cout << "Dump of cell " << cellAddress << ":" << std::endl;
            std::cout << cell->dump() << std::endl;
        } else {
            std::cout << "Cell " << cellAddress << " does not exist."
                      << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void TableEditor::processDumpCommand(const std::string& command) {
    std::string cellAddress = command.substr(5);
    CellCoord   coord(cellAddress);
    try {
        std::shared_ptr<Cell> cell = table.at(coord);
        if (cell) {
            std::cout << "Dump of cell " << cellAddress << ":" << std::endl;
            std::cout << cell->dumpFull() << std::endl;
        } else {
            std::cout << "Cell " << cellAddress << " does not exist."
                      << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void TableEditor::processCellAssignmentCommand(
    const std::string& command, std::string::size_type equalsPos) {
    std::string coordString = command.substr(0, equalsPos);
    std::string value       = command.substr(equalsPos + 1);

    CellCoord coord(coordString);
    try {
        table.setCell(coord, value);
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void TableEditor::processHelpCommand() { displayInPager("./static/help.txt"); }

std::string TableEditor::trim(const std::string& str) {
    std::string whitespace = " \t\n\r\f\v";
    std::size_t start      = str.find_first_not_of(whitespace);
    std::size_t end        = str.find_last_not_of(whitespace);
    return (start == std::string::npos) ? ""
                                        : str.substr(start, end - start + 1);
}

void TableEditor::processEditCommand(const std::string& command) {
    std::string cellAddress = command.substr(5);
    CellCoord   coord(cellAddress);
    try {
        std::shared_ptr<Cell> cell = table.at(coord);

        std::string currentValue = cell ? cell->dump() : "empty";
        std::cout << "Current value: " << cellAddress << ": " << currentValue
                  << std::endl;

        char* input = linenoise((cellAddress + '=').c_str());
        if (input == nullptr) {
            std::cout << "Edit interrupted, exiting..." << std::endl;
            exit(0);
        }
        linenoiseHistoryAdd(input); /* Remember the command for history. */

        processCellAssignmentCommand(cellAddress + "=" + input,
                                     cellAddress.size());

        free(input);

    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

TableEditor::TableEditor(size_t rows, size_t cols) : table(rows, cols) {
    std::signal(SIGINT, signalHandler);
}

void TableEditor::run() {
    while (true) {
        char* input = linenoise("Enter command: ");
        if (input == nullptr) {
            std::cout << "Received interrupt, exiting..." << std::endl;
            exit(0);
        }
        linenoiseHistoryAdd(input);  // Remember the command for history.

        try {
            std::string            command   = trim(input);
            std::string::size_type equalsPos = command.find('=');

            if (command == "less")
                processLessCommand();
            else if (command == "exit")
                processExitCommand();
            if (command.substr(0, 4) == "edit")
                processEditCommand(command);
            else if (command == "json")
                processJsonCommand();
            else if (command.substr(0, 5) == "print")
                processPrintCommand(command);
            // dump
            else if (command.substr(0, 4) == "dump") {
                processDumpCommand(command);
            } else if (equalsPos != std::string::npos && equalsPos > 0)
                processCellAssignmentCommand(command, equalsPos);
            else if (command == "help")
                processHelpCommand();
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        free(input);
    }
}