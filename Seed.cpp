#include "Seed.h"

#include <iostream>

// Constructor
Seed::Seed(std::string seedName, float timeToGrow, int val)
    : name(seedName), growTime(timeToGrow), value(val), isHarvested(false) {}

// Getters
std::string Seed::get_Name() const { return name; }
float Seed::get_GrowTime() const { return growTime; }
int Seed::get_Value() const { return value; }
bool Seed::get_HarvestStatus() const { return isHarvested; }
bool Seed::isReadyToHarvest() const { return !isHarvested; }

// Virtual methods
void Seed::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

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

void Seed::reset() { isHarvested = false; }

void Seed::plant() {
  std::cout << "Planting " << name << "...\n";
  grow();
}
