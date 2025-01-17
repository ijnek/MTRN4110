#ifndef EXPLORATION_MAZE_H
#define EXPLORATION_MAZE_H

#include "ExplorationEnums.h"

// definition of MIN, MAX
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// rows and cols of map as we see it
#define ROWS 9 // Number of rows in maze
#define COLS 5 // Number of cols in maze

#define EXPLORATION_GRID_ROWS (MAX(ROWS, COLS))
#define EXPLORATION_GRID_COLS (2 * MAX(ROWS, COLS) - 1)

#define H_WALL_ROWS (EXPLORATION_GRID_ROWS + 1)
#define H_WALL_COLS EXPLORATION_GRID_COLS
#define V_WALL_ROWS EXPLORATION_GRID_ROWS
#define V_WALL_COLS (EXPLORATION_GRID_COLS + 1)

// Always assume we start at the following position
#define EXPLORATION_STARTING_X (MAX(ROWS, COLS) - 1)
#define EXPLORATION_STARTING_Y 0
#define EXPLORATION_STARTING_FACING_DIRECTION SOUTH

#define VERT_EXIST_VISUAL '|'
#define VERT_NO_EXIST_VISUAL ' '
#define VERT_UNEXPLORED_VISUAL '*'
#define HORI_EXIST_VISUAL "---"
#define HORI_NO_EXIST_VISUAL "   "
#define HORI_UNEXPLORED_VISUAL "***"
#define GRID_EXPLORED_VISUAL " "
// #define GRID_UNEXPLORED_VISUAL "*"
#define GRID_UNEXPLORED_VISUAL " "
#define GRID_GOAL_LOCATION_VISUAL "X"
// #define ROBOT_VISUAL_FACING_NORTH "^"
// #define ROBOT_VISUAL_FACING_SOUTH "v"
// #define ROBOT_VISUAL_FACING_EAST ">"
// #define ROBOT_VISUAL_FACING_WEST "<"
#define ROBOT_VISUAL_FACING_NORTH "N"
#define ROBOT_VISUAL_FACING_SOUTH "S"
#define ROBOT_VISUAL_FACING_EAST "E"
#define ROBOT_VISUAL_FACING_WEST "W"
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
    ExplorationMaze()
    {
        reset();
    }

    void reset()
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

    GridStatus explorationGrid[EXPLORATION_GRID_ROWS][EXPLORATION_GRID_COLS];
    WallStatus verticalWall[V_WALL_ROWS][V_WALL_COLS];
    WallStatus horizontalWall[H_WALL_ROWS][H_WALL_COLS];

    void printMaze(int mapX, int mapY, Direction facingDirection, int startRow=0, int startCol=0, int endRow=EXPLORATION_GRID_ROWS-1, int endCol=EXPLORATION_GRID_COLS-1)
    {
        // print vert, hori, vert, hori, ...
        for (int row = startRow * 2; row < (endRow + 1) * 2 + 1; ++row)
        {
            if (row % 2 == 0)
                printHori(startCol, endCol, row / 2);
            else
                printVert(startCol, endCol, row / 2, mapX, mapY, facingDirection);
        }
    }

    String encodeHAndVWall(int mapX, int mapY, Direction facingDirection, int startRow=0, int startCol=0, int endRow=EXPLORATION_GRID_ROWS-1, int endCol=EXPLORATION_GRID_COLS-1)
    {

        // Serial.print("startRow: ");
        // Serial.println(startRow);
        // Serial.print("startCol: ");
        // Serial.println(startCol);
        // Serial.print("endROw: ");
        // Serial.println(endRow);
        // Serial.print("endCol: ");
        // Serial.println(endCol);

        String encodedString;
        encodedString += "H";

        for (int row = startRow; row <= endRow+1; ++row)
        {
            for (int col = startCol; col <= endCol; ++col)
            {
                switch(horizontalWall[row][col])
                {
                    case WALL_NO_EXIST:
                    {
                        encodedString += "0";
                        break;
                    }
                    case WALL_EXIST:
                    {
                        encodedString += "1";
                        break;
                    }
                    case WALL_UNEXPLORED:
                    {
                        encodedString += "2";
                        break;
                    }
                }
            }
        }

        encodedString += "V";
        for (int row = startRow; row <= endRow; ++row)
        {
            for (int col = startCol; col <= endCol+1; ++col)
            {
                switch(verticalWall[row][col])
                {
                    case WALL_NO_EXIST:
                    {
                        encodedString += "0";
                        break;
                    }
                    case WALL_EXIST:
                    {
                        encodedString += "1";
                        break;
                    }
                    case WALL_UNEXPLORED:
                    {
                        encodedString += "2";
                        break;
                    }
                }
            }
        }

        return encodedString;
    }


    void printHorizontalWallLine()
    {
        // print top wall
        for (int col = 0; col < H_WALL_COLS; ++col)
        {
            Serial.print(" ");
            Serial.print(HORI_EXIST_VISUAL);
        }
        Serial.print("\n");
    }

    void printVert(int startCol, int endCol, int row, int mapX, int mapY, Direction facingDirection)
    {
        for (int col = startCol; col <= endCol + 1; ++col)
        {
            switch(verticalWall[row][col])
            {
                case(WALL_UNEXPLORED):
                {
                    Serial.print(VERT_UNEXPLORED_VISUAL);
                    break;
                }
                case(WALL_EXIST):
                {
                    Serial.print(VERT_EXIST_VISUAL);
                    break;
                }
                case(WALL_NO_EXIST):
                {
                    Serial.print(VERT_NO_EXIST_VISUAL);
                    break;
                }
            }

            // if not last col, print whether we've explored the grid
            if (col != endCol + 1)
            {
                Serial.print(" ");

                if (mapX == col && mapY == row)
                {
                    switch(facingDirection)
                    {
                        case(NORTH): Serial.print(ROBOT_VISUAL_FACING_NORTH); break;
                        case(SOUTH): Serial.print(ROBOT_VISUAL_FACING_SOUTH); break;
                        case(EAST): Serial.print(ROBOT_VISUAL_FACING_EAST); break;
                        case(WEST): Serial.print(ROBOT_VISUAL_FACING_WEST); break;
                        default: Serial.print(ROBOT_VISUAL_FACING_NONE); break;
                    }
                }
                // HACK ALERT (kenji) : following else if statement hardcodes the "Goal position", REMOVE IF POSSIBLE
                else if (col == (endCol + startCol) / 2 && row == (MAX(ROWS, COLS) - 1) / 2)
                {
                    Serial.print(GRID_GOAL_LOCATION_VISUAL);
                }
                else
                {
                    switch(explorationGrid[row][col])
                    {
                        case(GRID_UNEXPLORED):
                        {
                            Serial.print(GRID_UNEXPLORED_VISUAL);
                            break;
                        }
                        case(GRID_EXPLORED):
                        {
                            Serial.print(GRID_EXPLORED_VISUAL);
                            break;
                        }
                    }
                }
                Serial.print(" ");
            }
        }
        Serial.print("\n");
    }

    void printHori(int startCol, int endCol, int row)
    {
        for (int col = startCol; col <= endCol; ++col)
        {
            Serial.print(" ");
            switch(horizontalWall[row][col])
            {
                case(WALL_UNEXPLORED):
                {
                    Serial.print(HORI_UNEXPLORED_VISUAL);
                    break;
                }
                case(WALL_EXIST):
                {
                    Serial.print(HORI_EXIST_VISUAL);
                    break;
                }
                case(WALL_NO_EXIST):
                {
                    Serial.print(HORI_NO_EXIST_VISUAL);
                    break;
                }
            }
        }
        Serial.print("\n");
    }

    void fillVerticalWall(int col)
    {
        for (unsigned i = 0; i < V_WALL_ROWS; ++i)
        {
            verticalWall[i][col] = WALL_EXIST;
        }
    }


    void fillHorizontalWall(int row)
    {
        for (unsigned i = 0; i < H_WALL_COLS; ++i)
        {
            horizontalWall[row][i] = WALL_EXIST;
        }
    }
};

#endif // EXPLORATION_MAZE_H