#include "Season.h"

#include <iostream>

#include "Cauliflower.h"
#include "Eggplant.h"
#include "Potato.h"
#include "Strawberry.h"
#include "Tomato.h"
#include "Wheat.h"
#include "Carrot.h"
#include "Lettuce.h"
#include "Peas.h"
#include "Kale.h"
#include "Beetroot.h"
#include "Onion.h"
using namespace std;

// Constructor
// constructs season with its name and time limit
Season::Season(std::string seasonName, int limit)
    : name(seasonName), timeLimit(limit), currentSeedIndex(0) {
  loadSeeds();  // populate the list of seeds for this season
}

// Destructor
// Cleans up dynamically allocated Seed objects when the Season object is
// destroyed.
Season::~Season() {
  for (auto seed : seeds) delete seed;  // free memory for each Seed pointer
  seeds.clear();                        // empty the vector after deletion
}

// Returns reference to the vector of Seed pointers
std::vector<Seed*>& Season::getSeeds() { return seeds; }

std::vector<std::string> Season::getSeedList() { return seedTypes; }

int Season::getRequiredPoints() { return requiredPoints; }

// Load seeds based on season
// Creates new Seed objects and stores them in the vector.
// The first seed is automatically unlocked.
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
    seeds.push_back(new Carrot());
    seeds.push_back(new Lettuce());
    seeds.push_back(new Peas());
  } else if (name == "Winter") {
    seeds.push_back(new Kale());
    seeds.push_back(new Beetroot());
    seeds.push_back(new Onion());
  }

  currentSeedIndex = 0;  // unlock first seed if available
  if (!seeds.empty()) unlockNextSeed();
}

// Get current seed
// Returns a pointer to the currently active Seed.
// Pointer to the current Seed object, or nullptr if none.
Seed* Season::getCurrentSeed() {
  if (currentSeedIndex < seeds.size()) return seeds[currentSeedIndex];
  return nullptr;
}

// Check if more seeds left unharvested
bool Season::hasMoreSeeds() { return currentSeedIndex < seeds.size(); }

// Unlock next seed
// return Pointer to the next Seed object, or nullptr if all done.
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

// Checks whether the current seed index points to a valid seed.
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

// Checks if the growing time for the current seed has expired.
// return True if the elapsed time >= the seed’s grow time.
bool Season::isSeedTimeUp(float elapsedTime) {
  if (currentSeedIndex < seeds.size()) {
    Seed* current = seeds[currentSeedIndex];
    return elapsedTime >= current->get_GrowTime();
  }
  return false;
}

// Moves the index to the next seed in the vector.
void Season::advanceSeed() {
  if (!seeds.empty() && currentSeedIndex < seeds.size() - 1) {
    currentSeedIndex++;
  } else {
    // All seeds done
    std::cout << "Season " << name << " completed!\n";
  }
}

// Marks the season as completed and prints a message
void Season::endSeason() {
  cout << "Season " << name << " completed!\n";
  completed = true;
}

// Returns true if the season has been marked as completed.
bool Season::isCompleted() { return completed; }
