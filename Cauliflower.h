#ifndef CAULIFLOWER_H
#define CAULIFLOWER_H
#include <iostream>

#include "Seed.h"

class Cauliflower : public Seed {
 public:
  // Constructor
  Cauliflower();

  void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
};

#endif