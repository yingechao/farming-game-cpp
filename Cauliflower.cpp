
#include "Cauliflower.h"

#include "Seed.h"

Cauliflower::Cauliflower() : Seed("Cauliflower", 10, 20) {}

void Cauliflower::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Cauliflower::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  return 0;
}

void Cauliflower::reset() { isHarvested = false; }

void Cauliflower::plant() {
  std::cout << "Planting Cauliflower specifically!\n";
  grow();
}
