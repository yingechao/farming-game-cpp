#ifndef TOMATO_H
#define TOMATO_H

#include "Seed.h"

class Tomato : public Seed {
 public:
  Tomato();
  void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
  Seed* clone() const override;
};

#endif