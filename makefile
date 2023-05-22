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
SRC_FILES := $(wildcard $(SRC_DIR)/**/**/* $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
HDR_FILES := $(wildcard $(INC_DIR)/**/**/* $(INC_DIR)/**/*.hpp $(INC_DIR)/*.hpp)
SRC_FILES_C := $(wildcard $(LIB_DIR)/*.c)
HDR_FILES_C := $(wildcard $(LIB_DIR)/*.h)
# HDR_FILES := $(wildcard $(INC_DIR)/**/*.hpp $(INC_DIR)/*.hpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
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



$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDR_FILES) $(HDR_FILES_C) $(SRC_FILES_C)
# 	create directory if it doesn't exist
	@mkdir -p $(@D) 
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*
	rm -f $(EXE_FILE)
	rm -rf $(DOC_DIR)/*
