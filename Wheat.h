#ifndef WHEAT_H
#define WHEAT_H

#include "Seed.h"

class Wheat : public Seed {
 public:
  Wheat();  // constructor declaration
  void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
  Seed* clone() const override;
};

#endif
