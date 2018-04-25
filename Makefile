CC=g++
SRC_DIR := ./src
OBJ_DIR := ./build
INC_DIR := ./include
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_DIR := ./bin
LIB_DIR := ./lib
EXEC_NAME := demo
LIB_NAME := ormplusplus
TEST_DIR := ./test

CFLAGS= -w -Wall -g -std=c++0x -I$(INC_DIR)
LIBS= -std=c++0x -lPocoUtil -lPocoXML -lPocoJSON -lPocoNet -lPocoFoundation -lPocoData -lPocoDataSQLite -lPocoDataMySQL
EXEC_LIBS=$(LIBS) -L$(LIB_DIR) -l$(LIB_NAME)
LDOPTIONS= -Wl,-rpath='$$ORIGIN/../lib'

all: $(EXEC_NAME)

$(OBJ_DIR)/demo.o: $(SRC_DIR)/demo.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CC) -fPIC -c -o $@ $< $(CFLAGS)

demo: $(LIB_NAME)
	$(CC) -g $(TEST_DIR)/$@.cpp -o $(BIN_DIR)/$@ $(CFLAGS) $(EXEC_LIBS) $(LDOPTIONS)

$(LIB_NAME): $(OBJ_FILES)
	$(CC) -shared -g -o $(LIB_DIR)/lib$@.so $^ $(LIBS)

.phony: all clean

clean:
	rm -f $(OBJ_DIR)/*.o $(LIB_DIR)/*.so $(BIN_DIR)/$(EXEC_NAME)