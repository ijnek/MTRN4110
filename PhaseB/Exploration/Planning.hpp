#ifndef PLANNING_HPP
#define PLANNING_HPP


// For debugging without the Arduino library
// #include <Arduino.h>
#include "Vector.h"
#include "Blackboard.h"

#define NOWALL  0
#define WALL    1
#define UNEXPL  2

// #define NORTH   0
// #define SOUTH   1
// #define EAST    2
// #define WEST    3

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
        Maze(std::string);
        ~Maze();
        void shortestPath(Vector<Position>&);
        void print();
};

// class Planning
// {
//     Blackboard &blackboard;

//   public:
//     Planning(Blackboard &blackboard) : blackboard(blackboard)
//     {
//     }

//     void tick()
//     {
//         Position pathArray[15];
//         Vector<Position> path(pathArray);
//         Maze *maze = new Maze(blackboard.encodedMaze);
//         maze->shortestPath(path);
//     }
// };

#endif