#pragma once 
#include "CropState.h"

class Harvestable : public CropState{

    public:
    Harvestable(); // Constructor
    void updateGrowth(int);
    int getStatus();


};