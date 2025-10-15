#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>

#include "Player.h"
#include "Season.h"

class Game {
 private:
  int currentLevel;
  int timeLimit;
  bool isGameOver;
  float getCurrentTime() const;
  float currentSeedStartTime;
  int currentSeedIndex = 0;
  int currentSeasonIndex = 0;

 public:
  Season* currentSeason;
  float startTime;
  Player player;

  Game();   //  constructor
  ~Game();  //  destructor

  void startGame();
  void startLevel();
  void nextLevel();
  void handleTimeOut();
  void updateTime();
  bool checkProgress();
  void endGame();

  bool isGameOverStatus() { return isGameOver; }
  bool isSeedTimeUp(float seedGrowTime) const;

  Seed* getCurrentSeed();
  void plantCurrentSeed();
  int harvestCurrentSeed();
  void advanceSeed();
  int getPlayerScore() const;

  Player& getPlayer();
  Season* getCurrentSeason();
  bool isTimeUp() const;
};

#endif
