

# Detect OS
UNAME_S := $(shell uname -s)

# Compiler and flags based on OS
ifeq ($(UNAME_S),Darwin)
    # macOS settings
    CXX = clang++
    CXXFLAGS = -std=c++17 -Wall
    RAYLIB_INCLUDE = -I$(shell brew --prefix raylib)/include
    RAYLIB_LIB = -L$(shell brew --prefix raylib)/lib
    RAYLIB_FLAGS = $(RAYLIB_INCLUDE) $(RAYLIB_LIB) -lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
else
    # Linux settings
    CXX = g++
    CXXFLAGS = -std=c++17 -Wall
    RAYLIB_FLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
endif



# Source files for GUI version
GUI_SOURCES = RaylibOpeningSlide.cpp FarmingGrid.cpp

# Executables
CONSOLE_TARGET = console_farming_game
GUI_TARGET = gui_farming_game

# Default target
all: console gui



# GUI version (Raylib-based)
gui: $(GUI_TARGET)

$(GUI_TARGET): $(GUI_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(RAYLIB_FLAGS)

# Clean up
clean:
	rm -f $(CONSOLE_TARGET) $(GUI_TARGET) *.o



# Run GUI version
run-gui: gui
	./$(GUI_TARGET)


