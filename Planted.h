#pragma once 
#include "CropState.h"

class Planted : public CropState{

    public:
    Planted(); // Constructor
    void updateGrowth(int);
    int getStatus();


};