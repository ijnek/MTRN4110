#ifndef PLANNING_H
#define PLANNING_H

#define NOWALL  0
#define WALL    1
#define UNEXPL  2

#define COLS    9   // # of columns
#define ROWS    5   // # of rows
#define GRRW        (ROWS*2)-1  // # of rows for walls
#define GRID_ROW(x) x*2         // Row # for walls

typedef unsigned int edge;

class Graph {
    private:
        edge** walls;   // Array of walls in the map (excluding the outer walls)
        edge** paths;   // Shortest path possible based on the given map
        edge startRow;  // Original position (row)
        edge startCol;  // Original position (column)
        void initWalls(char*);        // Initialise wall
        void initStart(char*);        // Initialise startRow and startCol
        edge* getAdjVal(edge, edge);  // Get the values of adjacent cells
        void floodFill();             // Populate 'paths' using the flood fill algorithm
        String getSymbol(edge, edge); // Transfrom cell position into a printable symbol
    
    public:
        Graph(char*);             // Class constructor
        ~Graph();                 // Class destructor
        Graph(const Graph&);      // Copy constructor
        bool calcShortestPath();  // Get the shortest path from a specific starting point
        void printGraph();        // Print the graph walls and path
        friend bool isExplored(Graph* g); // Has the graph been sufficiently explored?
};

#endif
