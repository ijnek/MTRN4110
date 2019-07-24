#ifndef SENSING_H
#define SENSING_H

#include "../IMU/IMU.h"
#include "../Blackboard/Blackboard.h"

class Sensing
{
  public:
    Sensing(Blackboard &blackboard) : blackboard(blackboard), imu(blackboard) {}

    void tick()
    {
        imu.tick();
    }

  private:
    IMU imu;
    Blackboard &blackboard;
};

#endif // SENSING_H