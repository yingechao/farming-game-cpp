#ifndef TOMATO_H
#define TOMATO_H

#include "Seed.h"

class Tomato : public Seed {
 public:
  Tomato() : Seed("Tomato", 6, 15) {}
  void grow() override;
  int harvest() override;
  void reset() override;
  void plant() override;
};

#endif