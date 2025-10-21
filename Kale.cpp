
#include "Kale.h"

#include "Seed.h"

Kale::Kale() : Seed("Kale", 10, 26) {
    pointsUnlockThreshold = 0;
}

void Kale::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Kale::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  return 0;
}

void Kale::reset() { isHarvested = false; }

void Kale::plant() {
  std::cout << "Planting Kale specifically!\n";
  grow();
}
