#pragma once 
#include "CropState.h"

class Planted : public CropState{ //public inheritance from CropState

    public:
    Planted(); // Constructor
    void updateGrowth(int); //update growth progress from CropState class
    int getStatus(); //get growth progress
};