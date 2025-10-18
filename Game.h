#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include "Seed.h"
#include "Player.h"
#include "Season.h"

class Game {
 private:
  int currentLevel;
  int timeLimit;
  bool isGameOver;
  double getCurrentTime() const;
  double currentSeedStartTime;
  int currentSeedIndex = 0;
  int currentSeasonIndex = 0;
  Seed* currentSeed;
  int seasonStartScore;

 public:
  Season* currentSeason;
  std::chrono::steady_clock::time_point startTime;
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
  bool isSeedTimeUp(double seedGrowTime) const;

  Seed* getCurrentSeed();
  Seed* selectNewSeed(int a);

  void plantCurrentSeed();
  int harvestCurrentSeed();
  void advanceSeed();
  int getPlayerScore() const;

  Player& getPlayer();
  Season* getCurrentSeason();
  bool isTimeUp() const;
};

#endif
