#include "raylib.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

//This struct creates a clickable button on the screen and has text written on it
struct Button { 
    Rectangle box; //object diefines the buttons position and size on the screen
    std::string text; // text appears on the button
};

//Function determines whether the mouse is hovering within the area of the button. To ensure
//accuracy so that the program knows when to register a click.
bool MouseInside(Rectangle area) {

    //Built in library function that helps to determine the x and y coordinates of the mouse
    Vector2 position = GetMousePosition();

    // Determine if the mouse's x coordinate is within the rectangles width range
    bool inside_horizontal = false;
    if (position.x >= area.x && position.x <= area.x + area.width){
        inside_horizontal = true;
    }
    //Determine if the mouse's y coordinate is within the rectangles height range
    bool inside_vertical = false;
    if(position.y >= area.y && position.y <= area.y + area.height){
        inside_vertical = true;
    }
    
    //If both x and y are within the ranges of the rectangle, then it returns true and the mouse is within the rectangle
    if(inside_horizontal && inside_vertical){
        return true;
    }else{
        return false;
    }

}

// Returns true if the mouse is within the button and it was pressed
bool isButtonClicked(Rectangle button) {
    //Check if the mouse is within the button area
    bool mouseInside = MouseInside(button);

    //Check if the mouse button was pressed
    bool mousePressed = false;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        mousePressed = true;
    }

    if(mouseInside && mousePressed){
        return true;
    }else{
        return false;
    }
}

int GetCenterText(std::string text, int fontSize, int screenWidth) {
    //Determine width of text in pixels
    int textWidth = MeasureText(text.c_str(), fontSize);

    //Determine the position of the text and place it in the center with equal space on either side
    int extraSpace = screenWidth - textWidth;
    int centered = extraSpace/2;
   
   return centered;
}
//Function for drawing and coloring a buttton
void DrawSimpleButton(Button b, int fontSize) {
    Color fill;
    
    if (MouseInside(b.box)){
        fill = Color{102,191,255,255}; // fill with skyblue when mouse hovers over button otherwise fill with white

    }else{
        fill = RAYWHITE;
    }
    DrawRectangleRec(b.box, fill);
    DrawRectangleLinesEx(b.box, 2.0f, BLACK);
    
    //Checking the top, bottom and space on the sides to ensure that the text and button is centered properly
    int textWidth = MeasureText(b.text.c_str(), fontSize);
    int textSpace = b.box.x + (b.box.width - textWidth)/2;
    int textSpace2 = b.box.y + (b.box.height - fontSize) / 2;

    DrawText(b.text.c_str(), textSpace, textSpace2, fontSize, BLACK);
}

//Reads the instructions file and returns all the text inside like one really big string
std::string ReadTextFile(std::string filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()){

        return "instructions.txt not found.";
    }
    std::string line;
    std::string allContent;
    while (std::getline(file, line)) {
        allContent += line + "\n";
    }
        return allContent;
}
//Function splits the big string into a vector with one element per vector so that each line is individually displayed
std::vector<std::string> SplitLines(std::string instructions) {

    std::vector<std::string> lines;
    std::stringstream buffer(instructions);
    std::string line;

    while (std::getline(buffer, line)) {
        lines.push_back(line);
    }
    return lines;
}


enum class Screen { 
    MENU, 
    GAME, 
    INSTRUCTIONS 
};

// Forward declaration: implemented in FarmingGrid.cpp
void RunFarmingGrid();

int main() {
    // Window (MENU)
    int screenWidth = 900;
    int screenHeight = 550;
    //Title of the window
    InitWindow(screenWidth, screenHeight, "Farm Game (Menu)");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // ESC won't immediately kill the window, player has control then

    Screen currentScreen = Screen::MENU;

    int titleFont = 48;
    int buttonFont = 24;
    int buttonW = 260;
    int buttonH = 52;
    int gap = 16;
    float x = (screenWidth - buttonW) / 2.0f;
    float y = 200.0f;

    //Start button, contains the size, the position and the label for each one
    Button btnStart { {x, y + 0*(buttonH+gap), (float)buttonW, (float)buttonH}, "START" };
    //Instructions button
    Button btnInstruct { {x, y + 1*(buttonH+gap), (float)buttonW, (float)buttonH}, "INSTRUCTIONS" };
    //Quit button
    Button btnQuit  { {x, y + 2*(buttonH+gap), (float)buttonW, (float)buttonH}, "EXIT" };

    std::string instructionsFile = ReadTextFile("instructions.txt");
    std::vector<std::string> instructionLines = SplitLines(instructionsFile);

    //Create the back button to navigate the menu and place small rectangle in top-left corner
    Button backButton;
    backButton.box = { 20, 20, 140, 50};
    backButton.text = "BACK";

    // -------- main loop for button navigation --------
    while (!WindowShouldClose()) {
        // --- UPDATE ---
        if (currentScreen == Screen::MENU) {

            // Start button is pressed
            if (isButtonClicked(btnStart.box)) {
                // close the menu window
                CloseWindow();                 // 1) close menu window
                // run the farming grid in a new window (switch)
                RunFarmingGrid();              // 2) open & run farming window

                //Re-open the menu window or next window as game progresses
                InitWindow(screenWidth, screenHeight, "Farm Game (Menu)"); // 3) recreate menu
                SetTargetFPS(60);
                SetExitKey(KEY_NULL);
                //Return to menu screen
                currentScreen = Screen::MENU;
                continue; // skip drawing in this frame
            }

            //Instructions button is clicked
            if (isButtonClicked(btnInstruct.box)) {
                //Read the text from the instructions file
                instructionsFile = ReadTextFile("instructions.txt");
                //Split the lines into the drawing
                instructionLines = SplitLines(instructionsFile);

                currentScreen = Screen::INSTRUCTIONS;
            }
            //if exit button is pressed the game quits and the window closes
            if (isButtonClicked(btnQuit.box)) {
                break;
            }
        }

        else if (currentScreen == Screen::INSTRUCTIONS) {
            // if back button is pressed return to main menu
            if (isButtonClicked(backButton.box)) {
                currentScreen = Screen::MENU;
            }
        }

        // Create the drawings for the farm game buttons
        BeginDrawing();
        //Background for every window is white
        ClearBackground(RAYWHITE);

        //Draw a big title that is centered at the top
        if (currentScreen == Screen::MENU) {
            int heading = GetCenterText("FARM GAME", titleFont, screenWidth);
            int headingSize = 70;
            DrawText("FARM GAME", heading, headingSize, titleFont, BLACK);
    
            //Implement the 3 menu buttons
            DrawSimpleButton(btnStart, buttonFont);
            DrawSimpleButton(btnInstruct, buttonFont);
            DrawSimpleButton(btnQuit,  buttonFont);
        }
        else if (currentScreen == Screen::INSTRUCTIONS) {
            //Back button
            DrawSimpleButton(backButton, buttonFont);
            //Instructions title that is centered at the top
            int instructTitle = GetCenterText("INSTRUCTIONS", 36, screenWidth);
            int instructTitle2 = 30;
            DrawText("INSTRUCTIONS", instructTitle, instructTitle2, 36, BLACK);

            //Where to start drawing
            int leftMargin = 80;
            int topMargin = 120;
            int lineHeight = 22;

            int h = screenHeight - topMargin - 30;
            int l = h/lineHeight;
            //Outline the text for the lines and how many lines can fit on the screen
            for (int i = 0; i < l && i < (int)instructionLines.size(); i++) {
                int x = leftMargin;
                int y = topMargin + i * lineHeight;
                    DrawText(instructionLines[i].c_str(), x, y, 20, BLACK);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}




