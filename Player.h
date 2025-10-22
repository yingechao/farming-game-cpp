#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "Seed.h"
using namespace std;

class Player {
 private:
  int points;
  int requiredPoints;
  // Pointers to seeds the player owns
  std::vector<Seed*> seedsOwned;  // pointers to seeds

  std::vector<Seed*>
      harvestedCrops;  // Pointers to crops the player has harvested

 public:
  string name;
  Seed* currentSeed;  // Pointer to the currently selected seed

  Player();  // Constructor

  void addPoints(int pointsToAdd);  // Adds points to the player's score
  bool hasCompletedSeason(); 

  int getPoints() const;  // Returns current points 
  std::string getName();  // Returns player's name
  void setName(const std::string& playerName);  // Sets player's name
  void selectSeed(string seedType);  // Select a seed to use

  void resetProgress();  // clears harvested crops and current seed, but does not delete seedsOwned

  void addSeed(Seed* s);  // Add a seed to the player's collection
  void setSeeds(const std::vector<Seed*>& seeds);  // Replace the player's owned seeds with a new vector of seeds
  std::vector<Seed*>&
  getOwnedSeeds();  // Returns a reference to the vector of owned seeds
};

#endif
