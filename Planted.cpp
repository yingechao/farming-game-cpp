#include "CropState.h"
#include "Planted.h"

Planted::Planted() {
    stateName = "Planted";
    growthProgress = 0;
    isHarvestable = false;
}

void Planted::updateGrowth(int growthProgress){
this->growthProgress = growthProgress;

}

int Planted::getStatus(){
    return growthProgress;
}