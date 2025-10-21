#pragma once 
#include "CropState.h"

class Harvested : public CropState{

    public:
    Harvested(); // Constructor
    void updateGrowth(int);
    int getStatus();


};