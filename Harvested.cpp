#include "CropState.h"
#include "Harvested.h"

Harvested::Harvested() {
    stateName = "Harvested";
    growthProgress = 100;
    isHarvestable = true;
}

void Harvested::updateGrowth(int growthProgress){
this->growthProgress = growthProgress;

}

int Harvested::getStatus(){
    return growthProgress;
}