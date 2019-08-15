#ifndef PLANNING_HPP
#define PLANNING_HPP

#define __DEBUG__

// For debugging without the Arduino library
#ifdef __DEBUG__
    #include "Vector/Vector.h"
    #include <string>
    #include <iostream>
    #define String      std::string
    #define indexOf     find
    #define substring   substr
#else
    #include <Arduino.h>
    #include "Vector/Vector.h"
#endif

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

#endif
