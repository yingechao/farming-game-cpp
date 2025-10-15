#include "Winter.h"

Winter::Winter() : Season("Winter", 60) {  // 60 seconds
  seeds.push_back(new Kale());
  seeds.push_back(new Beetroot());
  seeds.push_back(new Onion());
}