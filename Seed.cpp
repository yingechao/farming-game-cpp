#include "Seed.h"

#include <iostream>

// Constructor
Seed::Seed(std::string seedName, float timeToGrow, int val)
    : name(seedName), growTime(timeToGrow), value(val), isHarvested(false) {}

// Getters
// Returns the seed's name
std::string Seed::get_Name() const { return name; }
// Returns the seed's growth time
float Seed::get_GrowTime() const { return growTime; }
// Return the value of the seed
int Seed::get_Value() const { return value; }
//  Returns whether or not the seed is harvested
bool Seed::get_HarvestStatus() const { return isHarvested; }

// Currently implemented simply as "not harvested",
// but could later be expanded to check for actual time-based growth.
bool Seed::isReadyToHarvest() const { return !isHarvested; }


int Seed::getPointsUnlockThreshold() const {
  return pointsUnlockThreshold;
}
