#include <iostream>
#include <string>

#ifndef PLANNING_H
#define PLANNING_H

// Symbols for the map
#define WALL    0
#define NOWALL  1
#define UNEXPL  2

#define COLS    9
#define ROWS    5


typedef unsigned int edge;

class Graph {
    private:
        edge **nodes;
    
    public:
        Graph();
        void addEdge(edge row, edge col, edge val);
        friend std::ostream& operator<<(std::ostream& output, const Graph& g);
};

class Maze {
    private:
        const char** map;  // Using an adjacency list to represent the map
        int length; // Length of the map
        // variable for storing the path?
    public:
        Maze(const char* map);
        char* shortestPath(); // Get the shortest path?
        friend std::ostream& operator<<(std::ostream& output, const Maze& maze);
};
char** createMap(char *map);

#endif