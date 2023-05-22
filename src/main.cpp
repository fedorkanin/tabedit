#include <csignal>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../libs/linenoise.h"
#include "cell_table.hpp"

// Clear the console
void clearConsole() {
    // On Windows, use system("cls"); on Linux or macOS, use system("clear")
    system("clear");
}

// Signal handler for Ctrl+C
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Received interrupt, exiting..." << std::endl;
        exit(0);
    }
}

// Function to display table in less pager
void displayTableInPager(const std::string& filename) {
    std::stringstream command;
    command << "less " << filename;
    system(command.str().c_str());
}

int main() {
    CellTable table(100, 10);  // Create a 20x20 table

    // Register signal handler for Ctrl+C
    std::signal(SIGINT, signalHandler);

    while (true) {
        char* input = linenoise("Enter command: ");
        if (input == nullptr) {
            std::cout << "Received interrupt, exiting..." << std::endl;
            exit(0);
        }
        linenoiseHistoryAdd(input); /* Remember the command for history. */

        std::string            command   = input;
        std::string::size_type equalsPos = command.find('=');

        if (equalsPos != std::string::npos && equalsPos > 0) {
            std::string coordString = command.substr(0, equalsPos);
            std::string value       = command.substr(equalsPos);

            CellCoord coord(coordString);
            try {
                table.setCell(coord, value);
            } catch (std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }

        std::ofstream outputFile("table.txt");
        if (outputFile.is_open()) {
            outputFile << table;
            outputFile.close();
            displayTableInPager("table.txt");
        } else {
            std::cout << "Failed to open file for writing." << std::endl;
        }

        clearConsole();

        free(input);
    }

    return 0;
}
