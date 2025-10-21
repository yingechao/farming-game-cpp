
#include "Cauliflower.h"

#include "Seed.h"

Cauliflower::Cauliflower() : Seed("Cauliflower", 10, 20) {
  pointsUnlockThreshold = 30; // Unlock after earning 30 points (2 potatoes + 1 strawberry)
}

void Cauliflower::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Cauliflower::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

void Cauliflower::reset() { isHarvested = false; }

void Cauliflower::plant() {
  std::cout << "Planting Cauliflower specifically!\n";
  grow();
}
