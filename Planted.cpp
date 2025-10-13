#include "CropState.h"
#include "Planted.h"

void Planted::updateGrowth(int growthProgress){
this->growthProgress = growthProgress;

}

int Planted::getStatus(){
    return growthProgress;
}