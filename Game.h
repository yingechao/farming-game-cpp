#ifndef GAME_HPP
#define GAME_HPP

// Chrono library, which provides time-related utilities.
#include <chrono>

#include "Player.h"
#include "Season.h"
#include "Seed.h"

class Game {
 private:
  int currentLevel;
  int timeLimit;
  bool isGameOver;
  float getCurrentTime() const;  // gets current time
  float currentSeedStartTime;
  int currentSeedIndex = 0;
  int currentSeasonIndex = 0;
  Seed* currentSeed;  // currentSeed is a pointer to an object of type
                      // Seed and declares the pointer

 public:
  Season* currentSeason;  // currentSeason is a pointer to an object of type
                          // Season and declares the pointer
  float startTime;
  Player player;  // declaring an object named player of type Player

  Game();   //  constructor , runs automatically when an object of the class is
            //  created,
            // it is used to allocate memory when object is created
  ~Game();  //  destructor,runs automatically when an object is destroyed, it is
            //  used to free memory

  void startGame();      // Initializes player, sets level (Spring)
  void startLevel();     // Loads the current season, starts timer
  void nextLevel();      // Moves to next season
  void handleTimeOut();  // handles when time is up(called when time expires)
  void updateTime();     // tracks time remaining
  bool checkProgress();  // Checks if all seeds are planted, grown, harvested
  void endGame();        // ends the game

  bool isGameOverStatus() {
    return isGameOver;
  }  // returns if game is over or not
  bool isSeedTimeUp(float seedGrowTime)
      const;  // passes seedGrowTimeUp to check if seed time is up or not

  Seed* getCurrentSeed();    // function returns a pointer to a Seed object
  void plantCurrentSeed();   // declares a function that performs the action of
                             // planting the current seed
  int harvestCurrentSeed();  //  this function performs the “harvest” action for
                             //  the currently selected seed and then returns an
                             //  integer
  void advanceSeed();  // function declaration that will be used to move to the
                       // next seed in current season
  int getPlayerScore()
      const;  // declares a read-only function that returns the player’s score
              // as an integer and doesn’t modify the object’s data

  Player& getPlayer();         // returns a reference to a Player object
  Season* getCurrentSeason();  // function returns a pointer to a Season object
  bool isTimeUp() const;  // returns true or false to check if time is up or not
};

#endif
