#include "CropState.h"
#include <iostream>

CropState::CropState() {
    growthProgress = 0;
    isHarvestable = false;
    stateName = "Unknown";
}

// Pure virtual function - implemented by derived classes
// void CropState::updateGrowth(int) = 0;

CropState::~CropState() {}

