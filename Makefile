SRC_DIR = ./src
DIST_DIR = ./dist
SRC_FILES = $(shell find $(SRC_DIR)/ -type f -name '*.cpp')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(DIST_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(patsubst %.o,%.d,$(OBJ_FILES))

EXE = $(DIST_DIR)/index.js

CXX = emcc
CXXFLAGS = \
	-O3 \
	-Wall \
	-Werror \
	-Wextra \
	-pedantic-errors \
	-std=c++17 \
	-I$(SRC_DIR)/

EMCCFLAGS = \
	-s WASM=1 \
	-s MAX_WEBGL_VERSION=2 \
	-s FETCH=1 \
	-s MODULARIZE=1 \
	-s 'EXPORT_NAME="createFireWebEngine"'

.PHONY = default clean

default: $(EXE)

$(EXE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(EMCCFLAGS) $^

-include $(DEP_FILES)

$(DIST_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p "$(dir $@)"
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

clean:
	rm -rf $(DIST_DIR)/
