SRC_DIR = ./src
DIST_DIR = ./dist
GLSL_DIR = $(SRC_DIR)/Graphics/Shaders
SRC_FILES = $(shell find $(SRC_DIR)/ -type f -name '*.cpp')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(DIST_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(patsubst %.o,%.d,$(OBJ_FILES))
GLSL_FILES = $(shell find $(GLSL_DIR)/ -type f -name '*.glsl')
COMPILED_GLSL_FILES = $(patsubst %.glsl,%.h,$(GLSL_FILES))

EXE = $(DIST_DIR)/index.js

CXX = emcc
CXXFLAGS = \
	-O0 \
	-Wall \
	-Werror \
	-Wextra \
	-pedantic-errors \
	-std=c++17 \
	-I$(SRC_DIR)/ \
	-I./lib/

EMCCFLAGS = \
	-s WASM=1 \
	-s MAX_WEBGL_VERSION=2 \
	-s FETCH=1 \
	-s USE_SDL=2 \
	-s USE_SDL_MIXER=2 \
	-s USE_SDL_IMAGE=2 \
	-s SDL2_IMAGE_FORMATS='["png"]' \
	-s USE_ZLIB=1 \
	-s MODULARIZE=1 \
	-s 'EXPORT_NAME="createFireWebEngine"'

.PHONY = default clean

default: $(EXE)

$(EXE): $(COMPILED_GLSL_FILES) $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(EMCCFLAGS) $(OBJ_FILES)

-include $(DEP_FILES)

$(DIST_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p "$(dir $@)"
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

$(GLSL_DIR)/%.h: $(GLSL_DIR)/%.glsl
	xxd -i $< > $@

clean:
	rm -rf $(DIST_DIR)/ $(COMPILED_GLSL_FILES)
