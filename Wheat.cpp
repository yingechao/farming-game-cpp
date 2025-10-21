#include "Wheat.h"

#include <iostream>

Wheat::Wheat() : Seed("Wheat", 8, 18) {
    pointsUnlockThreshold = 35;
}  // initialize base Seed

void Wheat::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Wheat::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  return 0;
}

void Wheat::reset() { isHarvested = false; }

void Wheat::plant() {
  std::cout << "Planting Wheat specifically!\n";
  grow();
}
