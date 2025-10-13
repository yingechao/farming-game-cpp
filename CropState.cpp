#include "CropState.h"

void CropState::setGrowthProgress(int growthProgress){
    this->growthProgress = growthProgress;
}

int CropState::getGrowthProgress(){
    return growthProgress;
}

bool CropState::isDoneGrowing(){
if (isHarvestable = true){
    return true; //returns true if the crop is finished growing
}
return false; //returns false otherwise
}

