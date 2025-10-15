

#ifndef CARROT_H
#define CARROT_H
#include <iostream>

#include "Seed.h"

class Carrot : public Seed {
 public:
  // Constructor
  Carrot();

   void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
};

#endif