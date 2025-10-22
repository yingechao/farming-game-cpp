#ifndef SEASON_H
#define SEASON_H

#include <string>
#include <vector>

#include "Seed.h"

using namespace std;

class Season {
 protected:
  vector<string> seedTypes;  //vector of seeds
  int requiredPoints;  // Points required to complete the season or unlock next
  std::string name;   
  int timeLimit;     
  std::vector<Seed*> seeds;  // list of seed objects in a specific season such as winter or summer
  int currentSeedIndex;
  bool completed = false;

 public:
  int levelNumber;

 
  Season(std::string name, int limit);  // Constructs a Season object with a given name and time limit

  ~Season();  // desctructor for Seed objects

  // Getters
  int getTimeLimit();            // Returns the season's time limit 
  vector<string> getSeedList();  // Returns a list of all seed type names available in this season
  int getRequiredPoints();  // Returns the number of points required to completethe season
  std::string get_Name();   // Return season name

  void update();        // Updates the state of the season (seed progress, whether the season is completed)
  void resetSeason();   // Resets the season to its initial state is the user wants to replay the season
  void loadSeeds();     // Loads all Seed objects associated with a specific season
  bool hasMoreSeeds();  // checks if more seeds left to be grown or harvested, determines whether the player can move to the next season
  Seed* getCurrentSeed();    // Returns a pointer to the currently active Seed
  Seed* unlockNextSeed();    // unlocks the next seed in the list and return
                             // pointer  to the next Seed object
  bool allSeedsCompleted();  // Checks if all seeds in this season have been harvested
  bool isCurrentSeedAvailable();  // checks if seed is unlocked

  std::vector<Seed*>& getSeeds();  // Provides access to the list of Seed objects in this season
               // return ref to the vector of seeds

  bool isSeedTimeUp(
      float elapsedTime);  // return True if time is up, otherwise false

  bool isCompleted();  // Returns true if season ended
};

#endif
