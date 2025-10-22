#ifndef GAME_H
#define GAME_H

#include <chrono>
#include "Player.h"
#include "Season.h"
#include "Seed.h"
#include "Spring.h"
#include "Summer.h"
#include "Autumn.h"
#include "Winter.h"
#include "CropState.h"
#include "Planted.h"
#include "Harvestable.h"
#include "Harvested.h"
#include "Plot.h"

class Game {
private:
    int currentLevel;
    bool isGameOver;
    float getCurrentTime() const;
    float startTime;
    int currentSeasonIndex;
    
    Player* player;
    Season* currentSeason;
    

    bool harvested[3];
    int totalPoints;
    
    // Private helper methods
    void initializeSeason();
    void cleanupSeason();
    bool checkSeasonProgress() const;
    void advanceToNextSeason();
    
public:
    // Constructor and Destructor 
    Game();
    ~Game();
    
    // Game lifecycle methods 
    void startGame();
    void endGame();
    bool isGameOverStatus() const;
    
    // Season management
    void nextLevel();
    bool checkProgress() const;
    
    // Player interaction methods
    bool plantSeed(Plot& plot, int cropIndex);
    int harvestSeed(Plot& plot);
    void updateGrowth(Plot& plot, float timeFrame);
    
    // Getters for GUI
    Player* getPlayer() const;
    Season* getCurrentSeason() const;
    int getTotalPoints() const;
    int getSeasonIndex() const;
    float getSeasonTimeLeft() const;
    bool isCropUnlocked(int cropIndex) const;
    bool isCropHarvested(int cropIndex) const;
    void setCropHarvested(int cropIndex, bool harvested);
    
    // Game state management
    void resetSeason();
    void updateTime(float deltaTime);
    void startSeasonTimer();
};

#endif 
