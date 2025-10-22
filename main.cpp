#include "raylib.h"
#include "FarmingGrid.h"


enum class Screen { 
    MENU, 
    GAME, 
    INSTRUCTIONS 
};

// run farming grid function
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

    // button navigation loop
    while (!WindowShouldClose()) {
       
        if (currentScreen == Screen::MENU) {

            // Start button is pressed
            if (isButtonClicked(btnStart.box)) {
                // close the menu window
                CloseWindow();                 // 1) close menu window
                // run the farming grid in a new window (switch)
                RunFarmingGrid();              // 2) open and run farming window

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




