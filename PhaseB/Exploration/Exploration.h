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
        explorationMaze(blackboard),
        goal_6_4_reached(false),
        goal_12_2_reached(false),
        goal_10_4_reached(false),
        goal_4_2_reached(false)
    {
        sensingSimulation.simulationMaze.printMaze();
    }
    void tick();

    Direction backTrackMovesArray[MAX_BACKTRACK_MOVES];  // Allocate some memory to use the vector
    Vector<Direction> backTrackMoves;
    SensingSimulation sensingSimulation;
    Blackboard &blackboard;
    ExplorationMaze explorationMaze;

    bool goal_6_4_reached;
    bool goal_12_2_reached;
    bool goal_10_4_reached;
    bool goal_4_2_reached;
};

#endif // EXPLORATION_H