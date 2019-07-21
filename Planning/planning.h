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
#define GRRW        (ROWS*2)-1        // Grid row
#define GRID_ROW(x) x*2

namespace Maze {
    typedef unsigned int edge;

    class Graph {
        private:
            edge** walls;
            edge** paths;
            edge* shortestPath;
        
        public:
            Graph(char*);
            void addEdge(edge, edge, edge); // Add an edge to the graph
            bool* isWall(edge, edge);
            edge* getShortestPath(); // Get the shortest path
            friend std::ostream& operator<<(std::ostream&, const Graph&);
    };
}
#endif