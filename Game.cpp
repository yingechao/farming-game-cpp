#include "Game.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "Spring.h"
#include "Summer.h"
// #include "Autumn.h"
// #include "Winter.h"

using namespace std;

// Constructor & Destructor
Game::Game()
    : currentLevel(1),               // start at level 1
      timeLimit(60),                 // Default time limit (seconds)
      isGameOver(false),             // game is not over at start
      currentSeason(nullptr),        // No season allocated yet
      currentSeedStartTime(0.0f) {}  // Seed timer starts at 0

Game::~Game() {
  // Clean up the current season if it exists to prevent memory leaks
  if (currentSeason) {
    delete currentSeason;
    currentSeason = nullptr;
  }
}

// Helper to get current system time in seconds
float Game::getCurrentTime() const {
  using namespace std::chrono;
  auto now = high_resolution_clock::now();
  auto ms = duration_cast<milliseconds>(now.time_since_epoch()).count();
  return ms / 1000.0f;  // convert milliseconds to seconds
}

// Start the game: initializes the first season and seed
void Game::startGame() {
  // start with Spring

  currentSeason = new Season(
      "Spring", timeLimit);  // allocates a Season object and stores its address
  currentSeedIndex = 0;      // first seed in season
  currentSeed =
      currentSeason->getSeeds()[currentSeedIndex];  // select current seed
  isGameOver = false;                               // mark game as ongoing
  std::cout << "\n Starting Spring season!\n";
}

// Start the current level/season based on currentLevel
void Game::startLevel() {
  if (currentSeason) {
    delete currentSeason;
    currentSeason = nullptr;
  }

  string seasonName;
  switch (currentLevel) {
    case 1:
      seasonName = "Spring";
      break;
    case 2:
      seasonName = "Summer";
      break;
    case 3:
      seasonName = "Autumn";
      break;
    case 4:
      seasonName = "Winter";
      break;
    default:
      endGame();
      return;
  }

  currentSeason =
      new Season(seasonName, timeLimit);  // allocate memory for new season
  currentSeason->start_Season();          // start season timer
  startTime = getCurrentTime();           // record season start time
  cout << "Level " << currentLevel << ": " << seasonName
       << " season started!\n";
}

// Handle timeout
// restart current level
void Game::handleTimeOut() {
  cout << "⏰ Time’s up! Restarting level.\n";
  startLevel();
}

// Check if season time expired
bool Game::isTimeUp() const {
  if (!currentSeason) return false;
  // checks time passed
  float elapsed = getCurrentTime() - startTime;
  return elapsed >= currentSeason->getTimeLimit();
}

// Check if player has completed all seeds in the current season
bool Game::checkProgress() {
  if (!currentSeason) return false;
  return currentSeason->allSeedsCompleted();
}

// End the game and display final score
void Game::endGame() {
  isGameOver = true;
  cout << "🌾 Game Over! Final score: " << player.getPoints() << "\n";
}

// Check if a specific seed's grow time has elapsed(passed)
bool Game::isSeedTimeUp(float seedGrowTime) const {
  float elapsed = getCurrentTime() - currentSeedStartTime;
  return elapsed >= seedGrowTime;
}

// Plant the current seed: update timer and call seed's plant function
void Game::plantCurrentSeed() {
  Seed* s =
      getCurrentSeed();  // returns a pointer to the currently selected seed.
  if (!s) return;
  currentSeedStartTime = getCurrentTime();
  s->plant();
}

// Harvest the current seed: add points to player and return earned points
int Game::harvestCurrentSeed() {
  Seed* seed = getCurrentSeed();
  if (!seed)
    return 0;  //! s checks if the pointer s is null (nullptr), This is a safety
               //! check to ensure there is a current seed before trying to use
               //! it.
  int earned = seed->harvest();  // returns seed value
  player.addPoints(earned);      // update player score
  return earned;                 // return points earned
}

// Access the player object (by reference)
Player& Game::getPlayer() {
  return player;
}  // returns reference to the member variable

// Access the current season object (pointer)
Season* Game::getCurrentSeason() { return currentSeason; }

// Get the player's current score (read-only)
int Game::getPlayerScore() const { return player.getPoints(); }

// Access the current season object (pointer)
Seed* Game::getCurrentSeed() {
  if (!currentSeason) return nullptr;
  std::vector<Seed*>& seeds = currentSeason->getSeeds();
  if (currentSeedIndex < 0 || currentSeedIndex >= seeds.size()) return nullptr;
  return seeds[currentSeedIndex];
}

// Advance to the next seed in the current season or move to next season
void Game::advanceSeed() {
  if (!currentSeason) return;

  // Move to next seed in the current season
  if (currentSeedIndex + 1 <
      static_cast<int>(currentSeason->getSeeds().size())) {
    currentSeedIndex++;
    currentSeed = currentSeason->getSeeds()[currentSeedIndex];
  } else {
    // All seeds in this season are done → move to next season
    nextLevel();
  }
}

// Move to the next season or end the game if all seasons completed
void Game::nextLevel() {
  // Move to next season
  if (currentSeason->get_Name() == "Spring") {
    delete currentSeason;
    currentSeason = new Season("Summer", timeLimit);
    currentSeedIndex = 0;
    currentSeed = currentSeason->getSeeds()[currentSeedIndex];
    std::cout << "\n Summer has begun!\n";
  } else if (currentSeason->get_Name() == "Summer") {
    delete currentSeason;
    currentSeason = new Season("Autumn", timeLimit);
    currentSeedIndex = 0;
    currentSeed = currentSeason->getSeeds()[currentSeedIndex];
    std::cout << "\n Autumn has begun!\n";
  } else if (currentSeason->get_Name() == "Autumn") {
    delete currentSeason;
    currentSeason = new Season("Winter", timeLimit);
    currentSeedIndex = 0;
    currentSeed = currentSeason->getSeeds()[currentSeedIndex];
    std::cout << "\n  Winter has begun!\n";
  } else {
    // All seasons completed — end game
    std::cout << "\n All seasons completed! Game Over.\n";
    endGame();
  }
}