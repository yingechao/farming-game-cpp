#ifndef EGGPLANT_H
#define EGGPLANT_H
#include "Seed.h"

class Eggplant : public Seed {
 public:
  Eggplant();

  void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
};

#endif