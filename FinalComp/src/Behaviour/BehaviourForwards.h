#ifndef BEHAVIOUR_FORWARDS_H
#define BEHAVIOUR_FORWARDS_H

#include "WayPointBehaviour.h"
#include "../Utils/MathUtil/MathUtil.h"
#include "../Constants/MazeConstants.h"

// THIS IS QUITE A HACKY IMPLEMENTATION TBH, but it works

class BehaviourForwards : public WayPointBehaviour
{
  public:

    void reset()
    {
        WayPointBehaviour::reset();
        firstTime = true;
        clearWayPoints();
        addWayPoint(WayPoint(0, 0, 0)); // add a dummy waypoint
    }

    MovementRequest getMovementRequest(float myX, float myY, float myH)
    {
        if (firstTime)
        {
            float finalX = myX + CELL_SIZE * cos(myH);
            float finalY = myY + CELL_SIZE * sin(myH);
            Serial.println(finalX);
            Serial.println(finalY);
            clearWayPoints();
            addWayPoint(WayPoint(finalX, finalY, myH));
            firstTime = false;
        }

        return WayPointBehaviour::getMovementRequest(myX, myY, myH);       
    }

  private:
    bool firstTime;

};

#endif // BEHAVIOUR_FORWARDS_H