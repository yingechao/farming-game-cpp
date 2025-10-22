

#ifndef LETTUCE_H
#define LETTUCE_H
#include <iostream>

#include "Seed.h"

class Lettuce : public Seed {
 public:
  // Constructor
  Lettuce();

   void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
  Seed* clone() const override;
};

#endif