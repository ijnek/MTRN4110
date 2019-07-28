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

enum StartingPose
{
    LeftTopFacingRight,
    LeftTopFacingDown,
    RightTopFacingLeft,
    RightTopFacingDown,
    Unknown
};

class Blackboard
{
  public:
    Blackboard() : x(0), y(0), facingDirection(NONE), wallInFront(false), wallOnLeft(0), wallOnRight(0), startingPose(Unknown), reachedGoal(false){}
    int x;
    int y;
    Direction facingDirection;
    bool wallInFront;
    bool wallOnLeft;
    bool wallOnRight;
    StartingPose startingPose;
    bool reachedGoal;
};

#endif // BLACKBOARD_H