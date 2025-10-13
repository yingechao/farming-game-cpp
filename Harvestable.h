#pragma once 
#include "CropState.h"

class Harvestable : public CropState{

    public:
    void updateGrowth(int);
    int getStatus();


};