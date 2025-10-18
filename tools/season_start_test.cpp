#include <iostream>
#include "Game.h"

int main() {
  Game g;
  g.startGame();
  Season* s = g.getCurrentSeason();
  if (s) std::cout << "TEST: Season " << s->get_Name()
                   << " required points: " << s->getRequiredPoints() << "\n";

  // advance through remaining seasons
  for (int i = 0; i < 3; ++i) {
    g.nextLevel();
    s = g.getCurrentSeason();
    if (s) std::cout << "TEST: Season " << s->get_Name()
                     << " required points: " << s->getRequiredPoints() << "\n";
  }
  return 0;
}
