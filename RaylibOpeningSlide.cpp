#include "raylib.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// ---------- simple UI helpers ----------
struct Button { Rectangle box; std::string text; };

bool MouseInside(Rectangle area) {
    Vector2 m = GetMousePosition();
    return (m.x >= area.x && m.x <= area.x + area.width &&
            m.y >= area.y && m.y <= area.y + area.height);
}

bool isButtonClicked(Rectangle r) {
    return MouseInside(r) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int centerText(const std::string& text, int fontSize, int screenWidth) {
    return (screenWidth - MeasureText(text.c_str(), fontSize)) / 2;
}

void DrawSimpleButton(Button b, int fontSize) {
    Color fill = MouseInside(b.box) ? Color{102,191,255,255} : RAYWHITE;
    DrawRectangleRec(b.box, fill);
    DrawRectangleLinesEx(b.box, 2.0f, BLACK);
    int tx = (int)(b.box.x + (b.box.width - MeasureText(b.text.c_str(), fontSize)) / 2);
    int ty = (int)(b.box.y + (b.box.height - fontSize) / 2);
    DrawText(b.text.c_str(), tx, ty, fontSize, BLACK);
}

std::string ReadTextFile(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return "instructions.txt not found.";
    std::string line, out;
    while (std::getline(f, line)) out += line + "\n";
    return out;
}

std::vector<std::string> SplitLines(const std::string& t) {
    std::vector<std::string> lines;
    std::stringstream ss(t);
    std::string line;
    while (std::getline(ss, line)) lines.push_back(line);
    return lines;
}

// ---------- screens ----------
enum class Screen { MENU, GAME, INSTRUCTIONS };

// Forward declaration: implemented in FarmingGrid.cpp
void RunFarmingGrid();

int main() {
    // Window (MENU)
    int screenWidth = 900, screenHeight = 550;
    InitWindow(screenWidth, screenHeight, "Farm Game (Menu)");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // ESC won't auto-kill the app (we control it)

    Screen currentScreen = Screen::MENU;
    bool gameStarted = false;

    int titleFontSize = 48, buttonFontSize = 24;
    int buttonWidth = 260, buttonHeight = 52, gap = 16;
    float x = (screenWidth - buttonWidth) / 2.0f, y = 170.0f;

    Button btnStart { {x, y + 0*(buttonHeight+gap), (float)buttonWidth, (float)buttonHeight}, "START" };
    Button btnCont  { {x, y + 1*(buttonHeight+gap), (float)buttonWidth, (float)buttonHeight}, "CONTINUE" };
    Button btnInstr { {x, y + 2*(buttonHeight+gap), (float)buttonWidth, (float)buttonHeight}, "INSTRUCTIONS" };
    Button btnExit  { {x, y + 3*(buttonHeight+gap), (float)buttonWidth, (float)buttonHeight}, "EXIT" };

    std::string instructionsText = ReadTextFile("instructions.txt");
    std::vector<std::string> instructionLines = SplitLines(instructionsText);
    int scroll = 0;
    Button buttonBack { {20, 20, 140, 50}, "BACK" };

    // -------- main loop --------
    while (!WindowShouldClose()) {
        // --- UPDATE ---
        if (currentScreen == Screen::MENU) {

            // START: close menu -> run grid (its own window) -> recreate menu
            if (isButtonClicked(btnStart.box)) {
                CloseWindow();                 // 1) close menu window
                RunFarmingGrid();              // 2) open & run farming window
                InitWindow(screenWidth, screenHeight, "Farm Game (Menu)"); // 3) recreate menu
                SetTargetFPS(60);
                SetExitKey(KEY_NULL);
                gameStarted = true;
                currentScreen = Screen::MENU;
                continue; // IMPORTANT: skip drawing in this frame
            }

            if (gameStarted && isButtonClicked(btnCont.box)) {
                currentScreen = Screen::GAME; // (placeholder)
            }

            if (isButtonClicked(btnInstr.box)) {
                instructionsText = ReadTextFile("instructions.txt");
                instructionLines = SplitLines(instructionsText);
                scroll = 0;
                currentScreen = Screen::INSTRUCTIONS;
            }

            if (isButtonClicked(btnExit.box)) {
                break;
            }
        }
        else if (currentScreen == Screen::INSTRUCTIONS) {
            if (isButtonClicked(buttonBack.box)) currentScreen = Screen::MENU;
            if (IsKeyPressed(KEY_DOWN)) scroll++;
            if (IsKeyPressed(KEY_UP))   { scroll--; if (scroll < 0) scroll = 0; }
        }
        else if (currentScreen == Screen::GAME) {
            if (IsKeyPressed(KEY_ESCAPE)) currentScreen = Screen::MENU;
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == Screen::MENU) {
            DrawText("FARM GAME",
                     centerText("FARM GAME", titleFontSize, screenWidth),
                     70, titleFontSize, BLACK);

            DrawSimpleButton(btnStart, buttonFontSize);

            if (gameStarted) {
                DrawSimpleButton(btnCont, buttonFontSize);
            } else {
                DrawRectangleRec(btnCont.box, Color{220,220,220,255});
                DrawRectangleLinesEx(btnCont.box, 2.0f, GRAY);
                int tx = (int)(btnCont.box.x + (btnCont.box.width - MeasureText("CONTINUE", buttonFontSize)) / 2);
                int ty = (int)(btnCont.box.y + (btnCont.box.height - buttonFontSize) / 2);
                DrawText("CONTINUE", tx, ty, buttonFontSize, GRAY);
            }

            DrawSimpleButton(btnInstr, buttonFontSize);
            DrawSimpleButton(btnExit,  buttonFontSize);
        }
        else if (currentScreen == Screen::INSTRUCTIONS) {
            DrawSimpleButton(buttonBack, buttonFontSize);
            DrawText("INSTRUCTIONS",
                     centerText("INSTRUCTIONS", 36, screenWidth),
                     30, 36, BLACK);

            int leftMargin = 80, topMargin = 120, lineHeight = 22;
            int maxLines = (screenHeight - topMargin - 30) / lineHeight;
            for (int i = 0; i < maxLines; i++) {
                int idx = scroll + i;
                if (idx >= 0 && idx < (int)instructionLines.size())
                    DrawText(instructionLines[idx].c_str(), leftMargin, topMargin + i*lineHeight, 20, BLACK);
            }
        }
        else if (currentScreen == Screen::GAME) {
            DrawText("GAME RUNNING (placeholder)",
                     centerText("GAME RUNNING (placeholder)", 28, screenWidth),
                     200, 28, BLACK);
            DrawText("Press ESC to return to Menu",
                     centerText("Press ESC to return to Menu", 20, screenWidth),
                     240, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
