# Compiler
CXX = em++

# Compile flags
CXXFLAGS = -O3 -Iinclude -flto -fno-exceptions -fno-rtti -DNDEBUG -msimd128

# Link flags
LDFLAGS = \
  -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=8 \
  -sWASM=1 \
  -sEXPORT_NAME="engine" \
  -sMODULARIZE=1 \
  -sENVIRONMENT=web \
  -sEXPORTED_FUNCTIONS="['_getBestMove', '_malloc', '_free']" \
  -sEXPORTED_RUNTIME_METHODS="['ccall','cwrap']" \
  -sALLOW_MEMORY_GROWTH=1 \
  -sFILESYSTEM=0 \
  -sWASM_BIGINT=1 \
  -flto

# Target output files
TARGET = engine
OUTFILES = $(TARGET).js $(TARGET).wasm

# Source and build directories
SRC_DIR = src
BUILD_DIR = build

# List all source files (relative to Makefile)
SRCS = \
  $(SRC_DIR)/wasm_main.cpp \
  $(SRC_DIR)/chengine/chengine.cpp \
  $(SRC_DIR)/chengine/minMaxTree.cpp \
  $(SRC_DIR)/chengine/pawnStructureEvaluator.cpp \
  $(SRC_DIR)/chengine/quiesceSearch.cpp \
  $(SRC_DIR)/chengine/staticBoardAnalyzer.cpp \
  $(SRC_DIR)/chengine/theoryEvaluator.cpp \
  $(SRC_DIR)/board.cpp \
  $(SRC_DIR)/identifier.cpp \
  $(SRC_DIR)/moveGetter.cpp \
  $(SRC_DIR)/squareAttacker.cpp \
  $(SRC_DIR)/pieces/bishop.cpp \
  $(SRC_DIR)/pieces/knight.cpp \
  $(SRC_DIR)/pieces/king.cpp \
  $(SRC_DIR)/pieces/pawn.cpp \
  $(SRC_DIR)/pieces/rook.cpp \
  $(SRC_DIR)/pieces/queen.cpp 

# Create corresponding object file list in build/
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default build target
all: $(OUTFILES)

# Link object files to final .js/.wasm outputs
$(OUTFILES): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET).js

# Compile each .cpp into a .o, creating intermediate dirs as needed
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir $(subst /,\,$(dir $@)) 2>nul || exit 0
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean outputs
clean:
	del /q /f $(TARGET).js $(TARGET).wasm 2>nul || exit 0
	rmdir /s /q $(BUILD_DIR) 2>nul || exit 0

.PHONY: all clean
