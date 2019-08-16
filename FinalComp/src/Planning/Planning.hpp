#ifndef PLANNING_HPP
#define PLANNING_HPP


// For debugging without the Arduino library
#include <Arduino.h>
#include "../Utils/Vector/Vector.h"
#include "../Types/Blackboard.h"
#include "../Exploration/ExplorationEnums.h"

#define NOWALL  0
#define WALL    1
#define UNEXPL  2

#define N       5*9-1

typedef struct position {
    int row;
    int col;
} Position;


class Maze {
    private:
        Position* orientation;
        Position* origin;
        int heading;
        int* hWalls;
        int hNum;
        int* vWalls;
        int vNum;
        int** cells;
    private:
        int* getAdjCells(Position p);
        void floodFill();
        void getPath(int, int, Vector<Position>&);
    
    public:
        Maze(String);
        ~Maze();
        void shortestPath(Vector<Position>&);
        void print();
};

class Planning
{
    Blackboard &blackboard;

  public:
    Planning(Blackboard &blackboard) : blackboard(blackboard)
    {
    }

    void reset(){}  // planning is well-written, so nothing to do in reset

    void tick()
    {
        Position pathArray[30];
        Vector<Position> path(pathArray);

        // Serial.println("here");

        // String str = "S001H111111111022222222022202222022202222000002222111111111V10222222211022222221102200222110220022211000002221";
        // Maze *maze = new Maze(str);
        
        Maze *maze = new Maze(blackboard.encodedMaze);
        maze->shortestPath(path);

        // Serial.print("encodedMaze: ");
        // Serial.println(blackboard.encodedMaze);

        // Serial.println("Path: ");

        // for (unsigned i = 0; i < path.size(); ++i)
        // {
        //     Serial.print(path[i].row);
        //     Serial.print(", ");
        //     Serial.println(path[i].col);
        // }

        blackboard.plannedWayPoints.clear();

        Direction prevTravelDirection = NORTH;

        for (unsigned i = 1; i < path.size(); ++i)  // start from first, because we're already at the start
        {
            Direction travelDirection;
            if (path[i].row > path[i-1].row){
                travelDirection = NORTH;
            } else if (path[i].row < path[i-1].row){
                travelDirection = SOUTH;
            } else if (path[i].col > path[i-1].col){
                travelDirection = WEST;
            } else {
                travelDirection = EAST;
            }

            // If travel direction changed, then add last waypoint as a waypoint
            if (travelDirection != prevTravelDirection)
            {
                blackboard.plannedWayPoints.push_back(WayPoint(path[i-1].row * 250, path[i-1].col * 250, 0));
            }

            prevTravelDirection = travelDirection;
        }

        blackboard.plannedWayPoints.push_back(WayPoint(path[path.size()-1].row * 250, path[path.size()-1].col * 250, 0));


        // for (unsigned i = 0; i < blackboard.plannedWayPoints.size(); ++i)
        // {
        //     Serial.print(blackboard.plannedWayPoints[i].x);
        //     Serial.print(", ");
        //     Serial.println(blackboard.plannedWayPoints[i].y);
        // }


        blackboard.plan = BEHAVIOUR_PLANNED_ROUTE;

        delete maze;
    }
};

#endif