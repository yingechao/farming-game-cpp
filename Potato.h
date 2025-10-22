

#ifndef POTATO_H
#define POTATO_H
#include <iostream>

#include "Seed.h"

class Potato : public Seed {
 public:
  // Constructor
  Potato();

   void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
  Seed* clone() const override;
};

#endif