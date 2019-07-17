#include "planning.h"

/*
 * Graph constructor
 */
Graph::Graph(){
    // // Allocate enough memory for ROWS and COLS
    int adjustedRows = (ROWS*2)-1;
    nodes = new edge*[adjustedRows];
    for(int i = 0; i < adjustedRows; i++){
        nodes[i] = new edge[COLS];
    }
}

/*
 * Add an edge to the graph
 */
void Graph::addEdge(edge row, edge col, edge val){
    nodes[row][col] = val;
}

/*
 * Creates a map from the input
 * Return an adjacency list
 *  Horizontal walls will have (ROWS-1)*COLS spots
 *  Vertical walls will have ROWS*(COLS-1) spots
 */
char** createMap(char *map){
    Graph* res = new Graph();

    // Split into horizontal and vertical components
    std::string map_str = static_cast<std::string>(map);
    fpos_t pos = map_str.find("V");
    std::string horizontal = map_str.substr(1, pos-1);
    std::string vertical = map_str.substr(pos+1);

    // Initialise the edges of the graph as unexplored
    for(int i = 0; i < (ROWS*2)-1; i++){
        for(int j = 0; j < COLS; j++){
            res->addEdge(i, j, UNEXPL);
        }
    }

    // Fill the graph with the horizontal walls
    for(int i = 0; i < horizontal.size(); i++){
        res->addEdge(((i/COLS)*2+1), i%COLS, horizontal[i]-48);
    }

    // Fill the graph with the vertical walls
    for(int i = 0; i < vertical.size(); i++){
        res->addEdge((i/COLS)*2, i%COLS, vertical[i]-48);
    }

    return NULL;
}


std::ostream& operator<<(std::ostream& output, const Graph& g){
    for(int i = 0; i < (ROWS*2)-1; i++){
        for(int j=0; j < COLS; j++)
            output << g.nodes[i][j];
        output << std::endl;
    }
    return output;
}

/* 
 * Find the shortest path from the start to the center of the maze
 * A path is the shortest when:
 *  - it is a valid path
 *  - it is the shortest path
 *  - it requires the least number of turns
 *  - the map has been sufficiently explored to find the shortest path
 */

char* shortestPath(){
    /*

    initialize
        all CellValues <- N (N=NumberOfCells-1, e.g. N=44)
        GoalCellValue <- 0
        CurrentExploredValue <- 0
        MazeValueChanged <- 1
    while MazeValueChanged ≠ 0
        MazeValueChanged <- 0
        forall Rows
            forall Columns
                if CurrentCellValue==CurrentExploredValue
                    forall Directions (North, East, South, West)
                        if NeighbouringWall does not exist
                            if NeighbouringCellValue==N
                                NeighbouringCellValue <- CurrentCellValue+1
                                MazeValueChanged <- 1
        CurrentExploredValue = CurrentExploredValue +1

     */

    // shortest path is found by flood fill
    return nullptr;
}

std::ostream& operator<<(std::ostream& output, const Maze& maze){
    output << maze.map;
    return output;
}