#include "Tomato.h"

#include <iostream>

Tomato::Tomato() : Seed("Tomato", 9, 24) {
  pointsUnlockThreshold = 0;
}  // initialize base Seed

void Tomato::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Tomato::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

void Tomato::reset() { isHarvested = false; }

void Tomato::plant() {
  std::cout << "Planting Tomato specifically!\n";
  grow();
}
