
#include "Onion.h"

#include "Seed.h"

Onion::Onion() : Seed("Onion", 14, 30) {
    pointsUnlockThreshold = 100;
}

void Onion::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Onion::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

void Onion::reset() { isHarvested = false; }

void Onion::plant() {
  std::cout << "Planting Onion specifically!\n";
  grow();
}

Seed* Onion::clone() const {
  return new Onion();
}
