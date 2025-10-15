#include "Autumn.h"


Autumn::Autumn() : Season("Autumn", 60) {  // 60 seconds
  seeds.push_back(new Carrot());
  seeds.push_back(new Lettuce());
  seeds.push_back(new Peas());
}