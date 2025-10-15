
#include "Spring.h"

Spring::Spring() : Season("Spring", 60) {  // 60 seconds
  seeds.push_back(new Potato());
  seeds.push_back(new Strawberry());
  seeds.push_back(new Cauliflower());
}