#ifndef CROPSTATE_H
#define CROPSTATE_H

#include <ctime>
#include <string>
#include <vector>

using namespace std;

class CropState {
 protected:
  int growthProgress;
  bool isHarvestable;

 public:
  string stateName;

  CropState(); // Constructor
  virtual void updateGrowth(int) = 0;
  virtual ~CropState();
};

#endif

