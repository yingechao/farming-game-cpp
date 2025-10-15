#ifndef CROPSTATE_H
#define CROPSTATE_H

#include <ctime>
#include <string>
#include <vector>

using namespace std;

class CropState {
 private:
  int growthProgress;
  bool isHarvestable;

 public:
  string stateName;

  int getGrowthProgress();
  void updateGrowth();
  void getStatus();
  bool isDoneGrowing();
  virtual ~CropState();
};

#endif
