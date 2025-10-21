#include "CropState.h"
#include <iostream>

void CropState::setGrowthProgress(int growthProgress) {
    this->growthProgress = growthProgress;
}

int CropState::getGrowthProgress() { return growthProgress; }

void CropState::updateGrowth() { ///////////////////do we use this function?
    // Simple default growth update: increment progress and set harvestable
    growthProgress++;
    if (growthProgress >= 100) isHarvestable = true;
}

void CropState::getStatus() {
    std::cout << "State: " << stateName << " | Progress: " << growthProgress
                        << " | Harvestable: " << (isHarvestable ? "yes" : "no") << "\n";
}

bool CropState::isDoneGrowing() { return isHarvestable == true; }

CropState::~CropState() {}

