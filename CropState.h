#pragma once
#include <string>
using namespace std;

class CropState{
protected:
string stateName;
int growthProgress;
bool isHarvestable;

public:
virtual void updateGrowth(int) = 0;
virtual int getStatus() = 0;
bool isDoneGrowing();
int getGrowthProgress();
void setGrowthProgress(int);
~CropState();
};