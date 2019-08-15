#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "ExplorationEnums.h"
#include <string>

class Blackboard
{
  public:
    Blackboard() : x(0), y(0), facingDirection(NONE), wallInFront(false), wallOnLeft(0), wallOnRight(0), reachedGoal(false){}
    int x;
    int y;
    Direction facingDirection;
    bool wallInFront;
    bool wallOnLeft;
    bool wallOnRight;
    bool reachedGoal;
    std::string encodedMaze;
};

#endif // BLACKBOARD_H