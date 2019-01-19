CC=g++
SRC_DIR := ./src
OBJ_DIR := ./build
INC_DIR := ./include
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_DIR := ./bin
LIB_DIR := ./lib
LIB_NAME := ormplusplus
TESTS_DIR := ./test
TEST_SRC_FILES := $(wildcard $(TESTS_DIR)/*.cpp)
TEST_BIN_FILES := $(patsubst $(TESTS_DIR)/%.cpp,$(BIN_DIR)/%,$(TEST_SRC_FILES))

#create needed directories if they do not exist
$(shell mkdir -p ./build)
$(shell mkdir -p ./bin)
$(shell mkdir -p ./lib)

CFLAGS := -w -Wall -g -std=c++11 -I$(INC_DIR)
LIBS := -std=c++11 -lPocoFoundation -lPocoData -lPocoDataMySQL
EXEC_LIBS := $(LIBS) -L$(LIB_DIR) -l$(LIB_NAME)
LDOPTIONS := -Wl,-zdefs,-rpath='$$ORIGIN/../lib'

all: $(LIB_NAME) $(TEST_BIN_FILES)

allcvrg: CFLAGS+=--coverage
allcvrg: LDOPTIONS+=--coverage
allcvrg: all

test: $(BIN_DIR)/major_tests
	$(BIN_DIR)/major_tests

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(BIN_DIR)/%: $(TESTS_DIR)/%.cpp $(wildcard $(INC_DIR)/*.h)
	$(CC) -g $< -o $@ $(CFLAGS) $(EXEC_LIBS) $(LDOPTIONS)

$(LIB_NAME): $(OBJ_FILES)
	$(CC) -shared -g -o $(LIB_DIR)/lib$@.so $^ $(LDOPTIONS) $(LIBS)

.PHONY: all clean allcvrg test

clean:
	rm -f $(OBJ_DIR)/*.o $(LIB_DIR)/*.so $(BIN_DIR)/*