<<<<<<< HEAD
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
=======
#pragma once
#include <string>
using namespace std;

class CropState{
protected:
string stateName;
int growthProgress;
bool isHarvestable;

public:
virtual void updateGrowth(int) = 0;
virtual int getStatus() = 0;
bool isDoneGrowing();
int getGrowthProgress();
void setGrowthProgress(int);
~CropState();
};
>>>>>>> 4e4301a90c6ad91264df8e9e6dec24ad821af004
