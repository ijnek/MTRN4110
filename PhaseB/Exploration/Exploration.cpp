#include "Exploration.h"
#include "Vector.h"
#include "ExplorationMaze.h"
#include <iostream>
#include "Planning.hpp"

#define MAX_POSSIBLE_MOVE_DIRECTIONS 4


Direction lastMove = SOUTH;
bool wallOnLeft = false;
bool wallInFront = false;
bool wallOnRight = false;

void Exploration::tick()
{
    int x = blackboard.x;
    int y = blackboard.y;

    // update our map
    explorationMaze.explorationGrid[y][x] = GRID_EXPLORED;
    switch(blackboard.facingDirection)
    {
        case(NORTH):
        {
            explorationMaze.horizontalWall[y][x] = blackboard.wallInFront ? WALL_EXIST : WALL_NO_EXIST;
            explorationMaze.verticalWall[y][x] = blackboard.wallOnLeft ? WALL_EXIST : WALL_NO_EXIST;
            explorationMaze.verticalWall[y][x+1] = blackboard.wallOnRight ? WALL_EXIST : WALL_NO_EXIST;
            break;
        }
        case(SOUTH):
        {
            explorationMaze.horizontalWall[y+1][x] = blackboard.wallInFront ? WALL_EXIST : WALL_NO_EXIST;
            explorationMaze.verticalWall[y][x+1] = blackboard.wallOnLeft ? WALL_EXIST : WALL_NO_EXIST;
            explorationMaze.verticalWall[y][x] = blackboard.wallOnRight ? WALL_EXIST : WALL_NO_EXIST;
            break;
        }
        case(EAST):
        {
            explorationMaze.verticalWall[y][x+1] = blackboard.wallInFront ? WALL_EXIST : WALL_NO_EXIST;
            explorationMaze.horizontalWall[y][x] = blackboard.wallOnLeft ? WALL_EXIST : WALL_NO_EXIST;
            explorationMaze.horizontalWall[y+1][x] = blackboard.wallOnRight ? WALL_EXIST : WALL_NO_EXIST;
            break;
        }
        case(WEST):
        {
            explorationMaze.verticalWall[y][x] = blackboard.wallInFront ? WALL_EXIST : WALL_NO_EXIST;
            explorationMaze.horizontalWall[y+1][x] = blackboard.wallOnLeft ? WALL_EXIST : WALL_NO_EXIST;
            explorationMaze.horizontalWall[y][x] = blackboard.wallOnRight ? WALL_EXIST : WALL_NO_EXIST;
            break;
        }
    }

    // print the updated maze
    explorationMaze.printMaze();
    PRINT("\n");

    // check if we've figured out our starting pose, depending on the updates
    if (blackboard.startingPose == Unknown)
    {
        if (x >= EXPLORATION_STARTING_X + (MIN(ROWS, COLS) - 1))
        {
            if (explorationMaze.verticalWall[y][x+1] == WALL_NO_EXIST)
            {
                blackboard.startingPose = LeftTopFacingDown;
            }
        }
        if (x <= EXPLORATION_STARTING_X - (MIN(ROWS, COLS) - 1))
        {
            if (explorationMaze.verticalWall[y][x] == WALL_NO_EXIST)
            {
                blackboard.startingPose = RightTopFacingDown;
            }
        }
        if (y >= EXPLORATION_STARTING_Y + (MIN(ROWS, COLS) - 1) && x > EXPLORATION_STARTING_X)
        {
            if (explorationMaze.verticalWall[y+1][x] == WALL_NO_EXIST)
            {
                blackboard.startingPose = RightTopFacingLeft;
            }
        }
        if (y >= EXPLORATION_STARTING_Y + (MIN(ROWS, COLS) - 1) && x < EXPLORATION_STARTING_X)
        {
            if (explorationMaze.verticalWall[y+1][x] == WALL_NO_EXIST)
            {
                blackboard.startingPose = LeftTopFacingRight;
            }
        }
    }

    // check if we've accidentaly reached any of the goals
    if (blackboard.x == 6 && blackboard.y == 4)
    {
        goal_6_4_reached = true;
    }
    else if (blackboard.x == 12 && blackboard.y == 2)
    {
        goal_12_2_reached = true;
    }
    else if (blackboard.x == 10 && blackboard.y == 4)
    {
        goal_10_4_reached = true;
    }
    else if (blackboard.x == 4 && blackboard.y == 2)
    {
        goal_4_2_reached = true;
    }

    if (!blackboard.reachedGoal)
    {
        // If we know our starting pose, then we know if we're at the goal
        switch (blackboard.startingPose)
        {
            case(LeftTopFacingDown):
            {
                if (x == EXPLORATION_STARTING_X + (MAX(ROWS, COLS) - 1) / 2 &&
                    y == EXPLORATION_STARTING_Y + (MIN(ROWS, COLS) - 1) / 2)
                {
                    PRINT("GOAL\n");
                    blackboard.reachedGoal = true;
                }
                break;
            }
            case(LeftTopFacingRight):
            {
                if (x == EXPLORATION_STARTING_X - (MIN(ROWS, COLS) - 1) / 2 &&
                    y == EXPLORATION_STARTING_Y + (MAX(ROWS, COLS) - 1) / 2)
                {
                    PRINT("GOAL\n");
                    blackboard.reachedGoal = true;
                }
                break;
            }
            case(RightTopFacingLeft):
            {
                if (x == EXPLORATION_STARTING_X + (MIN(ROWS, COLS) - 1) / 2 &&
                    y == EXPLORATION_STARTING_Y + (MAX(ROWS, COLS) - 1) / 2)
                {
                    PRINT("GOAL\n");
                    blackboard.reachedGoal = true;
                }
                break;
            }
            case(RightTopFacingDown):
                if (x == EXPLORATION_STARTING_X - (MAX(ROWS, COLS) - 1) / 2 &&
                    y == EXPLORATION_STARTING_Y + (MIN(ROWS, COLS) - 1) / 2)
                {
                    PRINT("GOAL\n");
                    blackboard.reachedGoal = true;
                }
                break;
            default:
            {
            }
        }
    }

    // Consider any new possible exploration moves
    Direction moveDirectionArray[MAX_POSSIBLE_MOVE_DIRECTIONS];  // Allocate some memory to use the waypoints vector
    Vector<Direction> possibleMoveDirections(moveDirectionArray);

    // pick next move
    switch(lastMove)
    {
        case(NORTH):
        {
            if (!blackboard.wallInFront && explorationMaze.explorationGrid[y-1][x] == GRID_UNEXPLORED) possibleMoveDirections.push_back(NORTH);
            if (!blackboard.wallOnRight && explorationMaze.explorationGrid[y][x+1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(EAST); 
            if (!blackboard.wallOnLeft && explorationMaze.explorationGrid[y][x-1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(WEST); 
            break;
        }
        case(SOUTH):
        {
            if (!blackboard.wallInFront && explorationMaze.explorationGrid[y+1][x] == GRID_UNEXPLORED) possibleMoveDirections.push_back(SOUTH);
            if (!blackboard.wallOnRight && explorationMaze.explorationGrid[y][x-1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(WEST); 
            if (!blackboard.wallOnLeft && explorationMaze.explorationGrid[y][x+1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(EAST); 
            break;
        }
        case(EAST):
        {
            if (!blackboard.wallInFront && explorationMaze.explorationGrid[y][x+1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(EAST);
            if (!blackboard.wallOnRight && explorationMaze.explorationGrid[y+1][x] == GRID_UNEXPLORED) possibleMoveDirections.push_back(SOUTH); 
            if (!blackboard.wallOnLeft && explorationMaze.explorationGrid[y-1][x] == GRID_UNEXPLORED) possibleMoveDirections.push_back(NORTH);             
            break;
        }
        case(WEST):
        {
            if (!blackboard.wallInFront && explorationMaze.explorationGrid[y][x-1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(WEST);
            if (!blackboard.wallOnRight && explorationMaze.explorationGrid[y-1][x] == GRID_UNEXPLORED) possibleMoveDirections.push_back(NORTH);
            if (!blackboard.wallOnLeft && explorationMaze.explorationGrid[y+1][x] == GRID_UNEXPLORED) possibleMoveDirections.push_back(SOUTH);       
            break;
        }
        default:
        {
            break;
        }
    }

    // Choose what move to make
    if (!possibleMoveDirections.empty())
    {
        // If we haven't visited the goal yet, move in a direction towards the centre

        int goalX;
        int goalY;

        // we're exploring a new route!
        switch(blackboard.startingPose)
        {
            case LeftTopFacingRight:
            {
                goalX = 6;
                goalY = 4;
                break;                
            }
            case LeftTopFacingDown:
            {
                goalX = 12;
                goalY = 2;
                break;
            }
            case RightTopFacingLeft:
            {
                goalX = 10;
                goalY = 4;
                break;
            }
            case RightTopFacingDown:
            {
                goalX = 4;
                goalY = 2;
                break;
            }
            case Unknown:
            {
                // minimise goalX;
                if (blackboard.x > 8)
                {
                    goalX = 12;
                }
                else
                {
                    goalX = 4;
                }
                goalY = 4;
                break;
            }
        }

        int min_index = 0;
        int min_val = 9999;

        // PRINT("StartingPose: ");
        // PRINT(blackboard.startingPose);
        // PRINT("\n");

        for (unsigned i = 0; i < possibleMoveDirections.size(); ++i)
        {
            int val = 0;
            int nextX = blackboard.x;
            int nextY = blackboard.y;
            
            switch(possibleMoveDirections[i])
            {
                case NORTH:
                {
                    nextY -= 1;
                    break;
                }
                case SOUTH:
                {
                    nextY += 1;
                    break;
                }
                case EAST:
                {
                    nextX += 1;
                    break;
                }
                case WEST:
                {
                    nextX -= 1;
                    break;
                }
            }

            // PRINT("goalX: ");
            // PRINT(goalX);
            // PRINT("\n");
            // PRINT("goalY: ");
            // PRINT(goalY);
            // PRINT("\n");
            
            if (goalX > 0)
            {
                val += abs(nextX - goalX);
            }
            if (goalY > 0)
            {
                val += abs(nextY - goalY);
            }

            if (val < min_val)
            {
                min_index = i;
                min_val = val;
            }

            // PRINT("Direction: ");
            // PRINT(possibleMoveDirections[i]);
            // PRINT("\n");
            // PRINT("val: ");
            // PRINT(val);
            // PRINT("\n");
        }

        lastMove = possibleMoveDirections[min_index];

        // add how we can get back to the backTrackMoves
        switch(lastMove)
        {
            case(NORTH):
                backTrackMoves.push_back(SOUTH);
                break;
            case(SOUTH):
                backTrackMoves.push_back(NORTH);
                break;
            case(EAST):
                backTrackMoves.push_back(WEST);
                break;
            case(WEST):
                backTrackMoves.push_back(EAST);
                break;
            default:
                break;
        }
    }
    else
    {
        // we can't explore, lets backtrack!
        if (!backTrackMoves.empty())
        {
            // move back to last grid
            lastMove = backTrackMoves.back();
            backTrackMoves.pop_back();
        }
        else
            // we're back at the start, we're done so don't move
            lastMove = NONE;
    }

    if (!blackboard.reachedGoal)
    {

        // in simulation, we move the robot here
        switch(lastMove)
        {
            case(NORTH):
                blackboard.y -= 1;
                break;
            case(SOUTH):
                blackboard.y += 1;

                break;
            case(EAST):
                blackboard.x += 1;
                break;
            case(WEST):
                blackboard.x -= 1;
                break;   
            default:
                break;
        }
        blackboard.facingDirection = lastMove;
    }
}