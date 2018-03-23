CC=g++
SRC_DIR := .
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BUILD_DIR := ./build
EXEC_NAME := demo

CFLAGS= -w -Wall -g -std=c++0x
LIBS= -std=c++0x -lPocoMySQL

all: $(EXEC_NAME)

$(OBJ_DIR)/demo.o: $(SRC_DIR)/demo.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CC) -c -o $@ $< $(CFLAGS)

demo: $(OBJ_FILES)
	$(CC) -g -o $(BUILD_DIR)/$@ $^ $(LIBS) $(LDOPTIONS)

.phony: all clean

clean:
	rm -f $(OBJ_DIR)/*.o *~ core $(INCDIR)/*~  $(BUILD_DIR)/$(EXEC_NAME)