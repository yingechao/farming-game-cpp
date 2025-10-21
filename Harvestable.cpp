#include "CropState.h"
#include "Harvestable.h"

Harvestable::Harvestable() {
    stateName = "Harvestable";
    growthProgress = 100;
    isHarvestable = true;
}

void Harvestable::updateGrowth(int growthProgress){
this->growthProgress = growthProgress;

}

int Harvestable::getStatus(){
    return growthProgress;
}













