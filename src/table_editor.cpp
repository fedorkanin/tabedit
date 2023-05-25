#include "table_editor.hpp"

void TableEditor::signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Received interrupt, exiting..." << std::endl;
        exit(0);
    }
}

void TableEditor::displayInPager(const std::string& filename) {
    std::stringstream command;
    command << "less -S " << filename;
    system(command.str().c_str());
}

void TableEditor::processLessCommand() {
    std::ofstream outputFile("table.tmp");
    if (outputFile.is_open()) {
        outputFile << table;
        outputFile.close();
        displayInPager("table.tmp");
    } else {
        std::cout << "Failed to open file for writing." << std::endl;
    }
}

void TableEditor::processShrinkCommand() { table.shrinkToFit(); }

void TableEditor::processExitCommand() { exit(0); }

void TableEditor::processJsonCommand(const std::string& command) {
    std::string filename = "";  // Empty filename if no name is provided
    if (command.size() > 5) {
        filename = command.substr(5);
    }

    if (filename.empty()) {
        std::cout << nlohmann::json(table).dump(4) << std::endl;
        return;
    }

    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        nlohmann::json json_data(table);
        outputFile << json_data.dump(
            4);  // Write JSON data with indentation of 4 spaces
        outputFile.close();
        std::cout << "JSON data written to file: " << filename << std::endl;
    } else {
        std::cout << "Failed to open file for writing." << std::endl;
    }
}

void TableEditor::processPrintCommand(const std::string& command) {
    std::string cellAddress = command.substr(6);
    CellCoord   coord(cellAddress);
    try {
        std::shared_ptr<Cell> cell = table.at(coord);
        if (cell) {
            std::cout << "Cell " << cellAddress << ":" << std::endl;
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

    CellCoord   coord(coordString);
    try {
        table.setCell(coord, value);
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void TableEditor::processHelpCommand() { displayInPager("./assets/help.txt"); }

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

        std::string           currentValue = cell ? cell->dump() : "empty";
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

void TableEditor::processImportCommand(const std::string& command) {
    std::string filename =
        command.substr(7);  // Extract the filename from the command

    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        try {
            nlohmann::json json_data;
            inputFile >> json_data;
            table = json_data.get<CellTable>();
            std::cout << "Table imported from JSON file: " << filename
                      << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error while parsing JSON or evaluating cells in "
                      << filename << ": " << e.what() << std::endl;
        }
        inputFile.close();
    } else {
        std::cout << "Failed to open JSON file: " << filename << std::endl;
    }
}

TableEditor::TableEditor(size_t rows, size_t cols) : table(rows, cols) {
    std::signal(SIGINT, signalHandler);
}

void TableEditor::processClearCommand() { table.clear(); }

/// @todo: If circular references, json import is going to fail.
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

            if (equalsPos != std::string::npos && equalsPos > 0)
                processCellAssignmentCommand(command, equalsPos);
            else if (command == "less")
                processLessCommand();
            else if (command == "exit")
                processExitCommand();
            else if (command.substr(0, 4) == "edit")
                processEditCommand(command);
            else if (command.substr(0, 4) == "json")
                processJsonCommand(command);
            else if (command.substr(0, 5) == "print")
                processPrintCommand(command);
            else if (command.substr(0, 4) == "dump")
                processDumpCommand(command);
            else if (command == "help")
                processHelpCommand();
            else if (command.substr(0, 6) == "import")
                processImportCommand(command);
            else if (command == "clear")
                processClearCommand();
            else if (command == "shrink")
                processShrinkCommand();
            else
                std::cout << "Unknown command: " << command << std::endl;
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        free(input);
    }
}
