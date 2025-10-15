#ifndef SEASON_H
#define SEASON_H

#include <string>
#include <vector>

#include "Seed.h"

using namespace std;

class Season {
 protected:
  vector<string> seedTypes;  // Stores all available seed types
  int requiredPoints;
  std::string name;
  int timeLimit;             // seconds or minutes
  std::vector<Seed*> seeds;  // list of seed objects in the season
  int currentSeedIndex;
  bool completed = false;

 public:
  int levelNumber;

  // Constructor / Destructor
  Season(std::string name, int limit);
  ~Season();

  // Getters
  int getTimeLimit();
  vector<string> getSeedList();
  int getRequiredPoints();
  std::string get_Name();

  // Season methods
  void start_Season();
  void update();
  void resetSeason();
  void loadSeeds();        // loads the season's seeds
  bool hasMoreSeeds();     // checks if more seeds left
  Seed* getCurrentSeed();  // returns the current seed to work on
  Seed* unlockNextSeed();  // unlocks the next seed and return pointer
  bool allSeedsCompleted();
  bool isCurrentSeedAvailable();  // checks if current seed is unlocked

  std::vector<Seed*>& getSeeds();

  bool isSeedTimeUp(float elapsedTime);

  void advanceSeed();

  void endSeason();    // Marks the season as completed
  bool isCompleted();  // Returns true if season ended
};

#endif
