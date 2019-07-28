#ifndef BLACKBOARD_H
#define BLACKBOARD_H

enum Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NONE
};

class Blackboard
{
  public:
    Blackboard() : x(0), y(0), facingDirection(NONE), wallInFront(false), wallOnLeft(0), wallOnRight(0){}
    int x;
    int y;
    Direction facingDirection;
    bool wallInFront;
    bool wallOnLeft;
    bool wallOnRight;
};

#endif // BLACKBOARD_H