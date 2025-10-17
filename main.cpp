#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Game.h"

int main() {
  Game game;
  Player& player = game.getPlayer();

  // Get player name
  std::string name;
  std::cout << "Enter your name: ";
  std::cin >> name;
  player.setName(name);
  std::cout << "\nWelcome, " << player.getName() << "! Let's start the game.\n";

  game.startGame();  // starts Spring

  int seasonPoints = 0;

  while (!game.isGameOverStatus()) {
    Seed* currentSeed = game.getCurrentSeed();
    Season* currentSeason = game.getCurrentSeason();
    if (!currentSeed || !currentSeason) break;

    std::cout << "\nSelected seed: " << currentSeed->get_Name()
              << " - Grow time: " << currentSeed->get_GrowTime()
              << " seconds\n";

    std::string input;

    // Show the season's available seeds (this matches Game's internal ordering)
    auto* season = game.getCurrentSeason();
    std::vector<Seed*>& seeds = season->getSeeds();
    std::cout << "Available seeds (choose index): ";
    for (size_t i = 0; i < seeds.size(); ++i) {
      if (seeds[i])
        std::cout << seeds[i]->get_Name() << " (" << (i + 1) << ") ";
      else
        std::cout << "(null) (" << (i + 1) << ") ";
    }
    //////////////

    std::cout << std::endl;

    // while loop to select and replant certain seeds
    bool flagOn = true;
    while (flagOn == true) {
      std::cout << "Type 'plant' to plant this seed, or enter the index "
                   "indicated in brackets () to select another seed: ";
      std::cin >> input;

      // Validate input
      if (input != "plant" && input != "1" && input != "2" && input != "3") {
        std::cout << "You failed to plant the seed. Game Over!\n";
        game.endGame();
        break;
      }

      if (input == "plant" && seasonPoints >= currentSeed->getPointsUnlockThreshold()) {
        // Plant whatever is currently selected in the Game
        game.plantCurrentSeed();
        flagOn = false;
      } else if (input == "1" || input =="2" || input == "3") {
        // Numeric selection: update the game's current seed (1-based index)
        int idx = std::stoi(input);
        Seed* sel = game.selectNewSeed(idx);
        if (sel) {
          if (seasonPoints >= sel->getPointsUnlockThreshold()){
          currentSeed = sel;
          std::cout << "Selected seed: " << currentSeed->get_Name() << "\n";
          }else {
            std::cout << "This seed is locked. Earn more points to unlock. " << std::endl;
          }
        }
       } else if (input =="plant"){
          std::cout << "Invalid selection. Please choose another seed. \n";
        }
        // continue the loop so user can type 'plant' when ready
      }
    
    // Grow
    std::cout << "Type 'grow' to grow the seed: ";
    std::cin >> input;
    if (input != "grow") {
      std::cout << "You failed to grow the seed. Game Over!\n";
      game.endGame();
      break;
    }
    currentSeed->grow();
    std::this_thread::sleep_for(
        std::chrono::seconds(static_cast<int>(currentSeed->get_GrowTime())));

    // Harvest
    std::cout << "Type 'harvest' to harvest the seed: ";
    std::cin >> input;
    if (input != "harvest") {
      std::cout << "You failed to harvest the seed. Game Over!\n";
      game.endGame();
      break;
    }
    // Harvest and award points via the Game API (centralized)
    int earnedPoints = game.harvestCurrentSeed();
    seasonPoints += earnedPoints;

      // If all seeds in the current season have been harvested, advance to
      // the next season immediately. 
      if (game.checkProgress()) {
        Season* completedSeason = game.getCurrentSeason();
        std::cout << "\nSeason " << (completedSeason ? completedSeason->get_Name() : std::string("?"))
                  << " completed! Points this season: " << seasonPoints << "\n";
        player.resetProgress();
        seasonPoints = 0;  // reset for next season
        game.nextLevel();
        continue;  // start loop for the new season
      }

      // Advance to next seed in the current season
      game.advanceSeed();
  }

  std::cout << "\n Game Over! Final Score: " << player.getPoints()
            << " points.\n";
  std::cout << "Thanks for playing, " << player.getName() << "!\n";

  return 0;
}
