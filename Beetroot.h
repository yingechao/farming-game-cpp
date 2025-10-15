

#ifndef BEETROOT_H
#define BEETROOT_H
#include <iostream>

#include "Seed.h"

class Beetroot : public Seed {
 public:
  // Constructor
  Beetroot();

   void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
};

#endif