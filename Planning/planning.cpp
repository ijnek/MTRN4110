#include "planning.h"
#include <iostream>
#include <array>
#include <string>

/*
 * Graph constructor
 * Params:
 *      map - the maze layout and the starting location and heading of the robot
 */
Graph::Graph(char *map) {
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
    shortestPath = new edge[2*ROWS*COLS];
    for(int i = 0; i < 2*ROWS*COLS; i++) shortestPath[i] = FWD;
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
 * Basically counts the number of turns for each possible path
 * Return: turns for the shortest path, NULL if no valid path exists
 */
edge* Graph::getShortestPath(edge startRow, edge startCol){
    floodFill(startRow, startCol); // populate the paths structure
    int N = ROWS*COLS - 1;
    edge currVal = paths[ROWS/2][COLS/2]; // Value at the center of the map (the goal)
    if(currVal == N) return nullptr; // No path can reach the center

    edge minPath[ROWS][COLS]; // Minimum turn path
    edge curPath[ROWS][COLS]; // Current path
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++){
            curPath[i][j] = N;
            minPath[i][j] = N;
        }
            
    
    int pathsFlag = 1; // Flag to say all paths have been searched
    int minTurns = N*N; // Minimum amount of turns for the minimum path
    int curTurns = 0; // Current turn count
    int shortCount = 0;
    edge prevCell[2] = {ROWS/2, COLS/2}; // previous cell
    edge curCell[2] = {ROWS/2, COLS/2}; // current cell
    edge visited[ROWS*COLS] = {0};

    while(pathsFlag){
        for(int i = 0; i < ROWS; i++)
            for(int j = 0; j < COLS; j++)
                curPath[i][j] = N;
        curTurns = 0;
        currVal = paths[ROWS/2][COLS/2];
        curCell[0] = ROWS/2; // Reset the current cell to the center of the map
        curCell[1] = COLS/2;
        int valid = 0;
        while(currVal){ // search a path
            int count = 0;
            std::array<edge, 2> neigCell = {0, 0};
            bool *bounds = isWall(curCell[0], curCell[1]); // check the boundaries (walls) for this cell
            // Find the location of the next cell
            for(int k = 0; k < 4; k++){
                edge nextRow, nextCol; // next row and column
                if(bounds[k]){
                    switch(k){
                        case 0: // North Direction
                            nextRow = curCell[0] - 1;
                            nextCol = curCell[1];
                            break;
                        case 1: // South Direction
                            nextRow = curCell[0] + 1;
                            nextCol = curCell[1];
                            break;
                        case 2: // East Direction
                            nextRow = curCell[0];
                            nextCol = curCell[1] + 1;
                            break;
                        case 3: // West Direction
                            nextRow = curCell[0];
                            nextCol = curCell[1] - 1;
                            break;
                    }
                    if(paths[nextRow][nextCol] == currVal-1){ // Potential next cell found, add it to array
                        // store the location of the neighbour
                        neigCell[0] = nextRow;
                        neigCell[1] = nextCol;
                        count++;
                        int i;
                        for(i = 0; visited[i] != 0; i++);
                        visited[i] = currVal;
                    }
                }
            }
            if(count == 0) break; // No suitable neighbour cell was found --> no path exists

            // Update the cell values
            prevCell[0] = curCell[0];
            prevCell[1] = curCell[1];
            curCell[0] = neigCell[0];
            curCell[1] = neigCell[1];

            // Add to currPath and shortestPath
            curPath[neigCell[0]][neigCell[1]] = paths[neigCell[0]][neigCell[1]];
            shortestPath[shortCount] = neigCell[0];
            shortestPath[shortCount+1] = neigCell[0];
            shortCount += 2;

            if(count > 1) paths[neigCell[0]][neigCell[1]] += N; // 'Mark' the cell
            if(count == 1){
                for(int i = 0; i < ROWS; i++){
                    if(visited[i] == paths[neigCell[0]][neigCell[1]]){
                        paths[neigCell[0]][neigCell[1]] += N;
                        break;
                    }
                }
            }
            currVal--;
            pathsFlag = 1;
        }

        // calculate the number of turns in this path if the path is valid
        if(pathsFlag) {
            for(int i = 2*2; shortestPath[i] != FWD; i += 2){
                // Get every second cell
                edge pRow = shortestPath[i-4];
                edge pCol = shortestPath[i-3];
                edge cRow = shortestPath[i];
                edge cCol = shortestPath[i+1];

                edge difRow = cRow - pRow;
                edge difCol = cCol - pCol;
                if(difRow == 0 && difCol == 0) curTurns++;
            }
        }
        if(curTurns == 0) break;
        shortestPath = new edge[2*ROWS*COLS];
        for(int i = 0; i < 2*ROWS*COLS; i++) shortestPath[i] = FWD;

        std::cout << "========== CURRENT PATH ==========" << std::endl;
        for(int i = 0; i < ROWS; i++) {
            std::cout << "    ";
            for(int j = 0; j < COLS; j++) {
                if(curPath[i][j] == 44) std::cout << "* ";
                else std::cout << curPath[i][j] << " ";
            }
            std::cout<<std::endl;
        }
        std::cout << "Number of turns: " << curTurns << std::endl;
        std::cout << "==================================" << std::endl;

        if(curTurns < minTurns && curTurns != 0) { // Update the minimum turn path if necessary
            minTurns = curTurns;
            for(int i = 0; i < ROWS; i++)
                for(int j = 0; j < COLS; j++)
                    minPath[i][j] = curPath[i][j];
        }
    }
    
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++)
            paths[i][j] = minPath[i][j];

    // while flag
        // flag = 0
        // while currVal is 0
            // current node = center of the map
            // for all adjacent neighbours
                // if neighbour's value is == currVal - 1
                    // store neighbour location in X
            // if len(X) == 0
                // break
            // if len(X) > 1
                // X[0] value += 50
            // prev node = current node
            // current node == X[0] location
            // add current node to current path
            // flag = 1
            // clear X
            // did turn(current node, prev node) ? current turns ++
        // if current turns < minturns
            // min turns = current turns
            // update min path

    // paths = min path

    return shortestPath;
}

/*
 * Helper function for the << operator
 */
static std::string getSymbol(edge row, edge col, edge wall, edge path){
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
        if(path == ROWS*COLS - 1){
            switch(wall){
                case WALL:
                    return "   |";
                case NOWALL:
                    return "    ";
                case UNEXPL:
                    return "   *";
                default:
                    return "   *";
            }
        }
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
std::ostream& operator<<(std::ostream& output, const Graph& g){
    output << "  --- --- --- --- --- --- --- --- --- " << std::endl;
    for(int i = 0; i < GRRW; i++){
        if(i%2 == 0) output << "| ";
        else output << "  ";
        for(int j=0; j < COLS; j++){
            if(i == (ROWS/2 + 2) && j == COLS/2){ // Center of the maze
                output << " X  "; //TODO - include the wall in this
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