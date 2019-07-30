CC := g++
SRC_DIR := ./src
OBJ_DIR := ./build
INC_DIR := ./include
BIN_DIR := ./bin
LIB_DIR := ./lib
DEP_DIR := ./dep
TESTS_DIR := ./test
PROJECT_NAME := $(shell basename $(CURDIR))

SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/$(SRC_DIR)/%.o,$(SRC_FILES))
TEST_SRC_FILES := $(shell find $(TESTS_DIR) -name '*.cpp')
TEST_OBJ_FILES := $(patsubst $(TESTS_DIR)/%.cpp,$(OBJ_DIR)/$(TESTS_DIR)/%.o,$(TEST_SRC_FILES))

SO_FILE := $(LIB_DIR)/$(PROJECT_NAME).so
A_FILE := $(LIB_DIR)/$(PROJECT_NAME).a
EXEC_FILE := $(BIN_DIR)/$(PROJECT_NAME)
TEST_FILE := $(BIN_DIR)/main_test

CFLAGS := -Wall -Wconversion -Werror -g -std=c++11 -I$(INC_DIR)
LIBS := -lPocoFoundation -lPocoData -lPocoDataMySQL
VERSION := 0.1
LIB_LDFLAGS := -shared -Wl,-zdefs,-soname,$(PROJECT_NAME).so.$(VERSION)

all: shared test

coverage: CVRG := --coverage
coverage: all run
	lcov --quiet -c --directory . --output-file $(OBJ_DIR)/.info --no-external
	#genhtml --quiet $(OBJ_DIR)/.info --output-directory html
	#xdg-open ./html/index.html

shared: $(SO_FILE)
static: $(A_FILE)
exec: $(EXEC_FILE)
test: $(TEST_FILE)
run: test
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIB_DIR) $(TEST_FILE)

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D) $(DEP_DIR)/$(<D)
	$(CC) $(CFLAGS) $(CVRG) -fPIC -c -o $@ $< -MMD -MF $(DEP_DIR)/$<.dep

$(OBJ_DIR)/$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.cpp
	@mkdir -p $(@D) $(DEP_DIR)/$(<D)
	$(CC) $(CFLAGS) $(CVRG) -c -o $@ $< -MMD -MF $(DEP_DIR)/$<.dep

$(SO_FILE): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $(CVRG) -g -o $(SO_FILE).$(VERSION) $^ $(LIB_LDFLAGS) $(LIBS)
	ln -sf $(CURDIR)/$(SO_FILE).$(VERSION) $(SO_FILE)

$(A_FILE): $(OBJ_FILES)
	@mkdir -p $(@D)
	ar rcs $@ $^

$(EXEC_FILE): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $(CVRG) -g $? -o $@ $(LIBS)

$(TEST_FILE): $(TEST_OBJ_FILES)
	@mkdir -p $(@D)
	$(eval LIB_FILES := $(shell find $(LIB_DIR) -regex ".*\.\(so\|a\)" -printf "%f" -quit))
	$(if $(LIB_FILES), $(eval EXEC_LIBS := -L$(LIB_DIR) -l:$(LIB_FILES)))
	$(CC) $(CVRG) -g $? -o $@ $(EXEC_LIBS) $(LIBS)

.PHONY: init all coverage shared static exec run clean

init:
	$(shell mkdir -p $(SRC_DIR) $(INC_DIR) $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR) $(DEP_DIR) $(TESTS_DIR))

clean:
	rm -rf $(OBJ_DIR)/* $(LIB_DIR)/* $(BIN_DIR)/* $(DEP_DIR)/*

-include  $(shell find $(DEP_DIR) -name '*.dep')
