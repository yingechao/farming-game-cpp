#pragma once 
#include "CropState.h"

class Planted : public CropState{

    public:
    void updateGrowth(int);
    int getStatus();


};