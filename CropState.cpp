#include "CropState.h"
#include <iostream>

CropState::CropState() {
    growthProgress = 0;
    isHarvestable = false;
    stateName = "Unknown";
}


CropState::~CropState() {} 

