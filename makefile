CXX = g++
CC = gcc

INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DOC_DIR = doc
LIB_DIR = libs

INC_SUBDIRS = formula_token formula_token/data_types operation operation/visitors 
CXXFLAGS = -Wall -pedantic -std=c++17 -g -I $(INC_DIR) $(addprefix -I $(INC_DIR)/, $(INC_SUBDIRS))
CCFLAGS = -I $(LIB_DIR)

# include lib folder 
SRC_FILES := $(wildcard $(SRC_DIR)/*/*/*.cpp $(SRC_DIR)/*/*.cpp $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(SRC_FILES))
EXE_FILE = $(BIN_DIR)/main

.PHONY: doc

all: $(EXE_FILE)

run: $(EXE_FILE)
	./$(EXE_FILE)

#docs in doc directory
doc: 
	doxygen Doxyfile

# Rule to compile linenoise.c
$(OBJ_DIR)/linenoise.o: $(LIB_DIR)/linenoise.c $(LIB_DIR)/linenoise.h
	$(CC) $(CCFLAGS) -c $< -o $@

$(EXE_FILE): $(OBJ_FILES) $(OBJ_DIR)/linenoise.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Create each .o file from its corresponding .cpp file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D) 
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(CXX) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<

clean:
	rm -rf $(OBJ_DIR)/*
	rm -f $(EXE_FILE)
	rm -rf $(DOC_DIR)/*

# include the dependency files
-include $(DEP_FILES)
