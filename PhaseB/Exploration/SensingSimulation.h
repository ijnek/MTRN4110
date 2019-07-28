#ifndef SENSING_SIMULATION_H
#define SENSING_SIMULATION_H

#include "ExplorationMaze.h"
#include "Blackboard.h"

#define HWALL "11111111111111111" \
              "00000000010110001" \
              "00000000000101000" \
              "00000000001000010" \
              "00000000010001010" \
              "11111111111111111" \
              "00000000000000000" \
              "00000000000000000" \
              "00000000000000000" \
              "11111111111111111" \

#define VWALL "100000001000011001" \
              "100000001100000001" \
              "100000001100010001" \
              "100000001011010001" \
              "100000001100000001" \
              "100000001000000001" \
              "100000001000000001" \
              "100000001000000001" \
              "100000001000000001" \


class SensingSimulation
{
  public:
    SensingSimulation(Blackboard &blackboard) : blackboard(blackboard), simulationMaze(blackboard)
    {
        // Create vertical wall
        int i = 0;
        for (int row = 0; row < V_WALL_ROWS; ++row)
        {
            for (int col = 0; col < V_WALL_COLS; ++col)
            {
                simulationMaze.verticalWall[row][col] = VWALL[i] == '1' ? WALL_EXIST : WALL_NO_EXIST;
                ++i;
            }
        }
    
        // Create horizontal wall
        i = 0;
        for (int row = 0; row < H_WALL_ROWS; ++row)
        {
            for (int col = 0; col < H_WALL_COLS; ++col)
            {
                simulationMaze.horizontalWall[row][col] = HWALL[i] == '1' ? WALL_EXIST : WALL_NO_EXIST;
                ++i;
            }
        }
    }

    void tick()
    {
        int x = blackboard.x;
        int y = blackboard.y;

        switch(blackboard.facingDirection)
        {
            case(NORTH):
            {
                blackboard.wallInFront = (simulationMaze.horizontalWall[y][x] == WALL_EXIST);
                blackboard.wallOnLeft = (simulationMaze.verticalWall[y][x] == WALL_EXIST);
                blackboard.wallOnRight = (simulationMaze.verticalWall[y][x+1] == WALL_EXIST);
                break;
            }
            case(SOUTH):
            {
                blackboard.wallInFront = (simulationMaze.horizontalWall[y+1][x] == WALL_EXIST);
                blackboard.wallOnLeft = (simulationMaze.verticalWall[y][x+1] == WALL_EXIST);
                blackboard.wallOnRight = (simulationMaze.verticalWall[y][x] == WALL_EXIST);
                break;
            }
            case(EAST):
            {
                blackboard.wallInFront = (simulationMaze.verticalWall[y][x+1] == WALL_EXIST);
                blackboard.wallOnLeft = (simulationMaze.horizontalWall[y][x] == WALL_EXIST);
                blackboard.wallOnRight = (simulationMaze.horizontalWall[y+1][x] == WALL_EXIST);
                break;
            }
            case(WEST):
            {
                blackboard.wallInFront = (simulationMaze.verticalWall[y][x] == WALL_EXIST);
                blackboard.wallOnLeft = (simulationMaze.horizontalWall[y+1][x] == WALL_EXIST);
                blackboard.wallOnRight = (simulationMaze.horizontalWall[y][x] == WALL_EXIST);
                break;
            }
        }
    }

    Blackboard &blackboard;
    ExplorationMaze simulationMaze;
};

#endif // SENSING_SIMULATION_H