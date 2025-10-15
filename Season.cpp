#include "Season.h"

#include <iostream>

#include "Cauliflower.h"
#include "Eggplant.h"
#include "Potato.h"
#include "Strawberry.h"
#include "Tomato.h"
#include "Wheat.h"
using namespace std;

// Constructor
Season::Season(std::string seasonName, int limit)
    : name(seasonName), timeLimit(limit), currentSeedIndex(0) {
  loadSeeds();
}

// Destructor
Season::~Season() {
  for (auto seed : seeds) delete seed;
  seeds.clear();
}

std::vector<Seed*>& Season::getSeeds() { return seeds; }

// Load seeds based on season
void Season::loadSeeds() {
  if (name == "Spring") {
    seeds.push_back(new Potato());
    seeds.push_back(new Strawberry());
    seeds.push_back(new Cauliflower());
  } else if (name == "Summer") {
    seeds.push_back(new Tomato());
    seeds.push_back(new Wheat());
    seeds.push_back(new Eggplant());
  } else if (name == "Autumn") {
    // seeds.push_back(new Pumpkin());
    // seeds.push_back(new Yam());
    // seeds.push_back(new Cranberry());
  } else if (name == "Winter") {
    // seeds.push_back(new SnowYam());
    // seeds.push_back(new WinterRoot());
    // seeds.push_back(new Holly());
  }

  currentSeedIndex = 0;  // unlock first seed
  if (!seeds.empty()) unlockNextSeed();
}

// Get current seed
Seed* Season::getCurrentSeed() {
  if (currentSeedIndex < seeds.size()) return seeds[currentSeedIndex];
  return nullptr;
}

// Check if more seeds left
bool Season::hasMoreSeeds() { return currentSeedIndex < seeds.size(); }

// Unlock next seed
Seed* Season::unlockNextSeed() {
  if (currentSeedIndex + 1 < seeds.size()) {
    currentSeedIndex++;
    return seeds[currentSeedIndex];
  }
  return nullptr;  // no more seeds left
}

// Getters
std::string Season::get_Name() { return name; }
int Season::getTimeLimit() { return timeLimit; }
bool Season::isCurrentSeedAvailable() {
  return currentSeedIndex >= 0 && currentSeedIndex < seeds.size();
}

// Called when the season starts
void Season::start_Season() {
  currentSeedIndex = 0;
  if (!seeds.empty()) unlockNextSeed();  // unlock first seed

  std::cout << "Starting " << name << " season!" << std::endl;
  std::cout << "Loading seeds..." << std::endl;

  for (Seed* s : seeds) {
    std::cout << "Seed: " << s->get_Name()
              << " - Grow time: " << s->get_GrowTime() << " seconds\n";
  }

  std::cout << "Season loop starting..." << std::endl;
}

// Returns true if all seeds have been harvested
bool Season::allSeedsCompleted() {
  for (auto seed : seeds) {
    if (!seed->get_HarvestStatus())  // assumes Seed has getHarvestStatus()
      return false;
  }
  return true;
}

// Reset the season: marks all seeds as unharvested and resets index
void Season::resetSeason() {
  currentSeedIndex = 0;
  for (auto seed : seeds) {
    seed->reset();  // assumes Seed has reset() method
  }
}

bool Season::isSeedTimeUp(float elapsedTime) {
  if (currentSeedIndex < seeds.size()) {
    Seed* current = seeds[currentSeedIndex];
    return elapsedTime >= current->get_GrowTime();
  }
  return false;
}

void Season::advanceSeed() {
  if (!seeds.empty() && currentSeedIndex < seeds.size() - 1) {
    currentSeedIndex++;
  } else {
    // All seeds done
    std::cout << "Season " << name << " completed!\n";
    }
}

void Season::endSeason() {
  cout << "Season " << name << " completed!\n";
  completed = true;
}

bool Season::isCompleted() { return completed; }
