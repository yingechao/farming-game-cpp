#include "CropState.h"
#include "Harvestable.h"

void Harvestable::updateGrowth(int growthProgress){
this->growthProgress = growthProgress;

}

int Harvestable::getStatus(){
    return growthProgress;
}