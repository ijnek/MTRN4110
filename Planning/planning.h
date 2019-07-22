#ifndef PLANNING_H
#define PLANNING_H

// For nodes
#define NOWALL  0
#define WALL    1
#define UNEXPL  2

#define COLS    9
#define ROWS    5
#define GRRW        (ROWS*2)-1  // # of rows for walls
#define GRID_ROW(x) x*2         // Row # for walls

typedef unsigned int edge;

class Graph {
    private:
        edge** walls; // Array of walls in the map (excluding the outer walls)
        edge** paths; // Shortest path possible based on the given map
        edge startRow; // Original position (row)
        edge startCol; // Original position (column)
        void addEdge(edge, edge, edge); // Add an edge to the graph
        bool* isWall(edge, edge);
        void floodFill(edge, edge); 
    
    public:
        Graph(char*);
        void calcShortestPath(edge, edge); // Get the shortest path from a specific starting point
        void printGraph();        
};

#endif
