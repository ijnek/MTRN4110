#ifndef BEHAVIOUR_NEXT_WAY_POINT_H
#define BEHAVIOUR_NEXT_WAY_POINT_H

#include "WayPointBehaviour.h"

class BehaviourNextWayPoint : public WayPointBehaviour
{
  public:
    BehaviourNextWayPoint()
    {
    }

    void reset()
    {
        clearWayPoints();
        WayPointBehaviour::reset();
    }
};

#endif // BEHAVIOUR_NEXT_WAY_POINT_H