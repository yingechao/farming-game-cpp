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
  Seed(std::string seedName, float timeToGrow, int val);

  std::string get_Name() const;
  float get_GrowTime() const;
  int get_Value() const;
  int getPointsUnlockThreshold();
  bool get_HarvestStatus() const;
  bool isReadyToHarvest() const;
  virtual void plant();
  virtual void grow();
  virtual int harvest();
  virtual void reset();
  virtual ~Seed() {}
};

#endif
