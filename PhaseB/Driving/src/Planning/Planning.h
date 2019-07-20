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
    bool headingClose;  // whether our heading is close, used for hysteresis purpose

    float distanceErrorSquared(float currentX, float currentY, float aimX, float aimY);
    bool distanceIsClose(float currentX, float currentY, float aimX, float aimY);
    float headingError(float currentX, float currentY, float currentH, float aimX, float aimY);
    bool headingIsClose(float currentX, float currentY, float currentH, float aimX, float aimY);
};


#endif // PLANNING_H