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
  std::vector<Seed*> seedsOwned;  // pointers to seeds (not owned)
  std::vector<Seed*> harvestedCrops;

 public:
  string name;
  Seed* currentSeed;

  Player();

  void addPoints(int pointsToAdd);
  void deductPoints(int pointsToDeduct);
  bool hasCompletedSeason();

  int getPoints() const;
  std::string getName();
  void setName(const std::string& playerName);
  void selectSeed(string seedType);

  void plantSeed();
  void growCrop();
  void harvestCrop();
  void resetProgress();  // Clears references, not delete seeds

  void addSeed(Seed* s);
  void setSeeds(const std::vector<Seed*>& seeds);
  std::vector<Seed*>& getOwnedSeeds();
};

#endif
