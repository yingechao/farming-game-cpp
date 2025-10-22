#ifndef CROPSTATE_H
#define CROPSTATE_H

#include <ctime>
#include <string>
#include <vector>

using namespace std;

class CropState {
 protected:
  int growthProgress; //track the growth progress of the crop so harvest functions can be used by the user
  bool isHarvestable; //check if the crop is ready to be harvested

 public:
  string stateName;

  CropState(); // Constructor
  virtual void updateGrowth(int) = 0; //pure virtual function for updating the growth progress of the crop
  virtual ~CropState(); //virtual destructor
};

#endif

