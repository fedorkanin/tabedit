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

    /// @brief display a file in a less pager using system() call
    void        displayInPager(const std::string& filename);
    void        processLessCommand();
    void        processShrinkCommand();
    void        processExitCommand();
    void        processJsonCommand(const std::string& command);
    void        processPrintCommand(const std::string& command);
    void        processDumpCommand(const std::string& command);
    void        processCellAssignmentCommand(const std::string&     command,
                                             std::string::size_type equalsPos);
    void        processHelpCommand();
    void        processClearCommand();
    std::string trim(const std::string& str);
    void        processEditCommand(const std::string& command);
    void        processImportCommand(const std::string& command);
};
