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

  // Simulates planting the seed. Can be overridden for specific seed types
  virtual void plant();
  // Simulates the seed's growth over time. Can be customized by child classes
  virtual void grow();
  // Simulates harvesting the seed. Returns its value
  virtual int harvest();
  // Resets the seed's state
  virtual void reset();

  // Virtual destructor ensures proper cleanup for derived classes
  virtual ~Seed() {}
};

#endif
