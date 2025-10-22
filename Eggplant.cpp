#include "Eggplant.h"

#include <iostream>

Eggplant::Eggplant() : Seed("Eggplant", 8, 18) {
    pointsUnlockThreshold = 60;
}

void Eggplant::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Eggplant::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

void Eggplant::reset() { isHarvested = false; }

void Eggplant::plant() {
  std::cout << "Planting Eggplant specifically!\n";
  grow();
}

Seed* Eggplant::clone() const {
  return new Eggplant();
}
