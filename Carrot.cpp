
#include "Carrot.h"

#include "Seed.h"

Carrot::Carrot() : Seed("Carrot", 9, 22) {
    pointsUnlockThreshold = 0;
}

void Carrot::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Carrot::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  return 0;
}

void Carrot::reset() { isHarvested = false; }

void Carrot::plant() {
  std::cout << "Planting Carrot specifically!\n";
  grow();
}
