CC := g++
ARCH :=$(shell getconf LONG_BIT)
BUILD := debug
SRC_DIR := ./src
OBJ_DIR := ./obj/$(ARCH)/$(BUILD)
INC_DIR := ./include
BIN_DIR := ./bin/$(ARCH)/$(BUILD)
LIB_DIR := ./lib/$(ARCH)/$(BUILD)
DEP_DIR := ./dep
COV_REPORTS_DIR := ./cov
DOCS_DIR := ./docs
TEST_SRC_DIR := ./test
TEST_BIN_DIR := ./bin/test/$(ARCH)
PROJECT_NAME := $(shell basename $(CURDIR))
MAJOR_VERSION := 0
MINOR_VERSION := 1.9

SRC_FILES := $(shell find $(SRC_DIR) -regex '.*\.\(c\|cc\|cpp\|cxx\)')
OBJ_FILES := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/$(SRC_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)/%,$(DEP_DIR)/$(SRC_DIR)/%.dep,$(SRC_FILES)) $(patsubst $(TEST_SRC_DIR)/%,$(DEP_DIR)/$(TEST_SRC_DIR)/%.dep,$(TEST_SRC_FILES))
TEST_SRC_FILES := $(shell find $(TEST_SRC_DIR) -regex '.*\.\(c\|cc\|cpp\|cxx\)')
TEST_OBJ_FILES := $(patsubst $(TEST_SRC_DIR)/%,$(OBJ_DIR)/$(TEST_SRC_DIR)/%.o,$(TEST_SRC_FILES))

SO_FILE := $(PROJECT_NAME).so
A_FILE := $(PROJECT_NAME).a
EXEC_FILE := $(PROJECT_NAME)
TEST_FILE := main_test

CFLAGS := -m$(ARCH) -Wall -Wconversion -Werror -g -std=c++11 -I$(INC_DIR)
CFLAGS_DEBUG := -DDEBUG
CFLAGS_RELEASE := -O3
LIBS := #ex: -lthirdpary
LDFLAGS := -m$(ARCH)
SO_LDFLAGS := -shared -Wl,-zdefs,-soname,$(SO_FILE).$(MAJOR_VERSION),-rpath,'$$ORIGIN'
TEST_LDFLAGS := -L$(LIB_DIR) -l:$(SO_FILE) -Wl,-rpath,'$$ORIGIN/lib:$$ORIGIN/dep:$$ORIGIN/../../../$(LIB_DIR)'

ifeq ($(BUILD),debug)
	CFLAGS += $(CFLAGS_DEBUG)
else ifeq ($(BUILD),release)
	CFLAGS += $(CFLAGS_RELEASE)
	SO_DBG_FILE := $(SO_FILE).dbg
	A_DBG_FILE := $(A_FILE).dbg
	EXEC_DBG_FILE := $(EXEC_FILE).dbg
else ifeq ($(BUILD),coverage)
	CFLAGS += --coverage
	LDFLAGS += --coverage
else
$(error "allowed BUILD values are debug, release, coverage")
endif

all: shared test

shared: depend $(LIB_DIR)/$(SO_FILE) $(LIB_DIR)/$(SO_DBG_FILE)

static: $(LIB_DIR)/$(A_FILE)

exec: $(BIN_DIR)/$(EXEC_FILE) $(BIN_DIR)/$(EXEC_DBG_FILE)

test: shared $(TEST_BIN_DIR)/$(TEST_FILE)

run: test
	$(TEST_BIN_DIR)/$(TEST_FILE)
ifeq ($(BUILD),coverage)
	lcov --quiet -c --directory . --output-file $(OBJ_DIR)/.info --no-external
	genhtml --quiet $(OBJ_DIR)/.info --output-directory $(COV_REPORTS_DIR)
	xdg-open $(COV_REPORTS_DIR)/index.html
endif

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%
	@mkdir -p $(@D) $(DEP_DIR)/$(<D)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $< -MMD -MF $(DEP_DIR)/$<.dep

$(OBJ_DIR)/$(TEST_SRC_DIR)/%.o: $(TEST_SRC_DIR)/%
	@mkdir -p $(@D) $(DEP_DIR)/$(<D)
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF $(DEP_DIR)/$<.dep

$(LIB_DIR)/$(SO_FILE): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) -g -o $@.$(MAJOR_VERSION).$(MINOR_VERSION) $(OBJ_FILES) $(LDFLAGS) $(SO_LDFLAGS) $(LIBS)
	ln -sf ./$(SO_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(SO_FILE).$(MAJOR_VERSION)
	ln -sf ./$(SO_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(SO_FILE)

#install --prefix=./vendor
depend:
#	$(MAKE) -C $(DEP_LIB1) 
#for color in $(DEP_PROJ_DIRS); do \
#	$(MAKE) -C $(color) \
#done;  

$(LIB_DIR)/$(A_FILE): $(OBJ_FILES)
	@mkdir -p $(@D)
	ar rcs $@.$(MAJOR_VERSION).$(MINOR_VERSION) $^
	ln -sf ./$(A_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(A_FILE).$(MAJOR_VERSION)
	ln -sf ./$(A_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(A_FILE)

$(BIN_DIR)/$(EXEC_FILE): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) -g $? -o $@.$(MAJOR_VERSION).$(MINOR_VERSION) $(LDFLAGS) $(LIBS)
	ln -sf ./$(EXEC_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(EXEC_FILE).$(MAJOR_VERSION)
	ln -sf ./$(EXEC_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(EXEC_FILE)

%.dbg: %
	objcopy --only-keep-debug $< $@
	objcopy --strip-unneeded $< $<
	objcopy --add-gnu-debuglink=$< $@

$(TEST_BIN_DIR)/$(TEST_FILE): $(TEST_OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) -g $^ -o $@ $(LDFLAGS) $(TEST_LDFLAGS) $(LIBS) 

init:
	mkdir -p $(SRC_DIR) $(INC_DIR) $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR) $(DEP_DIR) $(TEST_SRC_DIR) $(TEST_BIN_DIR) $(DOCS_DIR)

clean:
	rm -f $(OBJ_FILES) $(TEST_OBJ_FILES) \
	$(LIB_DIR)/$(SO_FILE) $(SO_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(SO_FILE).$(MAJOR_VERSION) $(LIB_DIR)/$(SO_FILE).dbg \
	$(LIB_DIR)/$(A_FILE) $(A_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(A_FILE).$(MAJOR_VERSION) $(LIB_DIR)/$(A_FILE).dbg \
	$(BIN_DIR)/$(EXEC_FILE) $(EXEC_FILE).$(MAJOR_VERSION).$(MINOR_VERSION) $(LIB_DIR)/$(EXEC_FILE).$(MAJOR_VERSION) $(BIN_DIR)/$(EXEC_FILE).dbg \
	$(TEST_BIN_DIR)/$(TEST_FILE) $(DEP_FILES)

.PHONY: init all shared static exec run clean depend

-include $(shell test -d $(DEP_DIR) && find $(DEP_DIR) -name '*.dep')

#todo: add an extra target for the symlink itself
#todo: add target cleanall
#todo: add rebuild target
#todo: add section for other makefile deps
#todo: support pkg-config
#todo: support excluding source files from build
