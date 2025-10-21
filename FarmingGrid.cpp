// FarmingGrid.cpp 
#include "raylib.h"
#include <string>
#include <cstdio> // for snprintf

#include "Seed.h"
#include "Player.h"
#include "Season.h"
#include "Spring.h"
#include "Summer.h"
#include "Autumn.h"
#include "Winter.h"
#include "CropState.h"
#include "Planted.h"
#include "Harvestable.h"
#include "Harvested.h"
#include "Potato.h"
#include "Strawberry.h"
#include "Cauliflower.h"
#include "Tomato.h"
#include "Wheat.h"
#include "Eggplant.h"
#include "Carrot.h"
#include "Lettuce.h"
#include "Peas.h"
#include "Kale.h"
#include "Beetroot.h"
#include "Onion.h"
#include "Game.h"

//Defines crop behaviour in its respective seasons
struct CropAttributes {
    std::string name;      // inventory name
    int   thresholdUnlock; // points needed to unlock next crop
    int   totalPoints;     // points gained on harvest of each crop
    float growingTime;     // growth time
    Color color;           // visual while growing
};

struct Plot {
    Rectangle grid;           // draw rect (keep for GUI)
    Seed* plantedSeed;        // OOP: pointer to planted seed
    CropState* currentState;  // OOP: current crop state
    float timeRemaining;      // countdown until the crop is ready to be harvested

    void Clear() { 
        plantedSeed = nullptr; 
        currentState = nullptr; 
        timeRemaining = 0.0f; 
    }
    
    // Helper methods for GUI compatibility
    int getState() const {
        if (!plantedSeed) return 0;  // empty
        if (!currentState) return 0; // empty
        if (currentState->isDoneGrowing()) return 2; // ready
        return 1; // growing
    }
    
    int getCropIndex() const {
        if (!plantedSeed) return -1;
        // This will be determined by the season's seed order
        return -1; // Will be set by season logic
    }
};

struct GameState {
    // OOP Integration - Now using Game for encapsulation
    Game* gameController;  // OOP: Centralized game logic controller
    int   selectPlot;          // 0..3
    int   selectCrop;          // 0..2
    bool  allSeasonsCompleted; // after Winter is completed and all goals met
    
    // Constructor
    GameState() : gameController(nullptr), selectPlot(0), selectCrop(0), 
                  allSeasonsCompleted(false) {
            gameController = new Game();
    }
    
    // Destructor
    ~GameState() {
        delete gameController;
    }
    
    // Convenience getters that delegate to gameController
    Player* getPlayer() const { return gameController ? gameController->getPlayer() : nullptr; }
    Season* getCurrentSeason() const { return gameController ? gameController->getCurrentSeason() : nullptr; }
    int getSeasonIndex() const { return gameController ? gameController->getSeasonIndex() : 0; }
    float getSeasonTimeLeft() const { return gameController ? gameController->getSeasonTimeLeft() : 0.0f; }
    int getTotalPoints() const { return gameController ? gameController->getTotalPoints() : 0; }
    bool isCropUnlocked(int cropIndex) const { return gameController ? gameController->isCropUnlocked(cropIndex) : false; }
    bool isCropHarvested(int cropIndex) const { return gameController ? gameController->isCropHarvested(cropIndex) : false; }
};

int FARM_SCREEN_WIDTH  = 1000;
int FARM_SCREEN_HEIGHT = 640;
int TARGET = 60;
float SEASON_TIME_SECONDS = 60.0f;

namespace {

const char* SEASONS[4] = { 
    "Spring", 
    "Summer", 
    "Autumn", 
    "Winter" 
};

// Helper function to get crop attributes from OOP Season
std::vector<CropAttributes> GetSeasonCrops(GameState& game) {
    std::vector<CropAttributes> crops;
    
    if (!game.getCurrentSeason()) {
        return crops;
    }
    
    std::vector<Seed*>& seeds = game.getCurrentSeason()->getSeeds();
    for (int i = 0; i < 3 && i < (int)seeds.size(); i++) {
        Seed* seed = seeds[i];
        CropAttributes attr;
        attr.name = seed->get_Name();
        attr.thresholdUnlock = seed->getPointsUnlockThreshold();
        attr.totalPoints = seed->get_Value();
        attr.growingTime = seed->get_GrowTime();
        
        // Set colors based on crop type (keeping original GUI colors)
        if (attr.name == "Potato") attr.color = Color{180,220,180,255};
        else if (attr.name == "Strawberry") attr.color = Color{120,200,140,255};
        else if (attr.name == "Cauliflower") attr.color = Color{240,120,140,255};
        else if (attr.name == "Tomato") attr.color = Color{220,120,120,255};
        else if (attr.name == "Wheat") attr.color = Color{230,200,110,255};
        else if (attr.name == "Eggplant") attr.color = Color{160,220,160,255};
        else if (attr.name == "Carrot") attr.color = Color{255,180,100,255};
        else if (attr.name == "Lettuce") attr.color = Color{180, 60, 80,255};
        else if (attr.name == "Peas") attr.color = Color{230,170, 90,255};
        else if (attr.name == "Kale") attr.color = Color{120,180,140,255};
        else if (attr.name == "Beetroot") attr.color = Color{170,220,170,255};
        else if (attr.name == "Onion") attr.color = Color{160,200,160,255};
        else attr.color = Color{200,200,200,255}; // default
        
        crops.push_back(attr);
    }
    
    return crops;
}

//Color for the headers
Color GetSeasonColor(int seasonIndex) { 
    if (seasonIndex == 0){ 
        return Color{255,182,193,255}; //Pink 
    } else if (seasonIndex == 1){ 
        return GOLD; 
    } else if (seasonIndex == 2){ 
        return ORANGE; 
    } else if (seasonIndex == 3){ 
        return BLUE; 
    } else { 
        return BLACK; //when user wins 
    } 
}

} // end anonymous namespace

//Same from the Opening File
bool isMouseInside(Rectangle area) {

    //Built in library function that helps to determine the x and y coordinates of the mouse
    Vector2 position = GetMousePosition();

    // Determine if the mouse's x coordinate is within the rectangles width range
    bool inside_horizontal = false;
    if (position.x >= area.x && position.x <= area.x + area.width){
        inside_horizontal = true;
    }
    //Determine if the mouse's y coordinate is within the rectangles height range
    bool inside_vertical = false;
    if (position.y >= area.y && position.y <= area.y + area.height){
        inside_vertical = true;
    }
    
    //If both x and y are within the ranges of the rectangle, then it returns true and the mouse is within the rectangle
    if (inside_horizontal && inside_vertical){
        return true;
    } else {
        return false;
    }
}

bool isUsed(Rectangle btn) {
    //Check if the mouse is within the button area
    bool mouseInside = isMouseInside(btn);

    //Check if the mouse button was pressed
    bool mousePressed = false;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        mousePressed = true;
    }

    if (mouseInside && mousePressed){
        return true;
    } else {
        return false;
    }
}

//centers the text by finding the posotion and aliging
int centeredT(std::string& text, int fontSize, int screenWidth) {
    // Determine width of text in pixels
    int textWidth = MeasureText(text.c_str(), fontSize);

    // Determine the position of the text and place it in the center with equal space on either side
    int extraSpace = screenWidth - textWidth;
    int centered = extraSpace / 2;

    return centered; // return the x coordinate for centered text
}

bool cropUnlocked(GameState& game, int cropIndex) {
    // Delegate to gameController for OOP encapsulation
    return game.isCropUnlocked(cropIndex);
}

// beginner-style helper: check if all 3 crops are harvested
bool HarvestedAllThree(GameState& g) {
    // Delegate to gameController for OOP encapsulation
    return g.gameController && g.gameController->checkProgress();
}

void ResetSeason(GameState& game2, Plot plots[4]) {
    //clear the plots
    for (int i = 0; i < 4; i++) {
        plots[i].Clear();
    }
    
    //reset GUI selection
    game2.selectPlot = 0;
    game2.selectCrop = 0;
    
    // Delegate season reset to gameController for OOP encapsulation
    if (game2.gameController) {
        game2.gameController->resetSeason();
    }
}



void DrawOnePlot(const GameState&, const Plot& pPlot) {
    int middlex = (int)(pPlot.grid.x + pPlot.grid.width / 2);
    int middley = (int)(pPlot.grid.y + pPlot.grid.height / 2);

    //If the plot is empty, then color in with brown soil to look like farming plot
    if (pPlot.getState() == 0) {
        // EMPTY: light brown soil
        DrawRectangleRec(pPlot.grid, Color{210,180,140,255});  // tan brown shade

        // base border for visibility
        DrawRectangleLinesEx(pPlot.grid, 2.0f, BLACK);
        return;
    }
    //State 1, meaning the plot is growing, create a green-ish background with keyword GROW and the timer counting to display to
    // the player the time that remains
    if (pPlot.getState() == 1) {
        // GROWING: light green fill + labels
        DrawRectangleRec(pPlot.grid, Color{200, 235, 200, 180});

        //this is the font size number
        int fs = 22;

        //the word in the middle of each plot when the plant is growing
        char* growTxt = (char*)"GROW";

        int growWidth = MeasureText(growTxt, fs);

        // Finding the x and y positions so that the word grow remains centered within the plots
        int growT = (int)(pPlot.grid.x + (pPlot.grid.width - MeasureText(growTxt, fs)) / 2);
        int growY = (int)(pPlot.grid.y + pPlot.grid.height/2 - fs - 4);
        //ensure that the word grow is written in black
        DrawText(growTxt, growT, growY, fs, BLACK);

        // the amount of time left until the crop is ready to be harvested
        int secondsLeft = (int)(pPlot.timeRemaining + 0.5f);
        //String to display the seconds left
        std::string t = std::to_string(secondsLeft) + "s";
        //This is for the timer specifications, in dark green below the word GROW
        int timerX = (int)(pPlot.grid.x + (pPlot.grid.width - MeasureText(t.c_str(), fs)) / 2);
        DrawText(t.c_str(), timerX, growY + fs + 6, fs, DARKGREEN);

        // green highlight while growing
        DrawRectangleLinesEx(pPlot.grid, 3.0f, DARKGREEN);
        return;
    }

    //if the plot is READY (state 2), create a white background, red border and checkmark
    if (pPlot.getState() == 2) {
        // fill the plot with a light color to show it is ready
        DrawRectangleRec(pPlot.grid, Color{240,240,240,255});

        // draw a red outline to highlight it as “ready”
        DrawRectangleLinesEx(pPlot.grid, 2.0f, RED);

        // draw a gold circle in the middle of the plot
        int circleRadius = 18;
        DrawCircle(middlex, middley, 18, GOLD);

        // draw a green check mark (two lines) on top of the gold circle
        Vector2 lineA = {(float)middlex - 10, (float)middley +  2};
        Vector2 lineB = {(float)middlex -  2, (float)middley + 10};
        Vector2 lineC = {(float)middlex + 12, (float)middley -  6};
        DrawLineEx(lineA, lineB, 3.0f, DARKGREEN);
        DrawLineEx(lineB, lineC, 3.0f, DARKGREEN);

        // write the word “READY” at the bottom area of the plot once the plot is ready to be harvested
        int readyFontSize = 20;
        const char* readyWord = "READY";

        // center the word horizontally at the bottom
        int readyWidth = MeasureText(readyWord, readyFontSize);
        int readyTextX = (int)(pPlot.grid.x + (pPlot.grid.width - readyWidth) / 2);

        // place it a bit above the bottom edge of the plot
        int readyTextY = (int)(pPlot.grid.y + pPlot.grid.height - readyFontSize - 8);

        // draw the word “READY” in dark green
        DrawText(readyWord, readyTextX, readyTextY, readyFontSize, DARKGREEN);
    }
}


void DrawInventory(GameState& game3, Rectangle grid) {
    std::vector<CropAttributes> crops = GetSeasonCrops(game3);

     // draw the inventory grid and colors
    Color panelFill = Color{250, 250, 250, 255};   // very light gray/white
    DrawRectangleRec(grid, panelFill);
    float panelBorderThickness = 2.0f;
    DrawRectangleLinesEx(grid, panelBorderThickness, BLACK);

    // draw the panel title 
    int heading1 = (int)grid.x + 12;
    int heading2 = (int)grid.y + 10;
    int titleFont = 24;
    DrawText("Inventory", heading1, heading2, titleFont, BLACK);

    DrawRectangleRec(grid, Color{250,250,250,255});
    DrawRectangleLinesEx(grid, 2.0f, BLACK);
    DrawText("Inventory", (int)grid.x + 12, (int)grid.y + 10, 24, BLACK);

    float slotW = grid.width - 24.0f;
    float slotH = 64.0f;
    float gaps = 30.0f;

    // where the first row begins
    float firstRow  = grid.x + 12.0f;
    float firstRowTop   = grid.y + 48.0f;

    // we will draw 3 rows: i = 0, 1, 2
    float currentRowTop = firstRowTop;

    for (int i = 0; i < 3; i++) {
        // build the individuals sections in the inventory
        Rectangle section;
        section.width  = slotW;
        section.x      = firstRow;
        section.height = slotH;
        section.y      = currentRowTop;

        // check if crop is locked or unlocked
        bool isUnlocked = cropUnlocked(game3, i);

        // --- choose fill color depending on locked/unlocked ---
        Color slotFill;
        if (isUnlocked) {
            slotFill = Color{235, 245, 235, 255};  // light green if unlocked
        } else {
            slotFill = Color{230, 230, 230, 255};  // light gray if locked
        }
        DrawRectangleRec(section, slotFill);

        // For the border when locked or unlocked
        Color slotBorder;
        if (isUnlocked) {
            slotBorder = BLACK;
        } else {
            slotBorder = GRAY;
        }
        DrawRectangleLinesEx(section, 2.0f, slotBorder);

        // Create a label for increase of points i.e (Potato +10)
        if (i < (int)crops.size()) {
            std::string cropName = crops[i].name;
            int cropPointsValue  = crops[i].totalPoints;
            std::string rowLabel = cropName + " (+" + std::to_string(cropPointsValue) + ")";

        // Label centering and positioning
        int labelFont = 20;
        int labelX = (int)section.x + 10;
        // center-ish vertically: take row middle then shift up a bit by ~font half
        int labelY = (int)(section.y + (section.height / 2) - 10);

            // Drawing the label depening on color and positioning
            if (isUnlocked) {
                DrawText(rowLabel.c_str(), labelX, labelY, labelFont, BLACK);
            } else {
                DrawText(rowLabel.c_str(), labelX, labelY, labelFont, GRAY);
            }
        }

        // If unlocked, select this crop
        if (isUnlocked) {
            bool userClickedThis = isUsed(section);
            if (userClickedThis) {
                game3.selectCrop = i;  // remember which crop is selected
            }

            // if this crop is currently selected, use a thicker highlight
            if (game3.selectCrop == i) {
                DrawRectangleLinesEx(section, 4.0f, DARKBLUE);
            }
        }
        // If locked, display the points needed underneath each crop
        int neededPoints = crops[i].thresholdUnlock;
        if (!isUnlocked && neededPoints > 0) {
            int playerPointsNow = game3.getTotalPoints();

            // build something like "12 / 45 pts", determining and showing the earned and threshold difference
            std::string progressText = std::to_string(playerPointsNow) + " / " +
                                       std::to_string(neededPoints) + " pts";

            // put this near the bottom-left inside the slot
            int progressFont = 18;
            int progressX = (int)section.x + 10;
            int progressY = (int)(section.y + section.height - 10);
            DrawText(progressText.c_str(), progressX, progressY, progressFont, DARKGRAY);
        }

        // --- move down to the next row to avoid overlap ---
        float nextRowTop = section.y + section.height + gaps;
        currentRowTop = nextRowTop;
    }
}


void HandleMovement(GameState& game4) {
    int selectedRow = game4.selectPlot / 2; // 0 or 1, where top row = 0, and bottom row = 1
    int selectedCol = game4.selectPlot % 2; // 0 or 1, 0 = left and 1 = right

    //these conditional statements represent the navigation between the farming plots
    // that is done.
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { 
        if (selectedRow > 0){
             selectedRow--; 
        }
    }
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { 
        if (selectedRow< 1){
             selectedRow++; 
        }
    }
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) { 
        if (selectedCol > 0) {
            selectedCol--; 
        }
    }
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) { 
        if (selectedCol < 1) {
            selectedCol++; 
        }
    }

    // convert between row and column
    game4.selectPlot = selectedRow * 2 + selectedCol;
}

void Planting(GameState& game5, Plot& p) {
    // Delegate to gameController for OOP encapsulation
    if (game5.gameController) {
        game5.gameController->plantSeed(p, game5.selectCrop);
    }
}

void TryHarvest(GameState& game6, Plot& p) {
    // Delegate to gameController for OOP encapsulation
    if (game6.gameController) {
        game6.gameController->harvestSeed(p);
    }
}

void UpdateGrowth(GameState& game, Plot& p, float timeFrame) {
    // Delegate to gameController for OOP encapsulation
    if (game.gameController) {
        game.gameController->updateGrowth(p, timeFrame);
    }
}
// This is the code for the farming plots
void RunFarmingGrid() {
    InitWindow(FARM_SCREEN_WIDTH, FARM_SCREEN_HEIGHT, "Farming Grid");
    SetTargetFPS(TARGET);
    SetExitKey(KEY_NULL); // pressing ESC will NOT kill the whole app, only the current window

    // gridArea is where the 2x2 farm plots are located
    Rectangle gridA;
    gridA.x = 60;     // a bit from the left edge
    gridA.y = 120;    // a bit from the top
    gridA.width  = 520;
    gridA.height = 420;

    // inventory area dimensions on the right side that contains the crops
    Rectangle inventoryA;
    inventoryA.x = 620;     // placed to the right of the grid
    inventoryA.y = 120;     // align with grid vertically
    inventoryA.width  = 320;
    inventoryA.height = 380;

    Plot plots[4]; // creates 4 farming plots

     // how much space between the cells
    float gapCell = 20.0f;

    // each cell takes up half the grid width and height, minus a small gap
    float cellW  = (gridA.width  / 2.0f) - gapCell;
    float cellH = (gridA.height / 2.0f) - gapCell;

    // start placing the plots using nested loops
    int starting = 0;

    for (int row = 0; row < 2; row++) {      // two rows: 0 = top, 1 = bottom
        for (int col = 0; col < 2; col++) {  // two columns: 0 = left, 1 = right

            // work out where this plot should go
            float xPos = gridA.x + col * (cellW + gapCell);
            float yPos = gridA.y + row * (cellH + gapCell);

            // assign these values to the current plot to position it
            plots[starting].grid.x = xPos;
            plots[starting].grid.y = yPos;
            plots[starting].grid.width  = cellW;
            plots[starting].grid.height = cellH;

            // clear the plot so it starts empty
            plots[starting].Clear();

            // go to the next plot in the array
            starting += 1; // FIXED (was =+ 1)
        }
    }

    // --- now set up the game state variables ---
    GameState game7;

    // Initialize game using OOP controller
    if (game7.gameController) {
        game7.gameController->startGame();
    }

    // start on the first plot and first crop
    game7.selectPlot = 0;
    game7.selectCrop = 0;

    // we haven’t finished all seasons yet
    game7.allSeasonsCompleted = false;

    // make sure all crops start unharvested (handled by gameController)


    while (!WindowShouldClose()) {
        //Planting Section
        float remaining = GetFrameTime();

        bool pressedEscape = IsKeyPressed(KEY_ESCAPE);
        if (IsKeyPressed(KEY_ESCAPE)) {
            break; // return to menu
        }

        // Tick season timer - handled by gameController
        if (game7.gameController) {
            game7.gameController->updateTime(remaining);
        } 

        // Movement + actions
        HandleMovement(game7); // --> lets the user to move amongst the plots using WASD keys

        //plant on the desired plot once the SPACE key is pressed
        bool spaceBar = IsKeyPressed(KEY_SPACE);
        if (spaceBar) {
            int desiredPlot = game7.selectPlot;
            Planting(game7, plots[game7.selectPlot]);
        }
        //Harvest the plant from the desired plot
        bool H = IsKeyPressed(KEY_H);
        if (H) {
            int desiredHarvest = game7.selectPlot;
            TryHarvest(game7, plots[game7.selectPlot]);
        }

        //Update all plots so growing crops continue to countdown towards ready
        for (int i = 0; i < 4; i++) {
            UpdateGrowth(game7, plots[i], remaining);
        }

        // advance season if finished early, before the time ends
        if (game7.gameController && !game7.allSeasonsCompleted) {
            bool collectedAll = HarvestedAllThree(game7);
            float timeLeft = game7.getSeasonTimeLeft();
            
            // if we finished early (before timer ends) go to next season
            if (collectedAll && timeLeft > 0.0f) {
                game7.gameController->nextLevel();
                ResetSeason(game7, plots);
                
                // Start timer for the new season
                game7.gameController->startSeasonTimer();
                
                // Check if game is over
                if (game7.gameController->isGameOverStatus()) {
                    game7.allSeasonsCompleted = true;
                }
            }
        }

        // allow retry if time expired without finishing
        if (!game7.allSeasonsCompleted && game7.getSeasonTimeLeft() <= 0.0f && !HarvestedAllThree(game7)) {
            if (IsKeyPressed(KEY_ENTER)) ResetSeason(game7, plots);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Season heading (coloured) drawn once
        if (!game7.allSeasonsCompleted) {
            const char* seasonHeading = SEASONS[game7.getSeasonIndex()];
            //retrieve the respective color for each seasons heading
            Color seasonCol = GetSeasonColor(game7.getSeasonIndex());
            
            int headX = 24;
            int headY = 20;
            int titleFontSize = 36;
        
            DrawText(seasonHeading, 24, 20, 36, seasonCol);
            
        } else {
            //If all the seasons are done, output a congrats message saying all seasons are completed
            DrawText("All Seasons Completed!", 24, 20, 36, BLACK);
        }

        // Season timer (top-right)
        int screenW = GetScreenWidth();
        int tf = 28;
        //Round timer to nearest second
        int secondsLeft = (int)(game7.getSeasonTimeLeft() + 0.5f);
        std::string timeText = "Time: " + std::to_string(secondsLeft) + "s";

        int timeW = MeasureText(timeText.c_str(), tf);
        int timeX = screenW - timeW - 24;
        int timeY = 24;

        DrawText(timeText.c_str(), timeX, timeY, tf, BLACK);

        // Help text at the top of the grid
        DrawText("W/A/S/D move  |  Click crop to select  |  SPACE plant  |  H harvest  |  ENTER retry  |  ESC menu",
                 60, 80, 18, DARKGRAY);

        // Grid & plots with the border
        DrawRectangleLinesEx(gridA, 2.0f, BLACK);
        for (int i = 0; i < 4; i++) {
            bool selected = (i == game7.selectPlot);
            DrawOnePlot(game7, plots[i]);

            // Always draw a base border so all four plots have clear edges
            DrawRectangleLinesEx(plots[i].grid, 2.0f, BLACK);

            // If this plot is selected with WASD, show a dark-blue highlight (on top)
            if (selected) {
                DrawRectangleLinesEx(plots[i].grid, 4.0f, DARKBLUE);
            }
        }

        // Inventory panel
        DrawInventory(game7, inventoryA);

        // Total points display at bottom of screen
        int screenH = GetScreenHeight();
        int pointsFont = 24;
        std::string pointsText = "Total Points: " + std::to_string(game7.getTotalPoints());
        int pointsWidth = MeasureText(pointsText.c_str(), pointsFont);
        int pointsX = (GetScreenWidth() - pointsWidth) / 2; // Center horizontally
        int pointsY = screenH - 40; // Near bottom of screen
        
        // Draw background rectangle for points display
        Rectangle pointsBg = {(float)(pointsX - 10), (float)(pointsY - 5), (float)(pointsWidth + 20), (float)(pointsFont + 10)};
        DrawRectangleRec(pointsBg, Color{240, 240, 240, 255}); // Light gray background
        DrawRectangleLinesEx(pointsBg, 2.0f, BLACK); // Black border
        
        DrawText(pointsText.c_str(), pointsX, pointsY, pointsFont, BLACK);

        // Unlock thresholds for each crop in the inventory sections
        std::vector<CropAttributes> crops = GetSeasonCrops(game7);
        int y = (int)(inventoryA.y + inventoryA.height + 10);

        DrawText("Unlocks:", (int)inventoryA.x + 12, y, 18, BLACK);
        y += 24;
        for (int i = 0; i < 3 && i < (int)crops.size(); i++) {
            std::string row = crops[i].name + ": " + std::to_string(crops[i].thresholdUnlock) + " pts";
            DrawText(row.c_str(), (int)inventoryA.x + 12, y, 18, DARKGRAY);
            y += 20;
        }

        // Overlays, first for when the timer has ended and not all the crops have been unlocked, planted or harvested
        bool collectedAll = HarvestedAllThree(game7);
        if (game7.allSeasonsCompleted == false && game7.getSeasonTimeLeft() <= 0.0f && collectedAll == false) {
        std::string bigMessage   = "Time's up!";
        std::string smallMessage = "Press ENTER to retry this season, or ESC to return to the menu.";

        int sW = GetScreenWidth();
        int sH = GetScreenHeight();

        int boxTop = (sH / 2) - 80;
        int boxHeight = 140;

        // background box
        Color overlayColor = Color{255, 255, 255, 230}; // white with a bit of transparency
        DrawRectangle(0, boxTop, sW, boxHeight, overlayColor); // width = sW

        // border around the boxes of the inventory
        int boxLeft = 10;
        int boxWidth = sW - 20;
        DrawRectangleLines(boxLeft, boxTop, boxWidth, boxHeight, BLACK);

        // center the big text
        int bigFont = 36;
        int bigWidth = MeasureText(bigMessage.c_str(), bigFont);
        int bigX = (sW - bigWidth) / 2;
        int bigY = (sH / 2) - 40;
        DrawText(bigMessage.c_str(), bigX, bigY, bigFont, BLACK);

        // center the smaller text when it comes to the messages
        int smallFont = 22;
        int smallWidth = MeasureText(smallMessage.c_str(), smallFont);
        int smallX = (sW - smallWidth) / 2;
        int smallY = (sH / 2) + 2;
        DrawText(smallMessage.c_str(), smallX, smallY, smallFont, DARKGRAY);
    }

        //For when the game is completed, the final message that appears for the user on the screen
        if (game7.allSeasonsCompleted == true) {
            std::string big = "Congrats! All seasons completed.";
            std::string small = "Press ESC to return to menu, or ENTER to replay Spring.";

            int sW = GetScreenWidth();
            int sH = GetScreenHeight();

            int boxTop = (sH / 2) - 80;
            int boxHeight = 140;

            // white background area with a border
            Color overlayColor = Color{255, 255, 255, 230};
            DrawRectangle(0, boxTop, sW, boxHeight, overlayColor);
            DrawRectangleLines(10, boxTop, sW - 20, boxHeight, BLACK);

            // center the big text and the message
            int bigFont = 32;
            int bigWidth = MeasureText(big.c_str(), bigFont);
            int bigX = (sW - bigWidth) / 2;
            int bigY = (sH / 2) - 40;
            DrawText(big.c_str(), bigX, bigY, bigFont, BLACK);

            // center the small text with the message
            int smallFont = 20;
            int smallWidth = MeasureText(small.c_str(), smallFont);
            int smallX = (sW - smallWidth) / 2;
            int smallY = (sH / 2) + 2;
            DrawText(small.c_str(), smallX, smallY, smallFont, DARKGRAY);

            
            //If enter is pressed and all the seasons are completed everything resets
            if (IsKeyPressed(KEY_ENTER)) {
                game7.allSeasonsCompleted = false;
                if (game7.gameController) {
                    game7.gameController->startGame();
                }
                ResetSeason(game7, plots);
            }
        }

        EndDrawing();
    }

    CloseWindow();
}





