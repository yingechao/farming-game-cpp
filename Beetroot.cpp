
#include "Beetroot.h"

#include "Seed.h"

Beetroot::Beetroot() : Seed("Beetroot", 11, 28) {}

void Beetroot::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Beetroot::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  return 0;
}

void Beetroot::reset() { isHarvested = false; }

void Beetroot::plant() {
  std::cout << "Planting Beetroot specifically!\n";
  grow();
}
