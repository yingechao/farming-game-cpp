#include "Strawberry.h"

#include <iostream>

// Initializes a Strawberry seed by calling the base Seed constructor
Strawberry::Strawberry() : Seed("Strawberry", 7, 15) {}

// Overrides the base plant() method with a strawberry-specific message.
// Calls grow() afterward to simulate growth.
void Strawberry::plant() {
  std::cout << "Planting Strawberry specifically!\n";
  grow();
}
// Simulates the strawberry’s growth over time.
void Strawberry::grow() {
  std::cout << name << " is growing for " << growTime << " seconds.\n";
}

// return Integer number of points earned
int Strawberry::harvest() {
  std::cout << name << " harvested! +" << value << " points.\n";
  return value;
}

// resets  harvested status of strawberry and keeps false
void Strawberry::reset() { isHarvested = false; }
