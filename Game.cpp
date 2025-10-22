#include "Game.h"
#include <iostream>

// Constructor - Initialize game state
Game::Game()
    : currentLevel(1),
      isGameOver(false),
      startTime(0.0f),
      currentSeasonIndex(0),
      player(nullptr),
      currentSeason(nullptr),
      totalPoints(0) {
    
    // Initialize harvested array
    for (int i = 0; i < 3; i++) {
        harvested[i] = false;
    }
    
    // Initialize player
    player = new Player();
}

// Destructor - Clean up resources and free memory
Game::~Game() {
    cleanupSeason();
    if (player) {
        delete player;
        player = nullptr;
    }
}

// Get current system time in seconds
float Game::getCurrentTime() const {
    using namespace std::chrono;
    auto now = steady_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()).count();
    return ms / 1000.0f;
}

// Start the game
void Game::startGame() {
    currentLevel = 1;
    currentSeasonIndex = 0;
    isGameOver = false;
    totalPoints = 0;
    
    // Reset player progress
    if (player) {
        player->resetProgress();
    }
    
    // Initialize first season (Spring)
    initializeSeason();
}

// End the game
void Game::endGame() {
    isGameOver = true;
    std::cout << "Game Over! Final score: " << (player ? player->getPoints() : 0) << std::endl;
}

// Check if game is over
bool Game::isGameOverStatus() const {
    return isGameOver;
}

// Initialize current season
void Game::initializeSeason() {
    cleanupSeason();
    
    // Create season based on current level
    switch (currentLevel) {
        case 1:
            currentSeason = new Spring();
            break;
        case 2:
            currentSeason = new Summer();
            break;
        case 3:
            currentSeason = new Autumn();
            break;
        case 4:
            currentSeason = new Winter();
            break;
        default:
            endGame();
            return;
    }
    
    // Reset season-specific state
    for (int i = 0; i < 3; i++) {
        harvested[i] = false;
    }
    
    // Start season timer
    startTime = getCurrentTime();
    
    std::cout << "Starting " << currentSeason->get_Name() << " season!" << std::endl;
}

// Clean up current season
void Game::cleanupSeason() {
    if (currentSeason) {
        delete currentSeason;
        currentSeason = nullptr;
    }
}

// Check if current season is completed
bool Game::checkSeasonProgress() const {
    if (!currentSeason) return false;
    
    // Check if all three crop types have been harvested
    return harvested[0] && harvested[1] && harvested[2];
}

// Advance to next season
void Game::advanceToNextSeason() {
    currentLevel++;
    currentSeasonIndex++;
    
    if (currentLevel > 4) {
        std::cout << "All seasons completed!" << std::endl;
        endGame();
        return;
    }
    
    // Reset player progress for new season
    if (player) {
        player->resetProgress();
    }
    totalPoints = 0;
    
    // Initialize season but don't reset timer yet
    cleanupSeason();
    
    // Create season based on current level
    switch (currentLevel) {
        case 1:
            currentSeason = new Spring();
            break;
        case 2:
            currentSeason = new Summer();
            break;
        case 3:
            currentSeason = new Autumn();
            break;
        case 4:
            currentSeason = new Winter();
            break;
        default:
            endGame();
            return;
    }
    
    // Reset season-specific state
    for (int i = 0; i < 3; i++) {
        harvested[i] = false;
    }
    
    // Don't reset startTime here - it will be reset when the new season actually starts
}

// Move to next level/season
void Game::nextLevel() {
    advanceToNextSeason();
}

// Check if all progress is complete
bool Game::checkProgress() const {
    return checkSeasonProgress();
}

// Plant a seed in a plot
bool Game::plantSeed(Plot& plot, int cropIndex) {
    // Check if plot is empty
    if (plot.getState() != 0) {
        return false;
    }
    
    // Check if crop index is valid
    if (cropIndex < 0 || cropIndex > 2) {
        return false;
    }
    
    // Check if crop is unlocked
    if (!isCropUnlocked(cropIndex)) {
        return false;
    }
    
    if (!currentSeason) {
        return false;
    }
    
    std::vector<Seed*>& seeds = currentSeason->getSeeds();
    if (cropIndex >= (int)seeds.size()) {
        return false;
    }
    
    // Get the selected seed template and create a new instance for this plot
    Seed* seedTemplate = seeds[cropIndex];
    Seed* selectedSeed = seedTemplate->clone();
    
    // Set up the plot
    plot.plantedSeed = selectedSeed;
    plot.currentState = new Planted();
    plot.timeRemaining = selectedSeed->get_GrowTime();
    
    // Call the plant method
    selectedSeed->plant();
    
    return true;
}

// Harvest a seed from a plot
int Game::harvestSeed(Plot& plot) {
    // Check if plot contains a fully grown crop
    if (plot.getState() != 2) {
        return 0;
    }
    
    if (!plot.plantedSeed || !player) {
        return 0;
    }
    
    // Use harvest method
    int pointsEarned = plot.plantedSeed->harvest();
    player->addPoints(pointsEarned);
    
    // Update GUI points display
    totalPoints = player->getPoints();
    
    // Update harvested status for season completion logic
    if (currentSeason) {
        std::vector<Seed*>& seeds = currentSeason->getSeeds();
        for (int i = 0; i < 3 && i < (int)seeds.size(); i++) {
            if (seeds[i]->get_Name() == plot.plantedSeed->get_Name()) {
                harvested[i] = true;
                break;
            }
        }
    }
    
    // Clear the plot immediately after harvest
    plot.Clear();
    
    return pointsEarned;
}

// Update crop growth
void Game::updateGrowth(Plot& plot, float timeFrame) {
    // Only update if crop is growing
    if (plot.getState() == 1) {
        plot.timeRemaining -= timeFrame;
        
        // Check if growth is complete
        if (plot.timeRemaining <= 0.0f) {
            plot.timeRemaining = 0.0f;
            // Update crop state to harvestable
            if (plot.currentState) {
                delete plot.currentState;
            }
            plot.currentState = new Harvestable();
        }
    }
}

// Getter methods for GUI
Player* Game::getPlayer() const {
    return player;
}

Season* Game::getCurrentSeason() const {
    return currentSeason;
}

int Game::getTotalPoints() const {
    return totalPoints;
}

int Game::getSeasonIndex() const {
    return currentSeasonIndex;
}

float Game::getSeasonTimeLeft() const {
    if (!currentSeason) return 0.0f;
    
    float elapsed = getCurrentTime() - startTime;
    float remaining = currentSeason->getTimeLimit() - elapsed;
    return (remaining > 0.0f) ? remaining : 0.0f;
}

bool Game::isCropUnlocked(int cropIndex) const {
    if (!currentSeason || cropIndex < 0 || cropIndex > 2) {
        return false;
    }
    
    std::vector<Seed*>& seeds = currentSeason->getSeeds();
    if (cropIndex >= (int)seeds.size()) {
        return false;
    }
    
    return totalPoints >= seeds[cropIndex]->getPointsUnlockThreshold();
}

bool Game::isCropHarvested(int cropIndex) const {
    if (cropIndex < 0 || cropIndex > 2) {
        return false;
    }
    return harvested[cropIndex];
}

void Game::setCropHarvested(int cropIndex, bool harvested) {
    if (cropIndex >= 0 && cropIndex < 3) {
        this->harvested[cropIndex] = harvested;
    }
}

// Reset season
void Game::resetSeason() {
    if (player) {
        player->resetProgress();
    }
    totalPoints = 0;
    
    for (int i = 0; i < 3; i++) {
        harvested[i] = false;
    }
    
    // Start timer for retry
    startSeasonTimer();
}

// Update time
void Game::updateTime(float deltaTime) {
    // This method can be used for additional time-based updates
    // Currently handled by individual plot growth updates
}

// Start timer for new season
void Game::startSeasonTimer() {
    startTime = getCurrentTime();
}
