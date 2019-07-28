#include "Exploration.h"
#include "Vector.h"
#include "ExplorationMaze.h"
#include <iostream>

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


    // Consider any new possible exploration moves
    Direction moveDirectionArray[MAX_POSSIBLE_MOVE_DIRECTIONS];  // Allocate some memory to use the waypoints vector
    Vector<Direction> possibleMoveDirections(moveDirectionArray);

    // pick next move
    switch(lastMove)
    {
        // case(NORTH):
        // {
        //     if (!blackboard.wallInFront && explorationMaze.explorationGrid[x][y-1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(NORTH);
        //     if (!blackboard.wallOnRight && explorationMaze.explorationGrid[x+1][y] == GRID_UNEXPLORED) possibleMoveDirections.push_back(EAST); 
        //     if (!blackboard.wallOnLeft && explorationMaze.explorationGrid[x-1][y] == GRID_UNEXPLORED) possibleMoveDirections.push_back(WEST); 
        //     break;
        // }
        // case(SOUTH):
        // {
        //     if (!blackboard.wallInFront && explorationMaze.explorationGrid[x][y+1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(SOUTH);
        //     if (!blackboard.wallOnRight && explorationMaze.explorationGrid[x-1][y] == GRID_UNEXPLORED) possibleMoveDirections.push_back(WEST); 
        //     if (!blackboard.wallOnLeft && explorationMaze.explorationGrid[x+1][y] == GRID_UNEXPLORED) possibleMoveDirections.push_back(EAST); 
        //     break;
        // }
        // case(EAST):
        // {
        //     if (!blackboard.wallInFront && explorationMaze.explorationGrid[x+1][y] == GRID_UNEXPLORED) possibleMoveDirections.push_back(EAST);
        //     if (!blackboard.wallOnRight && explorationMaze.explorationGrid[x][y+1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(SOUTH); 
        //     if (!blackboard.wallOnLeft && explorationMaze.explorationGrid[x][y-1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(NORTH);             
        //     break;
        // }
        // case(WEST):
        // {
        //     if (!blackboard.wallInFront && explorationMaze.explorationGrid[x-1][y] == GRID_UNEXPLORED) possibleMoveDirections.push_back(WEST);
        //     if (!blackboard.wallOnRight && explorationMaze.explorationGrid[x][y-1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(NORTH);
        //     if (!blackboard.wallOnLeft && explorationMaze.explorationGrid[x][y+1] == GRID_UNEXPLORED) possibleMoveDirections.push_back(SOUTH);       
        //     break;
        // }
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

    for (unsigned i = 0; i < possibleMoveDirections.size(); ++i)
    {
        PRINT(possibleMoveDirections[i]);
        PRINT(", ");
    }
    PRINT("\n");

    // Choose what move to make
    if (!possibleMoveDirections.empty())
    {
        // we're exploring a new route!
        lastMove = possibleMoveDirections[0];

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

    std::cout << lastMove << std::endl;
}