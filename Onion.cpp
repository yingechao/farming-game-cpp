
#include "Onion.h"

#include "Seed.h"

Onion::Onion() : Seed("Onion", 14, 30) {
    pointsUnlockThreshold = 100;
}

void Onion::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Onion::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  return 0;
}

void Onion::reset() { isHarvested = false; }

void Onion::plant() {
  std::cout << "Planting Onion specifically!\n";
  grow();
}
