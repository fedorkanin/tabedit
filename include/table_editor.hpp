#pragma once

#include <algorithm>
#include <cctype>
#include <csignal>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>

#include "../libs/linenoise.h"
#include "cell_table.hpp"

class TableEditor {
   public:
    TableEditor(size_t rows, size_t cols);

    void run();

   private:
    CellTable   table;

    static void signalHandler(int signal);

    /// @brief Display a file in a less pager using system() call
    void        displayInPager(const std::string& filename);
    /// @brief Saves the table to a file and opens it in a less pager
    void        processLessCommand();
    /// @brief Shrinks the table to minimum size needed to fit all cells
    void        processShrinkCommand();
    /// @brief Exits the program
    void        processExitCommand();
    /// @brief Saves the table to a JSON file
    void        processJsonCommand(const std::string& command);
    /// @brief Prints the contents of a cell
    void        processPrintCommand(const std::string& command);
    /// @brief Prints full dump of the cell (see help for usage)
    void        processDumpCommand(const std::string& command);
    /// @brief Tries to parse a command as a cell assignment
    void        processCellAssignmentCommand(const std::string&     command,
                                             std::string::size_type equalsPos);
    /// @brief Prints help file using less pager
    void        processHelpCommand();
    /// @brief Clears the table, removing all cells
    void        processClearCommand();
    /// @brief Edits a cell
    void        processEditCommand(const std::string& command);
    /// @brief Imports a table from a JSON file, if file is invalid or not
    /// found, prints an error
    void        processImportCommand(const std::string& command);
    /// @brief Removes leading and trailing whitespace from a string
    std::string trim(const std::string& str);
};
