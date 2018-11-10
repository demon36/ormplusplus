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

CFLAGS= -w -Wall -g -std=c++0x -I$(INC_DIR)
LIBS= -std=c++0x -lPocoUtil -lPocoXML -lPocoJSON -lPocoNet -lPocoFoundation -lPocoData -lPocoDataSQLite -lPocoDataMySQL
EXEC_LIBS=$(LIBS) -L$(LIB_DIR) -l$(LIB_NAME)
LDOPTIONS= -Wl,-rpath='$$ORIGIN/../lib'

all: $(LIB_NAME) tests

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CC) -fPIC -c -o $@ $< $(CFLAGS)

tests: $(TEST_BIN_FILES)
	
$(BIN_DIR)/%: $(TESTS_DIR)/%.cpp
	$(CC) -g $< -o $@ $(CFLAGS) $(EXEC_LIBS) $(LDOPTIONS)

$(LIB_NAME): $(OBJ_FILES)
	$(CC) -shared -g -o $(LIB_DIR)/lib$@.so $^ $(LIBS)

.phony: all clean

clean:
	rm -f $(OBJ_DIR)/*.o $(LIB_DIR)/*.so $(BIN_DIR)/*