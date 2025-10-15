
#include "Lettuce.h"

#include "Seed.h"

Lettuce::Lettuce() : Seed("Lettuce", 9, 24) {}

void Lettuce::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Lettuce::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  return 0;
}

void Lettuce::reset() { isHarvested = false; }

void Lettuce::plant() {
  std::cout << "Planting Lettuce specifically!\n";
  grow();
}
