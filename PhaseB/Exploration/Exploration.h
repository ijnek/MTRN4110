#ifndef EXPLORATION_H
#define EXPLORATION_H

#include "Vector.h"
#include "SensingSimulation.h"
#include "Blackboard.h"

#define MAX_BACKTRACK_MOVES 100


class Exploration
{
  public:
    Exploration(Blackboard &blackboard) 
      : backTrackMoves(backTrackMovesArray),
        sensingSimulation(blackboard),
        blackboard(blackboard),
        explorationMaze(blackboard)
    {
        sensingSimulation.simulationMaze.printMaze();
    }
    void tick();

    Direction backTrackMovesArray[MAX_BACKTRACK_MOVES];  // Allocate some memory to use the vector
    Vector<Direction> backTrackMoves;
    SensingSimulation sensingSimulation;
    Blackboard &blackboard;
    ExplorationMaze explorationMaze;
};

#endif // EXPLORATION_H