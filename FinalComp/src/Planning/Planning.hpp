#ifndef PLANNING_HPP
#define PLANNING_HPP


// For debugging without the Arduino library
#include <Arduino.h>
#include "../Utils/Vector/Vector.h"
#include "../Types/Blackboard.h"

#define NOWALL  0
#define WALL    1
#define UNEXPL  2

#define NORTH   0
#define SOUTH   1
#define EAST    2
#define WEST    3

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

    void tick()
    {
        Position pathArray[15];
        Vector<Position> path(pathArray);
        Maze *maze = new Maze(blackboard.encodedMaze);
        maze->shortestPath(path);

        blackboard.plannedWayPoints.clear();

        for (unsigned i = 1; i < path.size(); ++i)  // start from first, because we're already at the start
        {
            blackboard.plannedWayPoints.push_back(WayPoint(path[i].row * 250, path[i].col * 250, 0));
        }
    }
};

#endif