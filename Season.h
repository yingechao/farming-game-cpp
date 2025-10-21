#ifndef SEASON_H
#define SEASON_H

#include <string>
#include <vector>

#include "Seed.h"

using namespace std;

class Season {
 protected:
  vector<string> seedTypes;  // Stores the names/types of all available seeds
                             // for this season
  int requiredPoints;  // Points required to complete the season or unlock next
                       // stage
  std::string name;    // Name of the season (e.g., "Spring", "Summer")
  int timeLimit;       // seconds or minutes
  std::vector<Seed*> seeds;  // list of seed objects in the season
  int currentSeedIndex;
  bool completed = false;

 public:
  int levelNumber;

  // Constructor / Destructor
  Season(std::string name, int limit);  // Constructs a Season object with a
                                        // given name and time limit

  ~Season();  // cleans up allocated Seed objects

  // Getters
  int getTimeLimit();            // Returns the time limit for the season.
  vector<string> getSeedList();  // Returns a list of all seed type names
                                 // available in this season.
  int getRequiredPoints();  // Returns the number of points required to complete
                            // the season.
  std::string get_Name();   // Returns the name of the season

  // Season methods
  void start_Season();  // Starts the season, initializing timers and loading
                        // available seeds.
  void update();        // Updates the state of the season (e.g., checks growth
                        // progress or timers).
  void resetSeason();   // Resets the season to its initial state for replaying
  void loadSeeds();     // Loads all Seed objects associated with this season.
  bool hasMoreSeeds();  // checks if more seeds left to be grown or harvested
  Seed* getCurrentSeed();    // Returns a pointer to the currently active Seed.
  Seed* unlockNextSeed();    // unlocks the next seed in the list and return
                             // pointer  to the next Seed object
  bool allSeedsCompleted();  // Checks if all seeds in this season have been
                             // completed (harvested)
  bool isCurrentSeedAvailable();  // checks if current seed is unlocked

  std::vector<Seed*>&
  getSeeds();  // Provides access to the list of Seed objects in this season.
               // return Reference to the vector of seeds

  bool isSeedTimeUp(
      float elapsedTime);  // return True if time is up, otherwise false.

  void advanceSeed();  // Advances the index to the next seed in the season.

  void endSeason();    // Marks the season as completed
  bool isCompleted();  // Returns true if season ended
};

#endif
