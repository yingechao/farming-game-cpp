#include "Game.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "Spring.h"
#include "Summer.h"
#include "Autumn.h"
#include "Winter.h"

using namespace std;

// Constructor & Destructor
Game::Game()
    : currentLevel(1),
      timeLimit(60),
      isGameOver(false),
      currentSeason(nullptr),
  currentSeedStartTime(0.0), seasonStartScore(0) {}

Game::~Game() {
  if (currentSeason) {
    delete currentSeason;
    currentSeason = nullptr;
  }
}

// Helper to get current system time in seconds
double Game::getCurrentTime() const {
  using namespace std::chrono;
  auto now = high_resolution_clock::now();
  auto ms = duration_cast<milliseconds>(now.time_since_epoch()).count();
  return ms / 1000.0;
}

void Game::startGame() {
  // Start the game at level 1 and delegate to startLevel() so the
  // concrete season constructors run and set season-specific data.
  currentLevel = 1;
  isGameOver = false;
  startLevel();
}

// Start the current level
void Game::startLevel() {
  if (currentSeason) {
    delete currentSeason;
    currentSeason = nullptr;
  }

  std::string seasonName;
  switch (currentLevel) {
    case 1: seasonName = "Spring"; break;
    case 2: seasonName = "Summer"; break;
    case 3: seasonName = "Autumn"; break;
    case 4: seasonName = "Winter"; break;
    default: endGame(); return;
  }

  // instantiate concrete season subclass so its constructor sets
  // requiredPoints and any season-specific config
  if (seasonName == "Spring")
    currentSeason = new Spring();
  else if (seasonName == "Summer")
    currentSeason = new Summer();
  else if (seasonName == "Autumn")
    currentSeason = new Autumn();
  else if (seasonName == "Winter")
    currentSeason = new Winter();
  else
    currentSeason = new Season(seasonName, timeLimit);

  currentSeason->start_Season();
  startTime = std::chrono::steady_clock::now();
  seasonStartScore = player.getPoints();

  // initialize seed index and current seed pointer
  currentSeedIndex = 0;
  if (!currentSeason->getSeeds().empty())
    currentSeed = currentSeason->getSeeds()[currentSeedIndex];

  cout << "Level " << currentLevel << ": " << seasonName
       << " season started!\n";
  if (currentSeason) {
    cout << "Required points for this season: "
         << currentSeason->getRequiredPoints() << "\n";
  }
}
// Handle timeout
void Game::handleTimeOut() {
  cout << "⏰ Time’s up!\n";
  int seasonPoints = player.getPoints() - seasonStartScore;
  int required = 0;
  if (currentSeason) required = currentSeason->getRequiredPoints();
  cout << "Points this season: " << seasonPoints << " (required: "
       << required << ")\n";
  if (seasonPoints >= required) {
    cout << "You reached the required points for this season! Advancing...\n";
    // advance to next level
    nextLevel();
  } else {
    cout << "You did NOT reach the required points. Restarting level...\n";
    startLevel();
  }
}

// Check if season time expired
bool Game::isTimeUp() const {
  if (!currentSeason) return false;
  using namespace std::chrono;
  auto now = steady_clock::now();
  auto elapsed = duration_cast<seconds>(now - startTime).count();
  return elapsed >= currentSeason->getTimeLimit();
}

// Public wrapper used by main loop
bool Game::checkProgress() {
  if (!currentSeason) return false;
  return currentSeason->allSeedsCompleted();
}

// End game
void Game::endGame() {
  isGameOver = true;
  cout << "🌾 Game Over! Final score: " << player.getPoints() << "\n";
}

bool Game::isSeedTimeUp(double seedGrowTime) const {
  double elapsed = getCurrentTime() - currentSeedStartTime;
  return elapsed >= seedGrowTime;
}

void Game::plantCurrentSeed() {
  Seed* s = getCurrentSeed();
  if (!s) return;
  // If this seed was previously harvested, reset it so it can be planted again.
  s->reset();
  currentSeedStartTime = getCurrentTime();
  s->plant();
}

int Game::harvestCurrentSeed() {
  Seed* seed = getCurrentSeed();
  if (!seed) return 0;
  int earned = seed->harvest();  // returns seed value
  player.addPoints(earned);
  // Show player's updated total after each harvest
  std::cout << "Total points: " << player.getPoints() << "\n";
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

Seed* Game::selectNewSeed(int a){
  if (!currentSeason) return nullptr;
  std::vector<Seed*>& seeds = currentSeason->getSeeds();
 
  if (seeds.empty()) return nullptr;

  // Treat `a` as a 1-based index coming from the UI (input "1" -> first seed).
  int idx = a - 1;
  if (currentSeedIndex < 0 || currentSeedIndex >= seeds.size()) return nullptr;

  // Update the authoritative index and pointer so other Game methods
  // (getCurrentSeed, plantCurrentSeed, advanceSeed, etc.) operate on the
  // same selected seed.
  this->currentSeedIndex = idx;
  this->currentSeed = seeds[idx];
  return seeds[idx];
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
    // Reached the end of the seed list. Only advance to next season if all
    // seeds have actually been harvested. This prevents progressing when a
    // player has selected or planted the last seed but not harvested it.
    if (currentSeason->allSeedsCompleted()) {
      nextLevel();
    } else {
      std::cout << "Cannot advance: not all seeds have been harvested yet.\n";
    }
  }
}

void Game::nextLevel() {
  // Advance the level and reuse startLevel() to ensure consistent
  // construction and initialization for the new season.
  currentLevel++;
  if (currentLevel > 4) {
    std::cout << "\n All seasons completed! Game Over.\n";
    endGame();
    return;
  }
  startLevel();
}