#include "Player.h"

#include <iostream>
using namespace std;

Player::Player() {
  points = 0;
  requiredPoints = 0;
  currentSeed = nullptr;
}

void Player::addPoints(int pointsToAdd) { points += pointsToAdd; }

void Player::deductPoints(int pointsToDeduct) {
  points -= pointsToDeduct;
  if (points < 0) points = 0;
}

bool Player::hasCompletedSeason() {
  for (auto seed : seedsOwned) {
    if (!seed->get_HarvestStatus()) return false;
  }
  return true;
}

int Player::getPoints() const { return points; }
std::string Player::getName() { return name; }

/* 
void Player::selectSeed(string seedType) {
  for (auto seed : seedsOwned) {
    if (seed->get_Name() == seedType) {
      currentSeed = seed;
      cout << "Selected seed: " << seed->get_Name() << endl;
      return;
    }
  }
  cout << "Seed " << seedType << " not found!" << endl;
}
  */

  Seed* Player::selectSeed(Seed* s) {
  for (auto seed : seedsOwned) {
    if (seed->get_Name() == s->get_Name()) {
      currentSeed = seed;
      cout << "Selected seed: " << seed->get_Name() << endl;
      return currentSeed;
    }
  }
  cout << "Seed " << s->get_Name() << " not found!" << endl;
  return nullptr;
}

void Player::plantSeed() {
  if (!currentSeed) {
    std::cout << "No seed selected!\n";
    return;
  }
  std::cout << "Planting " << currentSeed->get_Name() << "...\n";
}

void Player::growCrop() {
  if (!currentSeed) {
    std::cout << "No seed selected to grow!\n";
    return;
  }
  currentSeed->grow();
}

void Player::harvestCrop() {
  if (!currentSeed) {
    std::cout << "No seed selected to harvest!\n";
    return;
  }
  int pointsEarned = currentSeed->harvest();
  if (pointsEarned > 0) {
    addPoints(pointsEarned);
    std::cout << "Harvested " << currentSeed->get_Name() << "! Earned "
              << pointsEarned << " points.\n";
  }
}
void Player::resetProgress() {
  // Player doesn't own the seeds — just clear references
  seedsOwned.clear();
  harvestedCrops.clear();
  currentSeed = nullptr;
  points = 0; // Reset points to 0
}

void Player::addSeed(Seed* s) {
  if (!s) return;

  // Avoid adding duplicates: check if this pointer is already present.
  for (Seed* existing : seedsOwned) {
    if (existing == s) return;
  }

  seedsOwned.push_back(s);
}

void Player::setSeeds(const std::vector<Seed*>& seeds) { seedsOwned = seeds; }

void Player::setName(const std::string& playerName) { name = playerName; }

std::vector<Seed*>& Player::getOwnedSeeds() { return seedsOwned; }
