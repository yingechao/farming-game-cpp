// FarmingGrid.cpp – self-contained farming window
#include "raylib.h"
#include <string>
#include <cstdio>   // for snprintf (optional)

// ------------------------------ CONSTANTS ------------------------------

// Window size for the farming screen
const int FARM_SCREEN_WIDTH  = 1000;
const int FARM_SCREEN_HEIGHT = 640;

// Frames per second (how often the screen updates)
const int TARGET_FPS = 60;

// Game rules
const int NUMBER_OF_PLOTS  = 4;  // 2x2 grid

// How long one season lasts (in seconds). You can change this.
const float SEASON_TIME_SECONDS = 60.0f;

// ------------------------------ DATA TYPES -----------------------------

// A single crop type (for one season)
struct CropDef {
    std::string name;   // name to show in the inventory
    int unlockPoints;   // how many points you need to unlock this crop
    int yieldPoints;    // how many points you get when you harvest it
    float growSeconds;  // how long it takes to grow
    Color color;        // a color used while it is growing (for visuals)
};

// One grid square (plot) where a crop can be planted
struct Plot {
    Rectangle rect;  // where it is drawn
    int state;       // 0 = Empty, 1 = Growing, 2 = Ready to harvest
    int cropIndex;   // which crop is planted here (0..2) or -1 for none
    float timeLeft;  // time left to grow (only used when state == 1)

    void Clear() {
        state = 0;
        cropIndex = -1;
        timeLeft = 0.0f;
    }
};

// Game state while we are on the farming screen
struct GameState {
    int seasonIndex;                   // 0..3 (Spring, Summer, Autumn, Winter)
    float seasonTimeLeft;              // countdown timer for the season
    int totalPoints;                   // points earned THIS season
    bool harvested[3];   // which crops have been harvested this season
    int selectedPlot;                  // which plot the player has selected (0..3)
    int selectedCrop;                  // which crop is selected in the inventory (0..2)
    bool allSeasonsCompleted;          // true after Winter is completed
};

// ------------------------------ SEASONS -------------------------------

enum Season { SPRING = 0, SUMMER = 1, AUTUMN = 2, WINTER = 3 };

const char* SEASON_NAMES[4] = {
    "Spring", "Summer", "Autumn", "Winter"
};

// You can adjust these numbers later to match your repository values.
// These are simple, readable starter values.
const CropDef SPRING_CROPS[3] = {
    { "Cauliflower",      0, 10, 5.0f,  Color{180,220,180,255} },
    { "Potato",       25, 18, 6.0f,  Color{120,200,140,255} },
    { "Strawberry",60, 30, 8.0f,  Color{240,120,140,255} }
};

const CropDef SUMMER_CROPS[3] = {
    { "Tomato",      0, 12, 5.5f, Color{220,120,120,255} },
    { "Wheat",      30, 22, 7.0f,  Color{230,200,110,255} },
    { "Eggplant",70, 42, 9.5f,  Color{160,220,160,255} }
};

const CropDef AUTUMN_CROPS[3] = {
    { "Carrot",      0, 10, 5.0f,  Color{255,180,100,255} },
    { "Lettuce",  28, 20, 7.0f,  Color{180, 60, 80,255} },
    { "Peas",   65, 40, 10.0f, Color{230,170,90,255} }
};

const CropDef WINTER_CROPS[3] = {
    { "Kale",        0, 10, 6.0f,  Color{120,180,140,255} },
    { "Beetroot",      25, 20, 7.5f,  Color{170,220,170,255} },
    { "Onion",   60, 36, 9.5f,  Color{160,200,160,255} }
};

// Return a pointer to the 3 crops for the given season
const CropDef* GetSeasonCrops(int seasonIndex) {
    if (seasonIndex == SPRING)  return SPRING_CROPS;
    if (seasonIndex == SUMMER)  return SUMMER_CROPS;
    if (seasonIndex == AUTUMN)  return AUTUMN_CROPS;
    /* else */                  return WINTER_CROPS;
}

// --------------------------- SMALL HELPERS ----------------------------

// Simple “mouse inside rectangle” check
bool MouseInsideFarming(Rectangle r) {
    Vector2 m = GetMousePosition();
    if (m.x < r.x) return false;
    if (m.x > r.x + r.width) return false;
    if (m.y < r.y) return false;
    if (m.y > r.y + r.height) return false;
    return true;
}

// Did the user click inside r with the left mouse button this frame?
bool WasClicked(Rectangle r) {
    if (MouseInsideFarming(r) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return true;
    }
    return false;
}

// Center a piece of text horizontally on the screen
int CenterX(const std::string& text, int fontSize, int screenWidth) {
    int textWidth = MeasureText(text.c_str(), fontSize);
    return (screenWidth - textWidth) / 2;
}

// Is a crop unlocked yet?
bool IsCropUnlocked(const GameState& gs, int cropIndex) {
    const CropDef* crops = GetSeasonCrops(gs.seasonIndex);
    if (gs.totalPoints >= crops[cropIndex].unlockPoints) return true;
    return false;
}

// Did we harvest all three crops this season?
bool HarvestedAllThree(const GameState& gs) {
    if (gs.harvested[0] && gs.harvested[1] && gs.harvested[2]) return true;
    return false;
}

// Reset plots and season state when a new season starts (or on retry)
void ResetSeason(GameState& gs, Plot plots[NUMBER_OF_PLOTS]) {
    for (int i = 0; i < NUMBER_OF_PLOTS; i++) {
        plots[i].Clear();
    }
    gs.seasonTimeLeft = SEASON_TIME_SECONDS;

    for (int i = 0; i < 3; i++) {
        gs.harvested[i] = false;
    }

    gs.selectedPlot = 0;
    gs.selectedCrop = 0;

    // Points reset each season so unlocks are per-season (as per your spec)
    gs.totalPoints = 0;
}

// ------------------------------- DRAWING -------------------------------

// Draw one plot (tile). Show different visuals based on state.
void DrawOnePlot(const GameState& gs, const Plot& p, bool isSelected) {
    // Base tile
    DrawRectangleRec(p.rect, Color{240,240,240,255});

    // Border color: green if empty, red if occupied
    Color border = (p.state == 0) ? GREEN : RED;
    DrawRectangleLinesEx(p.rect, isSelected ? 4.0f : 2.0f, border);

    // Center point for drawing “symbols”
    int cx = (int)(p.rect.x + p.rect.width / 2);
    int cy = (int)(p.rect.y + p.rect.height / 2);

    if (p.state == 0) {
        // EMPTY: show a plus with two lines
        DrawLineEx({p.rect.x + p.rect.width/2, p.rect.y + 20},
                   {p.rect.x + p.rect.width/2, p.rect.y + p.rect.height - 20}, 3.0f, DARKGREEN);
        DrawLineEx({p.rect.x + 20, p.rect.y + p.rect.height/2},
                   {p.rect.x + p.rect.width - 20, p.rect.y + p.rect.height/2}, 3.0f, DARKGREEN);
    }
    else if (p.state == 1) {
        // GROWING: light green fill + "GROW" label + remaining seconds
        DrawRectangleRec(p.rect, Color{200, 235, 200, 180});

        const int fs = 22;
        const char* growTxt = "GROW";
        int tx = (int)(p.rect.x + (p.rect.width - MeasureText(growTxt, fs)) / 2);
        int ty = (int)(p.rect.y + p.rect.height/2 - fs - 4);
        DrawText(growTxt, tx, ty, fs, BLACK);

        // If you track remaining time in p.timeLeft:
        int secLeft = (int)(p.timeLeft + 0.5f);
        std::string t = std::to_string(secLeft) + "s";
        int tx2 = (int)(p.rect.x + (p.rect.width - MeasureText(t.c_str(), fs)) / 2);
        DrawText(t.c_str(), tx2, ty + fs + 6, fs, DARKGREEN);
    }
    else if (p.state == 2) {
        // READY: draw a gold circle and a checkmark composed of two lines
        DrawCircle(cx, cy, 18, GOLD);
        // Checkmark (two lines)
        Vector2 a = {(float)cx - 10, (float)cy + 2};
        Vector2 b = {(float)cx - 2,  (float)cy + 10};
        Vector2 c = {(float)cx + 12, (float)cy - 6};
        DrawLineEx(a, b, 3.0f, DARKGREEN);
        DrawLineEx(b, c, 3.0f, DARKGREEN);

        // Also draw text "READY" below
        const int fs = 20;
        const char* rtxt = "READY";
        int tx = (int)(p.rect.x + (p.rect.width - MeasureText(rtxt, fs)) / 2);
        int ty = (int)(p.rect.y + p.rect.height - fs - 8);
        DrawText(rtxt, tx, ty, fs, DARKGREEN);
    }
}

// Draw the inventory panel on the right with three crop slots.
// Each locked crop shows a progress bar underneath that no longer overlaps.
void DrawInventory(GameState& gs, Rectangle panel) {
    const CropDef* crops = GetSeasonCrops(gs.seasonIndex);

    // Panel background + border
    DrawRectangleRec(panel, Color{250,250,250,255});
    DrawRectangleLinesEx(panel, 2.0f, BLACK);
    DrawText("Inventory", (int)panel.x + 12, (int)panel.y + 10, 24, BLACK);

    // Layout constants
    float slotWidth   = panel.width - 24.0f;
    float slotHeight  = 64.0f;
    float barHeight   = 12.0f;
    float barGap      = 6.0f;    // space between slot and its progress bar
    float gapBetweenCrops = 30.0f; // extra space after each crop section

    float startX = panel.x + 12.0f;
    float startY = panel.y + 48.0f;

    for (int i = 0; i < 3; i++) {
        Rectangle slot = { startX, startY, slotWidth, slotHeight };
        bool unlocked = IsCropUnlocked(gs, i);

        // --- Draw the crop slot ---
        DrawRectangleRec(slot, unlocked ? Color{235,245,235,255} : Color{230,230,230,255});
        DrawRectangleLinesEx(slot, 2.0f, unlocked ? BLACK : GRAY);

        // Text label
        std::string label = crops[i].name + " (+" + std::to_string(crops[i].yieldPoints) + ")";
        DrawText(label.c_str(), (int)slot.x + 10, (int)(slot.y + slot.height / 2 - 10),
                 20, unlocked ? BLACK : GRAY);

        // Handle selection if unlocked
        if (unlocked) {
            if (WasClicked(slot)) {
                gs.selectedCrop = i;
            }
            if (gs.selectedCrop == i) {
                DrawRectangleLinesEx(slot, 4.0f, DARKBLUE);
            }
        }

        // --- Progress bar section ---
        if (!unlocked && crops[i].unlockPoints > 0) {
            // Bar background + border
            float barY = slot.y + slot.height + barGap;
            Rectangle bar = { startX, barY, slotWidth, barHeight };
            DrawRectangleRec(bar, Color{230,230,230,255});
            DrawRectangleLinesEx(bar, 2.0f, BLACK);

            // Fill amount
            float percent = (float)gs.totalPoints / (float)crops[i].unlockPoints;
            if (percent < 0) percent = 0;
            if (percent > 1) percent = 1;
            Rectangle fill = { bar.x, bar.y, bar.width * percent, bar.height };
            DrawRectangleRec(fill, DARKGREEN);

            // Progress numbers
            std::string progressText =
                std::to_string(gs.totalPoints) + " / " + std::to_string(crops[i].unlockPoints);
            DrawText(progressText.c_str(), (int)bar.x + 4, (int)bar.y - 18, 18, DARKGRAY);

            // Move Y below bar
            startY = barY + barHeight + gapBetweenCrops;
        } else {
            // If crop unlocked, just move Y below slot
            startY = slot.y + slot.height + gapBetweenCrops;
        }
    }
}


// ------------------------------ LOGIC --------------------------------

// Move the selected plot with W, A, S, D or arrow keys
void HandleMovement(GameState& gs) {
    int row = gs.selectedPlot / 2; // 0 or 1
    int col = gs.selectedPlot % 2; // 0 or 1

    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        if (row > 0) row = row - 1;
    }
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        if (row < 1) row = row + 1;
    }
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
        if (col > 0) col = col - 1;
    }
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        if (col < 1) col = col + 1;
    }

    gs.selectedPlot = row * 2 + col; // convert back to 0..3
}

// Try to plant the selected crop on the selected plot
void TryPlant(GameState& gs, Plot& p) {
    const CropDef* crops = GetSeasonCrops(gs.seasonIndex);

    // Can only plant on an empty plot
    if (p.state != 0) return;

    // Must have a valid selected crop (0..2)
    if (gs.selectedCrop < 0 || gs.selectedCrop > 2) return;

    // Crop must be unlocked
    if (!IsCropUnlocked(gs, gs.selectedCrop)) return;

    // Start growing
    p.state     = 1;
    p.cropIndex = gs.selectedCrop;
    p.timeLeft  = crops[p.cropIndex].growSeconds;
}

// Try to harvest the selected plot
void TryHarvest(GameState& gs, Plot& p) {
    const CropDef* crops = GetSeasonCrops(gs.seasonIndex);

    // Can only harvest if it is ready
    if (p.state != 2) return;
    if (p.cropIndex < 0 || p.cropIndex > 2) return;

    // Add points
    gs.totalPoints += crops[p.cropIndex].yieldPoints;

    // Mark this crop as harvested for the season
    gs.harvested[p.cropIndex] = true;

    // Clear the plot
    p.Clear();
}

// Update grow timer for one plot
void UpdateGrowth(Plot& p, float dt) {
    if (p.state == 1) { // only while growing
        p.timeLeft = p.timeLeft - dt;
        if (p.timeLeft <= 0.0f) {
            p.timeLeft = 0.0f;
            p.state = 2; // ready
        }
    }
}

// ------------------------------ ENTRY --------------------------------

// This function owns its own window. The menu should close its window
// before calling this, and recreate the menu window after this returns.
void RunFarmingGrid() {
    // Create the farming screen window
    InitWindow(FARM_SCREEN_WIDTH, FARM_SCREEN_HEIGHT, "Farming Grid");
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_NULL); // pressing ESC will NOT kill the whole app

    // Layout areas for drawing
    Rectangle gridArea = { 60, 120, 520, 420 };  // left side, 2x2 plots
    Rectangle invArea  = { 620, 120, 320, 380 }; // right side, inventory

    // Build the 4 plot rectangles (2 by 2 with a gap)
    Plot plots[NUMBER_OF_PLOTS];

    float cellWidth  = gridArea.width  / 2.0f - 20.0f;
    float cellHeight = gridArea.height / 2.0f - 20.0f;

    plots[0].rect = { gridArea.x,                 gridArea.y,                 cellWidth, cellHeight };
    plots[1].rect = { gridArea.x + cellWidth + 20.0f, gridArea.y,                 cellWidth, cellHeight };
    plots[2].rect = { gridArea.x,                 gridArea.y + cellHeight + 20.0f, cellWidth, cellHeight };
    plots[3].rect = { gridArea.x + cellWidth + 20.0f, gridArea.y + cellHeight + 20.0f, cellWidth, cellHeight };

    // Make sure plots start empty
    for (int i = 0; i < NUMBER_OF_PLOTS; i++) {
        plots[i].Clear();
    }

    // Set up the game state
    GameState gs;
    gs.seasonIndex = 0;                       // start at Spring
    gs.seasonTimeLeft = SEASON_TIME_SECONDS;  // season countdown
    gs.totalPoints = 0;                       // points this season
    gs.selectedPlot = 0;                      // start selecting top-left plot
    gs.selectedCrop = 0;                      // start with the first crop
    gs.allSeasonsCompleted = false;           // not yet

    for (int i = 0; i < 3; i++) {
        gs.harvested[i] = false;
    }

    // Main loop for the farming screen
    while (!WindowShouldClose()) {

        // ----------------- UPDATE -----------------
        float dt = GetFrameTime();

        // Allow ESC to return to the menu (close this window)
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        // If the game is not fully completed, the season timer counts down
        if (!gs.allSeasonsCompleted) {
            gs.seasonTimeLeft = gs.seasonTimeLeft - dt;
            if (gs.seasonTimeLeft < 0.0f) {
                gs.seasonTimeLeft = 0.0f;
            }
        }

        // Move selected plot
        HandleMovement(gs);

        // Plant with SPACE (on selected plot)
        if (IsKeyPressed(KEY_SPACE)) {
            TryPlant(gs, plots[gs.selectedPlot]);
        }

        // Harvest with H (on selected plot)
        if (IsKeyPressed(KEY_H)) {
            TryHarvest(gs, plots[gs.selectedPlot]);
        }

        // Grow plants
        for (int i = 0; i < NUMBER_OF_PLOTS; i++) {
            UpdateGrowth(plots[i], dt);
        }

        // If all 3 crops were harvested BEFORE time runs out, go to next season
        if (!gs.allSeasonsCompleted && HarvestedAllThree(gs) && gs.seasonTimeLeft > 0.0f) {
            if (gs.seasonIndex < WINTER) {
                // Next season
                gs.seasonIndex = gs.seasonIndex + 1;
                ResetSeason(gs, plots);
            } else {
                // We finished Winter too — all seasons done
                gs.allSeasonsCompleted = true;
            }
        }

        // If time is up and we did NOT finish all 3 crops, allow retry with ENTER
        if (!gs.allSeasonsCompleted && gs.seasonTimeLeft <= 0.0f && !HarvestedAllThree(gs)) {
            if (IsKeyPressed(KEY_ENTER)) {
                ResetSeason(gs, plots);
            }
        }

        // ------------------ DRAW ------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Title: show season name OR "All Seasons Completed!"
        if (!gs.allSeasonsCompleted) {
            std::string title = "Season: " + std::string(SEASON_NAMES[gs.seasonIndex]);
            DrawText(title.c_str(), 24, 20, 36, BLACK);
        } else {
            std::string title = "All Seasons Completed!";
            DrawText(title.c_str(), 24, 20, 36, BLACK);
        }

        // Season timer (top-right)
        int screenW = GetScreenWidth();
        int secondsLeft = (int)(gs.seasonTimeLeft + 0.5f);
        std::string timeText = "Time: " + std::to_string(secondsLeft) + "s";
        int timeTextX = screenW - MeasureText(timeText.c_str(), 28) - 24;
        DrawText(timeText.c_str(), timeTextX, 24, 28, BLACK);

        // Help text
        DrawText("W/A/S/D move  |  Click crop to select  |  SPACE plant  |  H harvest  |  ENTER retry  |  ESC menu",
                 60, 80, 18, DARKGRAY);

        // Draw the grid outline and the 4 plots
        DrawRectangleLinesEx(gridArea, 2.0f, BLACK);
        for (int i = 0; i < NUMBER_OF_PLOTS; i++) {
            bool selected = (i == gs.selectedPlot);
            DrawOnePlot(gs, plots[i], selected);
        }

        // Draw the inventory panel (3 crops + points bar)
        DrawInventory(gs, invArea);

        // Show the unlock thresholds (for clarity)
        const CropDef* crops = GetSeasonCrops(gs.seasonIndex);
        int y = (int)(invArea.y + invArea.height + 10);
        DrawText("Unlocks:", (int)invArea.x + 12, y, 18, BLACK);
        y = y + 24;
        for (int i = 0; i < 3; i++) {
            std::string row = crops[i].name + ": " + std::to_string(crops[i].unlockPoints) + " pts";
            DrawText(row.c_str(), (int)invArea.x + 12, y, 18, DARKGRAY);
            y = y + 20;
        }

        // Simple overlays for “retry” and “completed”
        if (!gs.allSeasonsCompleted && gs.seasonTimeLeft <= 0.0f && !HarvestedAllThree(gs)) {
            std::string big = "Time's up!";
            std::string small = "Press ENTER to retry this season, or ESC to return to the menu.";
            int sw = GetScreenWidth();
            int sh = GetScreenHeight();
            DrawRectangle(0, sh/2 - 80, sw, 140, Color{255,255,255,230});
            DrawRectangleLines(10, sh/2 - 80, sw-20, 140, BLACK);
            DrawText(big.c_str(),   CenterX(big,   36, sw), sh/2 - 40, 36, BLACK);
            DrawText(small.c_str(), CenterX(small, 22, sw), sh/2 +  2, 22, DARKGRAY);
        }

        if (gs.allSeasonsCompleted) {
            std::string big = "Congrats! All seasons completed.";
            std::string small = "Press ESC to return to menu, or ENTER to replay Spring.";
            int sw = GetScreenWidth();
            int sh = GetScreenHeight();
            DrawRectangle(0, sh/2 - 80, sw, 140, Color{255,255,255,230});
            DrawRectangleLines(10, sh/2 - 80, sw-20, 140, BLACK);
            DrawText(big.c_str(),   CenterX(big,   32, sw), sh/2 - 40, 32, BLACK);
            DrawText(small.c_str(), CenterX(small, 20, sw), sh/2 +   2, 20, DARKGRAY);

            // Allow ENTER to replay Spring right away
            if (IsKeyPressed(KEY_ENTER)) {
                gs.allSeasonsCompleted = false;
                gs.seasonIndex = SPRING;
                ResetSeason(gs, plots);
            }
        }

        EndDrawing();
    }

    // Close ONLY the farming window. The menu will recreate its own window.
    CloseWindow();
}

