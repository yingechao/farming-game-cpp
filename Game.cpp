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

// Game Start
void Game::startGame() {
  isGameOver = false;
  currentLevel = 1;
  player.resetProgress();

  currentSeason = new Spring();  // first season
  for (Seed* s : currentSeason->getSeeds()) {
    player.addSeed(s);
  }

  // Automatically select first seed
  if (!currentSeason->getSeeds().empty()) {
    player.selectSeed(currentSeason->getSeeds()[0]->get_Name());
  }

  currentSeedStartTime = getCurrentTime();
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

void Game::advanceSeed() {
  if (!currentSeason) return;

  currentSeedIndex++;  // move to next seed
  if (currentSeedIndex >= currentSeason->getSeeds().size()) {
    // season completed
    currentSeason->endSeason();
    currentSeedIndex = 0;  // reset for next season
    currentSeedStartTime = getCurrentTime();
    // Optionally, deselect currentSeed in player
    player.currentSeed = nullptr;
  } else {
    // select the next seed for the player automatically
    Seed* nextSeed = currentSeason->getSeeds()[currentSeedIndex];
    if (nextSeed) {
      player.selectSeed(nextSeed->get_Name());
      currentSeedStartTime = getCurrentTime();
    }
  }
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

void Game::nextLevel() {
  // Clear old seeds from player
  player.resetProgress();

  // Delete old season
  if (currentSeason) delete currentSeason;

  if (currentSeasonIndex == 0) {
    currentSeason = new Summer();  // move to Summer
    currentSeasonIndex = 1;
  } else {
    currentSeason = nullptr;  // no more seasons
    isGameOver = true;
    return;
  }

  currentSeedIndex = 0;

  // Assign new seeds to player
  for (Seed* s : currentSeason->getSeeds()) player.addSeed(s);

  // Select first seed
  if (!currentSeason->getSeeds().empty())
    player.selectSeed(currentSeason->getSeeds()[0]->get_Name());

  currentSeason->start_Season();
}
