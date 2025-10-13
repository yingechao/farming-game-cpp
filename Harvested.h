#pragma once 
#include "CropState.h"

class Harvested : public CropState{

    public:
    void updateGrowth(int);
    int getStatus();


};