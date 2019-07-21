#include <iostream>
#include <string>

#ifndef PLANNING_H
#define PLANNING_H

// For nodes
#define NOWALL  0
#define WALL    1
#define UNEXPL  2

// For shortestPath
#define FWD     3
#define BCK     4
#define LFT     5
#define RGT     6

#define COLS    9
#define ROWS    5
#define GRRW        (ROWS*2)-1  // # of rows for walls
#define GRID_ROW(x) x*2         // Row # for walls

namespace Maze {
    typedef unsigned int edge;

    class Graph {
        private:
            edge** walls;
            edge** paths;
            edge* shortestPath;
            edge startRow;
            edge startCol;
            void addEdge(edge, edge, edge); // Add an edge to the graph
            bool* isWall(edge, edge);
            void floodFill(edge, edge); 
        
        public:
            Graph(char*);
            edge* getShortestPath(edge, edge); // Get the shortest path from a specific starting point
            friend std::ostream& operator<<(std::ostream&, const Graph&);
    };
}
#endif