
#include "Peas.h"

#include "Seed.h"

Peas::Peas() : Seed("Peas", 7, 25) {
    pointsUnlockThreshold = 70;
}

void Peas::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Peas::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  return 0;
}

void Peas::reset() { isHarvested = false; }

void Peas::plant() {
  std::cout << "Planting Peas specifically!\n";
  grow();
}
