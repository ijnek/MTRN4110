#ifndef EXPLORATION_H
#define EXPLORATION_H

#include "../Vector/Vector.h"
#include "../Blackboard/Blackboard.h"
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
        startingPose(Unknown)
    {
    }
    void tick();

    Direction backTrackMovesArray[MAX_BACKTRACK_MOVES];  // Allocate some memory to use the vector
    Vector<Direction> backTrackMoves;
    Blackboard &blackboard;
    ExplorationMaze explorationMaze;
    int mapX;
    int mapY;
    Direction facingDirection;
    StartingPose startingPose;
};

#endif // EXPLORATION_H