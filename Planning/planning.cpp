#include "planning.h"
using namespace Maze;

/*
 * Graph constructor
 * Creates a map from the input
 * Return an adjacency list
 *     Horizontal walls will have (ROWS-1)*COLS spots
 *     Vertical walls will have ROWS*(COLS-1) spots
 */
Graph::Graph(char *map) : startRow(0), startCol(0) {
    // Allocate memory for walls
    walls = new edge*[GRRW];
    for(int i = 0; i < GRRW; i++) walls[i] = new edge[COLS];

    // Initialise walls as unexplored
    for(int i = 0; i < (ROWS*2)-1; i++)
        for(int j = 0; j < COLS; j++)
            addEdge(i, j, UNEXPL);

    // Split map into horizontal and vertical components
    std::string map_str = static_cast<std::string>(map);
    fpos_t pos = map_str.find("V");
    std::string horizontal = map_str.substr(1, pos-1);
    std::string vertical = map_str.substr(pos+1);

    // Pad the vertical wall ends with UNEXPL
    for(int i = 1; COLS*i < vertical.size(); i++) vertical.insert((COLS*i)-1, std::to_string(UNEXPL));

    // Fill walls with the map's vertical and horizontal walls
    for(int i = 0; i < vertical.size(); i++) addEdge((i/COLS)*2, i%COLS, vertical[i]-48);
    for(int i = 0; i < horizontal.size(); i++) addEdge(((i/COLS)*2+1), i%COLS, horizontal[i]-48);

    // Allocate memory for paths
    paths = new edge*[ROWS];
    for(int i = 0; i < COLS; i++) paths[i] = new edge[COLS];

    // Allocate memory for Initialise shortest path to the maximum possible path
    shortestPath = new edge[ROWS*COLS];
    for(int i = 0; i < ROWS*COLS; i++) shortestPath[i] = FWD;
}

/*
 * Add an edge to the graph
 */
void Graph::addEdge(edge row, edge col, edge val){
    walls[row][col] = val;
}

/*
 * Helper function for isWall
 * Checks for immediate out of bounds
 */
static bool isInBounds(edge row, edge col){
    if(col < 0 || col > COLS) return false; // Column is out of bounds
    if(row < 0 || row > GRRW) return false; // Row is out of bounds
    return true;
}

/*
 * Checks if a wall exists for a specific cell value
 * Returns an array representing the cell walls
 */
bool* Graph::isWall(edge row, edge col){
    bool* res = new bool[4];
    edge gridRow, gridCol;

    for(int i = 0; i < 4; i++){
        switch(i){
            case 0: // North Wall
                gridRow = GRID_ROW(row)-1;
                gridCol = col;
                break;
            case 1: // South Wall
                gridRow = GRID_ROW(row) + 1;
                gridCol = col;
                break;
            case 2: // East Wall
                gridRow = GRID_ROW(row);
                gridCol = col;
                break;
            case 3: // West Wall
                gridRow = GRID_ROW(row);
                gridCol = col - 1;
                break;
        }
        if(isInBounds(gridRow, gridCol)){ // Check if a wall exists and is within bounds
            if(gridRow == GRRW) gridRow--; // Edge case
            edge wallVal = walls[gridRow][gridCol];
            res[i] = wallVal & (WALL|UNEXPL) ? false : true;
        } else {
            res[i] = false;
        }
    }
    return res;
}

/*
 * Fills the path structure using the Flood Fill algorithm
 */
void Graph::floodFill(edge startRow, edge startCol){
    // Initialise each cell in paths to be the maximum value
    this->startRow = startRow;
    this->startCol = startCol;
    int N = ROWS*COLS - 1;
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            paths[i][j] = N;
            if(i == startRow && j == startCol) paths[i][j] = 0;
        }
    }
    edge goal = 0;          // Value we want to reach
    edge currentVal = 0;    // value we currently have
    int mazeVal = 1;        // flag for checking if paths has updated

    while(mazeVal){
        mazeVal = 0;
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLS; j++){
                if(paths[i][j] == currentVal){ // if CurrentCellValue==CurrentExploredValue
                    // Check if a wall exists for each direction
                    bool *bounds = isWall(i, j); // check the boundaries (walls) for this cell
                    for(int k = 0; k < 4; k++){
                        edge currRow, currCol;
                        if(bounds[k]){
                            switch(k){
                                case 0: // North Direction
                                    currRow = i - 1;
                                    currCol = j;
                                    break;
                                case 1: // South Direction
                                    currRow = i + 1;
                                    currCol = j;
                                    break;
                                case 2: // East Direction
                                    currRow = i;
                                    currCol = j + 1;
                                    break;
                                case 3: // West Direction
                                    currRow = i;
                                    currCol = j - 1;
                                    break;
                            }
                            if(paths[currRow][currCol] == N){ // Update the value accordingly
                                paths[currRow][currCol] = currentVal+1;
                                mazeVal = 1;
                            }
                        }
                    }
                } 
            }
        }
        currentVal++;
    }
}

/* 
 * Find the shortest path from the start to the center of the maze
 * A path is the shortest when:
 *  - it is a valid path
 *  - it is the shortest path
 *  - it requires the least number of turns
 *  - the map has been sufficiently explored to find the shortest path
 * Implementationi: Flood-Fill algorithm
 * Return: turns for the shortest path, NULL if no valid path exists
 */
edge* Graph::getShortestPath(edge startRow, edge startCol){
    floodFill(startRow, startCol); // populate the paths structure
    int N = ROWS*COLS - 1;
    int pathVal = 1; // Flag for checking if shortestPath has been updated

    // Iterate through paths to find the shortest path
    edge currVal = paths[ROWS/2][COLS/2];
    if(currVal == N){ // No valid paths exist
        return nullptr;
    }

    while(pathVal) {
        // for all neighbours
            //if( == currVal - 1) // Search for currentVal-1 in paths
            

            // mark the common value with some identifier
            // 
        pathVal = 0;
    }
   
                // if there's more than one value, 
                    // mark the common value with some identifier
                        // Shortest path exists for any path that gets to the start
                        // follow that path and take note of the number of turns
                    // stop when the starting value has been reached
                // loop this until all paths have been searched

    return shortestPath;
}

/*
 * Helper function for the << operator
 */
static std::string getSymbol(edge row, edge col, edge wall, edge path){
    if(row == (ROWS/2 + 2) && col == COLS/2) return " X  "; // Center of the maze

    if(row%2){ // Horizontal walls
        switch(wall){
            case WALL:
                return "--- ";
            case NOWALL:
                return "    ";
            case UNEXPL:
                return "*** ";
            default:
                return "*** ";
        }
    } else { // Vertical walls
        if(col == COLS-1) return "   ";
        if(path == ROWS*COLS - 1) return "   *";
        switch(wall){
            case WALL:
                return (path/10) ? std::to_string(path) + " |" : " " + std::to_string(path) + " |";
            case NOWALL:
                return (path/10) ? std::to_string(path) + "  " : " " + std::to_string(path) + "  ";
            case UNEXPL:
                return (path/10) ? std::to_string(path) + " *" : " " + std::to_string(path) + " *";
            default:
                return "   *";
        }
    }
}

/*
 * Output the graph in the proper format
 */
std::ostream& Maze::operator<<(std::ostream& output, const Graph& g){
    output << "  --- --- --- --- --- --- --- --- --- " << std::endl;
    for(int i = 0; i < GRRW; i++){
        if(i%2 == 0) output << "| ";
        else output << "  ";
        for(int j=0; j < COLS; j++){
            if(i == (ROWS/2 + 2) && j == COLS/2){ // Center of the maze
                output << " X  ";
                continue;
            } if(i == g.startRow && j == g.startCol) {
                output << " S  ";
                continue;
            }
            output << getSymbol(i, j, g.walls[i][j], g.paths[i/2][j]);
        }
        if(i%2 == 0) output << " |";
        output << std::endl;
    }
    output << "  --- --- --- --- --- --- --- --- --- ";
    return output;
}