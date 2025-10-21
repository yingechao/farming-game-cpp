#include "Strawberry.h"

#include <iostream>

Strawberry::Strawberry() : Seed("Strawberry", 7, 15) {
  pointsUnlockThreshold = 20;
}

void Strawberry::plant() {
  std::cout << "Planting Strawberry specifically!\n";
  grow();
}

void Strawberry::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Strawberry::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

void Strawberry::reset() { isHarvested = false; }
