

#ifndef STRAWBERRY_H
#define STRAWBERRY_H
#include <iostream>

#include "Seed.h"

class Strawberry : public Seed {
 public:
  Strawberry();            // Constructor
  void plant() override;   // Plant the seed
  void grow() override;    // Grow the seed
  int harvest() override;  // Harvest the seed
  void reset() override;   // Reset the seed
  Seed* clone() const override; // Clone the seed
};

#endif
