#include "Seed.h"

#include <iostream>

// Constructor
Seed::Seed(std::string seedName, float timeToGrow, int val)
    : name(seedName), growTime(timeToGrow), value(val), isHarvested(false) {}

// Getters
// Returns the seed's name.
std::string Seed::get_Name() const { return name; }
// Returns how long the seed takes to grow.
float Seed::get_GrowTime() const { return growTime; }
// Returns the reward value for harvesting this seed.
int Seed::get_Value() const { return value; }
//  Returns the harvest status (true if harvested).
bool Seed::get_HarvestStatus() const { return isHarvested; }

//  Checks if the seed is ready for harvest.

// Currently implemented simply as "not harvested",
// but could later be expanded to check for actual time-based growth.
bool Seed::isReadyToHarvest() const { return !isHarvested; }

// Virtual methods
// Simulates the growth process of the seed
void Seed::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

// Points gained from harvesting.
int Seed::harvest() {
      std::cout << name << " harvested! +" << value << " points.\n";
    return value;

  // if (!isHarvested) {
  //   isHarvested = true;
  //   std::cout << name << " harvested! +" << value << " points.\n";
  //   return value;
  // } else {
  //   std::cout << name << " was already harvested!\n";
  //   return 0;
  // }
}

// Resets the seed's state so it can be replanted.
void Seed::reset() { isHarvested = false; }

// Simulates planting the seed, then triggers its growth.
void Seed::plant() {
  std::cout << "Planting " << name << "...\n";
  grow();
}

int Seed::getPointsUnlockThreshold() const {
  return pointsUnlockThreshold;
}