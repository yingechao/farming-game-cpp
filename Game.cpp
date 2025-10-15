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
    : currentLevel(1),
      timeLimit(60),
      isGameOver(false),
      currentSeason(nullptr),
      currentSeedStartTime(0.0f) {}

Game::~Game() {
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
  return ms / 1000.0f;
}

void Game::startGame() {
  // start with Spring
  currentSeason = new Season("Spring", timeLimit);
  currentSeedIndex = 0;
  currentSeed = currentSeason->getSeeds()[currentSeedIndex];
  isGameOver = false;
  std::cout << "\n🌸 Starting Spring season!\n";
}

// Start the current level
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

  currentSeason = new Season(seasonName, timeLimit);
  currentSeason->start_Season();
  startTime = getCurrentTime();
  cout << "Level " << currentLevel << ": " << seasonName
       << " season started!\n";
}

// Handle timeout
void Game::handleTimeOut() {
  cout << "⏰ Time’s up! Restarting level.\n";
  player.deductPoints(10);
  startLevel();
}

// Check if season time expired
bool Game::isTimeUp() const {
  if (!currentSeason) return false;
  float elapsed = getCurrentTime() - startTime;
  return elapsed >= currentSeason->getTimeLimit();
}

// Check if player progressed
bool Game::checkProgress() {
  if (!currentSeason) return false;
  return currentSeason->allSeedsCompleted();
}

// End game
void Game::endGame() {
  isGameOver = true;
  cout << "🌾 Game Over! Final score: " << player.getPoints() << "\n";
}

bool Game::isSeedTimeUp(float seedGrowTime) const {
  float elapsed = getCurrentTime() - currentSeedStartTime;
  return elapsed >= seedGrowTime;
}

void Game::plantCurrentSeed() {
  Seed* s = getCurrentSeed();
  if (!s) return;
  currentSeedStartTime = getCurrentTime();
  s->plant();
}

int Game::harvestCurrentSeed() {
  Seed* seed = getCurrentSeed();
  if (!seed) return 0;
  int earned = seed->harvest();  // returns seed value
  player.addPoints(earned);
  return earned;
}

// Access player
Player& Game::getPlayer() { return player; }
Season* Game::getCurrentSeason() { return currentSeason; }
int Game::getPlayerScore() const { return player.getPoints(); }

//

Seed* Game::getCurrentSeed() {
  if (!currentSeason) return nullptr;
  std::vector<Seed*>& seeds = currentSeason->getSeeds();
  if (currentSeedIndex < 0 || currentSeedIndex >= seeds.size()) return nullptr;
  return seeds[currentSeedIndex];
}

//
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