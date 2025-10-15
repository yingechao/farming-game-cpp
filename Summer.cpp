#include "Summer.h"

Summer::Summer() : Season("Summer", 60) {  // 60 seconds
  seeds.push_back(new Wheat());
  seeds.push_back(new Tomato());
  seeds.push_back(new Eggplant());
}