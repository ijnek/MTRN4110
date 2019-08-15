#ifndef EXPLORATION_H
#define EXPLORATION_H

#include "../Utils/Vector/Vector.h"
#include "../Types/Blackboard.h"
#include "ExplorationMaze.h"
#include "ExplorationEnums.h"

#define MAX_BACKTRACK_MOVES 100


class Exploration
{
  public:
    Exploration(Blackboard &blackboard) 
      : backTrackMoves(backTrackMovesArray),
        blackboard(blackboard),
        mapX(EXPLORATION_STARTING_X),
        mapY(EXPLORATION_STARTING_Y),
        facingDirection(EXPLORATION_STARTING_FACING_DIRECTION),
        startingPose(Unknown),
        prevMapX(mapX),
        prevMapY(mapY),
        goal_6_4_reached(false),
        goal_12_2_reached(false),
        goal_10_4_reached(false),
        goal_4_2_reached(false)
    {
    }
    void tick();
    void reset();

    Direction backTrackMovesArray[MAX_BACKTRACK_MOVES];  // Allocate some memory to use the vector
    Vector<Direction> backTrackMoves;
    Blackboard &blackboard;
    ExplorationMaze explorationMaze;
    int mapX;
    int mapY;
    Direction facingDirection;
    StartingPose startingPose;

    int prevMapX;
    int prevMapY;

    bool goal_6_4_reached;
    bool goal_12_2_reached;
    bool goal_10_4_reached;
    bool goal_4_2_reached;
};

#endif // EXPLORATION_H