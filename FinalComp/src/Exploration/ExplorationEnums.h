#ifndef EXPLORATION_ENUMS_H
#define EXPLORATION_ENUMS_H

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

#endif // EXPLORATION_ENUMS_H