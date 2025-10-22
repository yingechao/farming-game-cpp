#ifndef PLOT_H
#define PLOT_H

#include "raylib.h"

// Forward declarations
class Seed;
class CropState;

// Plot struct for farming grid
struct Plot {
    Rectangle grid;           // draw rect (keep for GUI)
    Seed* plantedSeed;        // OOP: pointer to planted seed
    CropState* currentState;  // OOP: current crop state
    float timeRemaining;      // countdown until the crop is ready to be harvested

    void Clear(); 
    
    // Helper methods for GUI compatibility
    int getState() const;
    int getCropIndex() const;
};

#endif // PLOT_H
