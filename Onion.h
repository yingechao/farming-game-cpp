

#ifndef ONION_H
#define ONION_H
#include <iostream>

#include "Seed.h"

class Onion : public Seed {
 public:
  // Constructor
  Onion();

   void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
};

#endif