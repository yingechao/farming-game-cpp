#include "Strawberry.h"

#include <iostream>

Strawberry::Strawberry() : Seed("Strawberry", 7, 15) {}

void Strawberry::plant() {
  std::cout << "Planting Strawberry specifically!\n";
  grow();
}

void Strawberry::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

int Strawberry::harvest() {
  if (!isHarvested) {
    isHarvested = true;
    std::cout << name << " harvested! +" << value << " points.\n";
    return value;
  }
  std::cout << name << " was already harvested!\n";
  return 0;
}

void Strawberry::reset() { isHarvested = false; }
