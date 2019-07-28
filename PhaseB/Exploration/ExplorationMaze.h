#ifndef EXPLORATION_MAZE_H
#define EXPLORATION_MAZE_H

#include <algorithm>
#include "Print.h"
#include "Blackboard.h"
using namespace std;

// definition of MIN, MAX
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// rows and cols of map as we see it
#define ROWS 9 // Number of rows in maze
#define COLS 5 // Number of cols in maze

#define EXPLORATION_GRID_ROWS MAX(ROWS, COLS)
#define EXPLORATION_GRID_COLS 2 * MAX(ROWS, COLS) - 1

#define H_WALL_ROWS EXPLORATION_GRID_ROWS + 1
#define H_WALL_COLS EXPLORATION_GRID_COLS
#define V_WALL_ROWS EXPLORATION_GRID_ROWS
#define V_WALL_COLS EXPLORATION_GRID_COLS + 1

#define VERT_EXIST_VISUAL '|'
#define VERT_NO_EXIST_VISUAL ' '
#define VERT_UNEXPLORED_VISUAL '*'
#define HORI_EXIST_VISUAL "---"
#define HORI_NO_EXIST_VISUAL "   "
#define HORI_UNEXPLORED_VISUAL "***"
#define GRID_EXPLORED_VISUAL " "
#define GRID_UNEXPLORED_VISUAL "*"
#define ROBOT_VISUAL_FACING_NORTH "^"
#define ROBOT_VISUAL_FACING_SOUTH "v"
#define ROBOT_VISUAL_FACING_EAST ">"
#define ROBOT_VISUAL_FACING_WEST "<"
#define ROBOT_VISUAL_FACING_NONE "@"

enum GridStatus
{
    GRID_UNEXPLORED,
    GRID_EXPLORED
};

enum WallStatus
{
    WALL_UNEXPLORED,
    WALL_EXIST,
    WALL_NO_EXIST
};

class ExplorationMaze
{
  public:
    ExplorationMaze(Blackboard &blackboard) : blackboard(blackboard)
    {
        for (unsigned i = 0; i < EXPLORATION_GRID_ROWS; ++i)
        {
            for (unsigned j = 0; j < EXPLORATION_GRID_COLS; ++j)
            {
                explorationGrid[i][j] = GRID_UNEXPLORED;
            }
        }

        for (unsigned i = 0; i < H_WALL_ROWS; ++i)
        {
            for (unsigned j = 0; j < H_WALL_COLS; ++j)
            {
                horizontalWall[i][j] = WALL_UNEXPLORED;
            }
        }

        for (unsigned i = 0; i < V_WALL_ROWS; ++i)
        {
            for (unsigned j = 0; j < V_WALL_COLS; ++j)
            {
                verticalWall[i][j] = WALL_UNEXPLORED;
            }
        }

    }

    Blackboard &blackboard;
    GridStatus explorationGrid[EXPLORATION_GRID_ROWS][EXPLORATION_GRID_COLS];
    WallStatus verticalWall[V_WALL_ROWS][V_WALL_COLS];
    WallStatus horizontalWall[H_WALL_ROWS][H_WALL_COLS];

    void printMaze()
    {
        // print vert, hori, vert, hori, ...
        for (int row = 0; row < EXPLORATION_GRID_ROWS * 2 + 1; ++row)
        {
            if (row % 2 == 0)
                printHori(row / 2);
            else
                printVert(row / 2);
        }
    }

    void printHorizontalWallLine()
    {
        // print top wall
        for (int col = 0; col < H_WALL_COLS; ++col)
        {
            PRINT(" ");
            PRINT(HORI_EXIST_VISUAL);
        }
        PRINT("\n");
    }

    void printVert(int row)
    {
        for (int col = 0; col < V_WALL_COLS; ++col)
        {
            switch(verticalWall[row][col])
            {
                case(WALL_UNEXPLORED):
                {
                    PRINT(VERT_UNEXPLORED_VISUAL);
                    break;
                }
                case(WALL_EXIST):
                {
                    PRINT(VERT_EXIST_VISUAL);
                    break;
                }
                case(WALL_NO_EXIST):
                {
                    PRINT(VERT_NO_EXIST_VISUAL);
                    break;
                }
            }

            // if not last col, print whether we've explored the grid
            if (col < V_WALL_COLS - 1)
            {
                PRINT(" ");

                if (blackboard.x == col && blackboard.y == row)
                {
                    switch(blackboard.facingDirection)
                    {
                        case(NORTH): PRINT(ROBOT_VISUAL_FACING_NORTH); break;
                        case(SOUTH): PRINT(ROBOT_VISUAL_FACING_SOUTH); break;
                        case(EAST): PRINT(ROBOT_VISUAL_FACING_EAST); break;
                        case(WEST): PRINT(ROBOT_VISUAL_FACING_WEST); break;
                        default: PRINT(ROBOT_VISUAL_FACING_NONE); break;
                    }
                }
                else
                {
                    switch(explorationGrid[row][col])
                    {
                        case(GRID_UNEXPLORED):
                        {
                            PRINT(GRID_UNEXPLORED_VISUAL);
                            break;
                        }
                        case(GRID_EXPLORED):
                        {
                            PRINT(GRID_EXPLORED_VISUAL);
                            break;
                        }
                    }
                }
                PRINT(" ");
            }
        }
        PRINT("\n");
    }

    void printHori(int row)
    {
        for (int col = 0; col < H_WALL_COLS; ++col)
        {
            PRINT(" ");
            switch(horizontalWall[row][col])
            {
                case(WALL_UNEXPLORED):
                {
                    PRINT(HORI_UNEXPLORED_VISUAL);
                    break;
                }
                case(WALL_EXIST):
                {
                    PRINT(HORI_EXIST_VISUAL);
                    break;
                }
                case(WALL_NO_EXIST):
                {
                    PRINT(HORI_NO_EXIST_VISUAL);
                    break;
                }
            }
        }
        PRINT("\n");
    }
};

#endif // EXPLORATION_MAZE_H