#ifndef SEED_H
#define SEED_H

#include <string>

class Seed {
 protected:
  std::string name;
  float growTime;
  int value;
  bool isHarvested;
  int pointsUnlockThreshold;

 public:
  //  Constructs a Seed object with a name, growth time, and value.
  Seed(std::string seedName, float timeToGrow, int val);

  // Returns the name of the seed
  std::string get_Name() const;

  // Returns the time required for the seed to grow.
  float get_GrowTime() const;
  // Returns the point  of the seed.
  int get_Value() const;

  // Checks whether the seed has been harvested.
  bool get_HarvestStatus() const;
  // Determines whether the seed is ready for harvest.
  bool isReadyToHarvest() const;

  // pure virtual function for planting the seed
  virtual void plant() = 0;
  // pure virtual function for growing the seed
  virtual void grow() = 0;
  // pure virtual function for harvesting the seed
  virtual int harvest() = 0;
  // pure virtual function for resetting the seed's state
  virtual void reset() = 0;
  
  // Creates a copy of this seed for planting. this creates a new instance of the seed so plots do not share the same object
  // clone function for memory management ---- cloned seeds are owned by each plot. the plot::clear() function will delete the cloned seed
  virtual Seed* clone() const = 0;

  // Returns the points threshold needed for unlocking the seed
  int getPointsUnlockThreshold() const;

  // Virtual destructor 
  virtual ~Seed() {}
};

#endif
