# Makefile for Farming Game Project - Cross-Platform (Linux/macOS)

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
    CXXFLAGS = -std=c++11 -Wall
    RAYLIB_FLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
endif

# Source files for console version (excluding Raylib files)
CONSOLE_SOURCES = main.cpp Autumn.cpp Beetroot.cpp Carrot.cpp Cauliflower.cpp \
                  CropState.cpp Eggplant.cpp Game.cpp Harvestable.cpp Harvested.cpp \
                  Kale.cpp Lettuce.cpp Onion.cpp Peas.cpp Planted.cpp Player.cpp \
                  Potato.cpp Season.cpp Seed.cpp Spring.cpp Strawberry.cpp \
                  Summer.cpp Tomato.cpp Wheat.cpp Winter.cpp

# Source files for GUI version
GUI_SOURCES = RaylibOpeningSlide.cpp FarmingGrid.cpp

# Executables
CONSOLE_TARGET = console_farming_game
GUI_TARGET = gui_farming_game

# Default target
all: console gui

# Console version (text-based)
console: $(CONSOLE_TARGET)

$(CONSOLE_TARGET): $(CONSOLE_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# GUI version (Raylib-based)
gui: $(GUI_TARGET)

$(GUI_TARGET): $(GUI_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(RAYLIB_FLAGS)

# Clean up
clean:
	rm -f $(CONSOLE_TARGET) $(GUI_TARGET) *.o

# Run console version
run-console: console
	./$(CONSOLE_TARGET)

# Run GUI version
run-gui: gui
	./$(GUI_TARGET)

# macOS-specific targets
install-deps-mac:
	@echo "Installing Raylib via Homebrew..."
	brew install raylib

check-deps-mac:
	@echo "Checking if Raylib is installed..."
	@if brew list raylib >/dev/null 2>&1; then \
		echo "✅ Raylib is installed"; \
		echo "Raylib path: $$(brew --prefix raylib)"; \
	else \
		echo "❌ Raylib not found. Run 'make install-deps-mac' to install it."; \
	fi

# Show current OS and settings
info:
	@echo "Detected OS: $(UNAME_S)"
	@echo "Compiler: $(CXX)"
	@echo "C++ Standard: $(CXXFLAGS)"
	@echo "Raylib flags: $(RAYLIB_FLAGS)"

.PHONY: all console gui clean run-console run-gui install-deps-mac check-deps-mac info
