
#include "Beetroot.h"

#include "Seed.h"

Beetroot::Beetroot() : Seed("Beetroot", 11, 28) {
    pointsUnlockThreshold = 60;
}

void Beetroot::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Beetroot::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

void Beetroot::reset() { isHarvested = false; }

void Beetroot::plant() {
  std::cout << "Planting Beetroot specifically!\n";
  grow();
}

Seed* Beetroot::clone() const {
  return new Beetroot();
}
