
#include "Potato.h"

#include "Seed.h"

Potato::Potato() : Seed("Potato", 5, 10) {
  pointsUnlockThreshold = 0;
}

void Potato::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Potato::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

void Potato::reset() { isHarvested = false; }

void Potato::plant() {
  std::cout << "Planting Potato specifically!\n";
  grow();
}
