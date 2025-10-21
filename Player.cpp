#include "Player.h"

#include <iostream>
using namespace std;

Player::Player() {
  points = 0;
  requiredPoints = 0;     // Points required for a season start at 0
  currentSeed = nullptr;  // No seed selected initially
}

// Adds points to the player's score
void Player::addPoints(int pointsToAdd) { points += pointsToAdd; }

// Checks if the player has completed the season
// Returns true if all owned seeds have been harvested
bool Player::hasCompletedSeason() {
  for (auto seed : seedsOwned) {
    if (!seed->get_HarvestStatus()) return false;
  }
  return true;
}

// Returns current points
int Player::getPoints() const { return points; }

// Returns the player's name
std::string Player::getName() { return name; }

// Select a seed from player's owned seeds based on the seed type/name
void Player::selectSeed(string seedType) {
  for (auto seed : seedsOwned) {
    if (seed->get_Name() == seedType) {
      currentSeed = seed;  // Set the selected seed
      cout << "Selected seed: " << seed->get_Name() << endl;
      return;
    }
  }
  cout << "Seed " << seedType << " not found!"
       << endl;  // Seed not in collection
}

// Plant the currently selected seed
void Player::plantSeed() {
  if (!currentSeed) {  // Check if a seed has been selected
    std::cout << "No seed selected!\n";
    return;
  }
  std::cout << "Planting " << currentSeed->get_Name() << "...\n";
  currentSeed->plant();  // Call the Seed's plant method
}

// Grow the currently selected seed
void Player::growCrop() {
  if (!currentSeed) {
    std::cout << "No seed selected to grow!\n";
    return;
  }
  currentSeed->grow();  // Call the Seed's grow method
}

// Harvest the currently selected seed
void Player::harvestCrop() {
  if (!currentSeed) {
    std::cout << "No seed selected to harvest!\n";
    return;
  }
  int pointsEarned = currentSeed->harvest();  // Call Seed's harvest method
  if (pointsEarned > 0) {
    addPoints(pointsEarned);  // Add earned points to player
    std::cout << "Harvested " << currentSeed->get_Name() << "! Earned "
              << pointsEarned << " points.\n";
  }
}

// Reset player progress for a season
// Clears references but does NOT delete seeds themselves
void Player::resetProgress() {
  // Player doesn't own the seeds — just clear references
  seedsOwned.clear();      // Remove all seeds from collection (references only)
  harvestedCrops.clear();  // Clear harvested crops list
  currentSeed = nullptr;   // No seed selected
}

// Add a seed to the player's collection
void Player::addSeed(Seed* s) {
  // push_back() is a member function primarily associated with sequence
  // containers like std::vector and std::list. Its purpose is to add a new
  // element to the end of the container.
  if (s) seedsOwned.push_back(s);
}

// Replace player's owned seeds with a new vector
void Player::setSeeds(const std::vector<Seed*>& seeds) { seedsOwned = seeds; }

// Set the player's name
void Player::setName(const std::string& playerName) { name = playerName; }

// Get the vector of seeds owned by the player (by reference)
std::vector<Seed*>& Player::getOwnedSeeds() { return seedsOwned; }
