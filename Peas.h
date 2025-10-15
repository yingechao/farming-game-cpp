

#ifndef PEAS_H
#define PEAS_H
#include <iostream>

#include "Seed.h"

class Peas : public Seed {
 public:
  // Constructor
  Peas();

   void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
};

#endif