#ifndef FARMING_GRID_H
#define FARMING_GRID_H

#include "raylib.h"
#include <string>
#include <vector>

//This struct creates a clickable button on the screen and has text written on it
struct Button { 
    Rectangle box; //object diefines the buttons position and size on the screen
    std::string text; // text appears on the button
};

// Function declarations for design functions moved from main.cpp to FarmingGrid.cpp
bool MouseInside(Rectangle area);
bool isButtonClicked(Rectangle button);
int GetCenterText(std::string text, int fontSize, int screenWidth);
void DrawSimpleButton(Button b, int fontSize);
std::string ReadTextFile(std::string filePath);
std::vector<std::string> SplitLines(std::string instructions);

#endif // FARMING_GRID_H
