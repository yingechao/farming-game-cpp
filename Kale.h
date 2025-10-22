

#ifndef KALE_H
#define KALE_H
#include <iostream>

#include "Seed.h"

class Kale : public Seed {
 public:
  // Constructor
  Kale();

   void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
  Seed* clone() const override;
};

#endif