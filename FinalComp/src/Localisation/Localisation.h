#ifndef LOCALISATION_H
#define LOCALISATION_H

#include "../Types/Blackboard.h"
#include "../Utils/MatrixMath/MatrixMath.h"

class Localisation
{
  public:
    Localisation(Blackboard &blackboard);
    void reset();
    void tick();

  private:
    Blackboard &blackboard;
    unsigned long lastTime;
    mtx_type state[3];
    mtx_type covariance[3][3];

    void predict(float forwardChange, float turnChange);
    void updateWithLidar(float lidarX, float lidarY, float lidarH, float measuredDistance);
};


#endif // LOCALISATION_H