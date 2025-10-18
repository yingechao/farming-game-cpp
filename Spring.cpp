
#include "Spring.h"

Spring::Spring() : Season("Spring", 60) {  // 60 seconds
  // Seeds are loaded centrally by Season::loadSeeds(); only set
  // season-specific parameters here.
  requiredPoints = 60;
}