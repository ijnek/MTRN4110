#ifndef IMU_H
#define IMU_H

#include "../Blackboard/Blackboard.h"

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