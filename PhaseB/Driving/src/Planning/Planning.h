#ifndef PLANNING_H
#define PLANNING_H

#include "../Blackboard/Blackboard.h"
#include "../WayPoint/WayPoint.h"
#include "../Vector/Vector.h"

class Planning
{
  public:
    Planning(Blackboard &blackboard);
    void tick();

  private:
    Blackboard &blackboard;
    Vector<WayPoint> wayPoints;
    int wayPointIndex;

    float distanceErrorSquared(float currentX, float currentY, float aimX, float aimY);
    bool distanceClose(float currentX, float currentY, float aimX, float aimY);
    float headingError(float currentX, float currentY, float currentH, float aimX, float aimY);
    bool headingClose(float currentX, float currentY, float currentH, float aimX, float aimY);
};


#endif // PLANNING_H