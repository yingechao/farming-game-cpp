
#include "Lettuce.h"

#include "Seed.h"

Lettuce::Lettuce() : Seed("Lettuce", 9, 24) {
    pointsUnlockThreshold = 50;
}

void Lettuce::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Lettuce::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

void Lettuce::reset() { isHarvested = false; }

void Lettuce::plant() {
  std::cout << "Planting Lettuce specifically!\n";
  grow();
}
