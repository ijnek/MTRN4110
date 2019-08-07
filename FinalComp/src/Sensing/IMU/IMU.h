#ifndef IMU_H
#define IMU_H

#include "../../Types/Blackboard.h"

class IMU
{
  public:
    IMU(Blackboard &blackboard);
    void setup();
    void tick();

  private:
    Blackboard &blackboard;
    bool firstTime;

};

#endif // IMU_H