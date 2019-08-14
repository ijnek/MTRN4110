#ifndef NETWORKING_CONSTANTS_H
#define NETWORKING_CONSTANTS_H

// Bluetooth communication / Behaviours
#define BEHAVIOUR_NONE 0          // don't do anything
#define BEHAVIOUR_TURN_LEFT 1     // turn left 90 degrees
#define BEHAVIOUR_TURN_RIGHT 2    // turn right 90 degrees
#define BEHAVIOUR_FORWARDS 3      // go forwards
#define BEHAVIOUR_NEXT_WAYPOINT 4 // plan using the "nextWayPoint" variable on the blackboard
#define BEHAVIOUR_PLANNED_ROUTE 5

#endif // NETWORKING_CONSTANTS_H