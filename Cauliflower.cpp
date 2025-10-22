
#include "Cauliflower.h"

#include "Seed.h"

Cauliflower::Cauliflower() : Seed("Cauliflower", 10, 20) {
  pointsUnlockThreshold = 100; 
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

Seed* Cauliflower::clone() const {
  return new Cauliflower();
}
