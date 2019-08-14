#include "Exploration.h"
#include "../Utils/Vector/Vector.h"
#include "ExplorationMaze.h"
#include "../Utils/MathUtil/MathUtil.h"
#include "../Constants/MazeConstants.h"
#include "../Constants/VehicleConstants.h"

#define MAX_POSSIBLE_MOVE_DIRECTIONS 4


Direction lastMove = SOUTH;
bool wallOnLeft = false;
bool wallInFront = false;
bool wallOnRight = false;

void Exploration::tick()
{
    // If we've just finished a command, then read lidars
    if (blackboard.startDetected && blackboard.commandCompleted)
    // if (blackboard.startDetected && blackboard.commandCompleted && blackboard.nextMoveCommandReceived)
    {    

        blackboard.nextMoveCommandReceived = false; // reset this flag

        int x = mapX;
        int y = mapY;

        // update our map
        explorationMaze.explorationGrid[y][x] = GRID_EXPLORED;
        switch(facingDirection)
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

        // print the updated maze (for debugging)
        // explorationMaze.printMaze(mapX, mapY, facingDirection);
        // Serial.print("\n\n");

        // check if we've figured out our starting pose, depending on the updates
        if (startingPose == Unknown)
        {
            if (x >= EXPLORATION_STARTING_X + (MIN(ROWS, COLS) - 1))
            {
                if (explorationMaze.verticalWall[y][x+1] == WALL_NO_EXIST)
                {
                    startingPose = LeftTopFacingDown;
                    // fill the outer walls of the maze
                    explorationMaze.fillHorizontalWall(0);
                    explorationMaze.fillHorizontalWall(COLS);
                    explorationMaze.fillVerticalWall(EXPLORATION_STARTING_X);
                    explorationMaze.fillVerticalWall(EXPLORATION_STARTING_X + ROWS);                  
                }
            }
            if (x <= EXPLORATION_STARTING_X - (MIN(ROWS, COLS) - 1))
            {
                if (explorationMaze.verticalWall[y][x] == WALL_NO_EXIST)
                {
                    startingPose = RightTopFacingDown;
                    // fill the outer walls of the maze
                    explorationMaze.fillHorizontalWall(0);
                    explorationMaze.fillHorizontalWall(COLS);
                    explorationMaze.fillVerticalWall(EXPLORATION_STARTING_X + 1 - ROWS);
                    explorationMaze.fillVerticalWall(EXPLORATION_STARTING_X + 1);                    
                }
            }
            if (y >= EXPLORATION_STARTING_Y + (MIN(ROWS, COLS) - 1) && x > EXPLORATION_STARTING_X)
            {
                if (explorationMaze.horizontalWall[y+1][x] == WALL_NO_EXIST)
                {
                    startingPose = RightTopFacingLeft;
                    // fill the outer walls of the maze
                    explorationMaze.fillHorizontalWall(0);
                    explorationMaze.fillHorizontalWall(ROWS);
                    explorationMaze.fillVerticalWall(EXPLORATION_STARTING_X);
                    explorationMaze.fillVerticalWall(EXPLORATION_STARTING_X + COLS);
                }
            }
            if (y >= EXPLORATION_STARTING_Y + (MIN(ROWS, COLS) - 1) && x < EXPLORATION_STARTING_X)
            {
                if (explorationMaze.horizontalWall[y+1][x] == WALL_NO_EXIST)
                {
                    startingPose = LeftTopFacingRight;
                    // fill the outer walls of the maze
                    explorationMaze.fillHorizontalWall(0);
                    explorationMaze.fillHorizontalWall(ROWS);
                    explorationMaze.fillVerticalWall(EXPLORATION_STARTING_X - COLS + 1);
                    explorationMaze.fillVerticalWall(EXPLORATION_STARTING_X + 1);
                }
            }
        }

        if (!blackboard.reachedGoal)
        {
            // If we know our starting pose, then we know if we're at the goal
            switch (startingPose)
            {
                case(LeftTopFacingDown):
                {
                    if (x == EXPLORATION_STARTING_X + (MAX(ROWS, COLS) - 1) / 2 &&
                        y == EXPLORATION_STARTING_Y + (MIN(ROWS, COLS) - 1) / 2)
                    {
                        blackboard.reachedGoal = true;
                        // explorationMaze.printMaze(EXPLORATION_STARTING_X, EXPLORATION_STARTING_Y, EXPLORATION_STARTING_FACING_DIRECTION,
                        //                          EXPLORATION_STARTING_Y, EXPLORATION_STARTING_X,
                        //                          EXPLORATION_STARTING_Y + (COLS - 1), EXPLORATION_STARTING_X + (ROWS - 1));
                        String hAndVWall = explorationMaze.encodeHAndVWall(
                                                 EXPLORATION_STARTING_X, EXPLORATION_STARTING_Y, EXPLORATION_STARTING_FACING_DIRECTION,
                                                 EXPLORATION_STARTING_Y, EXPLORATION_STARTING_X,
                                                 EXPLORATION_STARTING_Y + (COLS - 1), EXPLORATION_STARTING_X + (ROWS - 1));
                        String position = "S001";
                        blackboard.encodedMaze = position + hAndVWall;
                    }
                    break;
                }
                case(LeftTopFacingRight):
                {
                    if (x == EXPLORATION_STARTING_X - (MIN(ROWS, COLS) - 1) / 2 &&
                        y == EXPLORATION_STARTING_Y + (MAX(ROWS, COLS) - 1) / 2)
                    {
                        blackboard.reachedGoal = true;
                        // explorationMaze.printMaze(EXPLORATION_STARTING_X, EXPLORATION_STARTING_Y, EXPLORATION_STARTING_FACING_DIRECTION,
                        //                           EXPLORATION_STARTING_Y, EXPLORATION_STARTING_X - (COLS - 1),
                        //                           EXPLORATION_STARTING_Y + (ROWS - 1), EXPLORATION_STARTING_X);
                        String hAndVWall = explorationMaze.encodeHAndVWall(
                                                  EXPLORATION_STARTING_X, EXPLORATION_STARTING_Y, EXPLORATION_STARTING_FACING_DIRECTION,
                                                  EXPLORATION_STARTING_Y, EXPLORATION_STARTING_X - (COLS - 1),
                                                  EXPLORATION_STARTING_Y + (ROWS - 1), EXPLORATION_STARTING_X);
                        String position = "S041";
                        blackboard.encodedMaze = position + hAndVWall;
                    }
                    break;
                }
                case(RightTopFacingLeft):
                {
                    if (x == EXPLORATION_STARTING_X + (MIN(ROWS, COLS) - 1) / 2 &&
                        y == EXPLORATION_STARTING_Y + (MAX(ROWS, COLS) - 1) / 2)
                    {
                        blackboard.reachedGoal = true;
                        // explorationMaze.printMaze(EXPLORATION_STARTING_X, EXPLORATION_STARTING_Y, EXPLORATION_STARTING_FACING_DIRECTION,
                        //                           EXPLORATION_STARTING_Y, EXPLORATION_STARTING_X,
                        //                           EXPLORATION_STARTING_Y + (ROWS - 1), EXPLORATION_STARTING_X + (COLS - 1));
                        String hAndVWall = explorationMaze.encodeHAndVWall(EXPLORATION_STARTING_X, EXPLORATION_STARTING_Y, EXPLORATION_STARTING_FACING_DIRECTION,
                                                  EXPLORATION_STARTING_Y, EXPLORATION_STARTING_X,
                                                  EXPLORATION_STARTING_Y + (ROWS - 1), EXPLORATION_STARTING_X + (COLS - 1));
                        String position = "S001";
                        blackboard.encodedMaze = position + hAndVWall;
                    }
                    break;
                }
                case(RightTopFacingDown):
                {
                    if (x == EXPLORATION_STARTING_X - (MAX(ROWS, COLS) - 1) / 2 &&
                        y == EXPLORATION_STARTING_Y + (MIN(ROWS, COLS) - 1) / 2)
                    {
                        blackboard.reachedGoal = true;
                        // explorationMaze.printMaze(EXPLORATION_STARTING_X, EXPLORATION_STARTING_Y, EXPLORATION_STARTING_FACING_DIRECTION,
                        //                           EXPLORATION_STARTING_Y, EXPLORATION_STARTING_X - ROWS + 1,
                        //                           EXPLORATION_STARTING_Y + COLS - 1, EXPLORATION_STARTING_X);
                        String hAndVWall = explorationMaze.encodeHAndVWall(EXPLORATION_STARTING_X, EXPLORATION_STARTING_Y, EXPLORATION_STARTING_FACING_DIRECTION,
                                                  EXPLORATION_STARTING_Y, EXPLORATION_STARTING_X - ROWS + 1,
                                                  EXPLORATION_STARTING_Y + COLS - 1, EXPLORATION_STARTING_X);
                        String position = "S081";
                        blackboard.encodedMaze = position + hAndVWall;
                    }
                    break;
                }
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

        // in simulation, we move the robot here
        switch(lastMove)
        {
            case(NORTH):
                mapY -= 1;
                break;
            case(SOUTH):
                mapY += 1;
                break;
            case(EAST):
                mapX += 1;
                break;
            case(WEST):
                mapX -= 1;
                break;   
            default:
                break;
        }
        facingDirection = lastMove;

        // Finally, write it to blackboard!
        blackboard.plan = BEHAVIOUR_NEXT_WAYPOINT;
        float nextWayPointX = CELL_SIZE * (mapY - EXPLORATION_STARTING_Y);
        float nextWayPointY = CELL_SIZE * (mapX - EXPLORATION_STARTING_X);

        // offset so robot doesn't go too forward
        if (mapX > prevMapX)
        {
            nextWayPointX -= WHEELS_OFFSET_X_MM;
        }
        else if (mapX < prevMapX)
        {
            nextWayPointX += WHEELS_OFFSET_X_MM;
        }
        else if (mapY > prevMapY)
        {
            nextWayPointY -= WHEELS_OFFSET_X_MM;
        }
        else if (mapY < prevMapY)
        {
            nextWayPointY += WHEELS_OFFSET_X_MM;
        }

        // String nextWayPoint = "nextWayPoint: " + String(nextWayPointX) + ", " + String(nextWayPointY);
        // Serial1.println(nextWayPoint);

        prevMapX = mapX;
        prevMapY = mapY;

        float nextWayPointH = 0;  // initialise just in case
        switch(facingDirection)
        {
            case(NORTH): nextWayPointH = DEG2RAD(180); break;
            case(SOUTH): nextWayPointH = DEG2RAD(0); break;
            case(EAST): nextWayPointH = DEG2RAD(90); break;
            case(WEST): nextWayPointH = DEG2RAD(-90); break;
        }
        blackboard.nextWayPoint = WayPoint(nextWayPointX, nextWayPointY, nextWayPointH);
        // Serial.println(EXPLORATION_STARTING_X);
        // Serial.print(mapX);
        // Serial.print(", ");
        // Serial.print(mapY);
        // Serial.print(", ");
        // Serial.println(facingDirection);
        // Serial.println("nextWayPoint:");
        // Serial.print(nextWayPointX);
        // Serial.print(", ");
        // Serial.print(nextWayPointY);
        // Serial.print(", ");
        // Serial.println(nextWayPointH);
    }
}