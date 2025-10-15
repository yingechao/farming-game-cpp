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

    // Plant
    std::cout << "Type 'plant' to plant this seed: ";
    std::cin >> input;
    if (input != "plant") {
      std::cout << "You failed to plant the seed. Game Over!\n";
      game.endGame();
      break;
    }
    game.plantCurrentSeed();

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

    // Harvest points BEFORE advancing
    int earnedPoints = currentSeed->harvest();
    player.addPoints(earnedPoints);
    seasonPoints += earnedPoints;

    // Check if season is about to end (last seed)
    if (currentSeason->getSeeds().back() == currentSeed) {
      std::cout << "\nSeason " << currentSeason->get_Name()
                << " completed! Points this season: " << seasonPoints << "\n";
      seasonPoints = 0;  // reset for next season
    }

    // Advance to next seed or next season
    game.advanceSeed();
  }

  std::cout << "\n Game Over! Final Score: " << player.getPoints()
            << " points.\n";
  std::cout << "Thanks for playing, " << player.getName() << "!\n";

  return 0;
}
