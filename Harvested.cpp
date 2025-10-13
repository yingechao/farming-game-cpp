#include "CropState.h"
#include "Harvested.h"

void Harvested::updateGrowth(int growthProgress){
this->growthProgress = growthProgress;

}

int Harvested::getStatus(){
    return growthProgress;
}