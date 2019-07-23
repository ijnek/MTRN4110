#include <Arduino.h>
#include "planning.h"

/*
 * Graph constructor
 * params:
 *    map - the maze layout and the starting location and heading of the robot
 * assumptions:
 *    'map' is defined
 * return: none
 */
Graph::Graph(char *map) {
  // Allocate memory for walls
  walls = new edge*[GRRW];
  for(int i = 0; i < GRRW; i++) walls[i] = new edge[COLS];
  
  // Allocate memory for paths
  paths = new edge*[ROWS];
  for(int i = 0; i < COLS; i++) paths[i] = new edge[COLS];
  
  initWalls(map);
  initStart(map);
}

/*
 * Graph destructor
 * params: none
 * assumptions: none
 * return: none
 */
 Graph::~Graph(){
  for(int i = 0; i < GRRW; i++) delete walls[i];
  for(int i = 0; i < ROWS; i++) delete paths[i];
  delete walls;
  delete paths;
 }

/*
 * Graph copy constructor
 * Provides deep copy for walls and path
 * params: none
 * assumptions: none
 * return: none
 */
Graph::Graph(const Graph& oldGraph){
  startRow = oldGraph.startRow;
  startCol = oldGraph.startCol;

    // Allocate memory for walls
  walls = new edge*[GRRW];
  for(int i = 0; i < GRRW; i++) walls[i] = new edge[COLS];

  // Copy walls
  for(int i = 0; i < GRRW; i++)
    for(int j = 0; j < COLS; j++)
      walls[i][j] = oldGraph.walls[i][j];
  
  // Allocate memory for paths
  paths = new edge*[ROWS];
  for(int i = 0; i < COLS; i++) paths[i] = new edge[COLS];

  // Copy paths
  for(int i = 0; i < ROWS; i++)
    for(int j = 0; j < COLS; j++)
      paths[i][j] = oldGraph.paths[i][j];
}

/*
 * Initialise Walls
 * params:
 *      map - the maze layout and the starting location and heading of the robot
 * assumptions:
 *   assumes that 'map' will come in the form "...H...V..."
 *   where 'H' and 'V' are the horizontal and vertical walls
 *   no error checking in here
 * return: none
 */
void Graph::initWalls(char* map){
  // Initialise everything as unexplored
  for(int i = 0; i < GRRW; i++)
    for(int j = 0; j < COLS; j++)
      walls[i][j] = UNEXPL;

  // Split map into horizontal and vertical components
  String map_str = String(map);
  int posH = map_str.indexOf("H"); // position of 'H'
  int posV = map_str.indexOf("V"); // position of 'V'
  String horizontal = map_str.substring(posH+1, posV-1);
  String vertical = map_str.substring(posV+1);
  
  // Pad the vertical wall ends with UNEXPL
  for(int i = 1; COLS*i < vertical.length(); i++) 
  vertical = vertical.substring(0, (COLS*i)-1) + String(UNEXPL) + vertical.substring((COLS*i)-1, vertical.length());
  
  // Fill walls with the map's vertical and horizontal walls
  for(int i = 0; i < vertical.length(); i++) walls[(i/COLS)*2][i%COLS] = vertical[i]-48;
  for(int i = 0; i < horizontal.length(); i++) walls[(i/COLS)*2+1][i%COLS] = horizontal[i]-48;
}

/*
 * Initialise startRow and startCol
 * params:
 *      map - the maze layout and the starting location and heading of the robot
 * assumptions:
 *   assumes that 'map' will come in the form "SRC..."
 *   where 'S' signals the starting position, and 'R'/'C' are the row/column
 *   no error checking in here
 * return: none
 */
void Graph::initStart(char* map){
    // Set starting origin to (0, 0)
    startRow = 0;
    startCol = 0;

    // Get the starting positions from map
    String map_str = String(map);
    int posS = map_str.indexOf("S"); // position of 'S'
    startRow = GRID_ROW(map_str.substring(posS+1, posS+2).toInt());
    startCol = map_str.substring(posS+2, posS+3).toInt();
}

/*
 * Helper function for calcShortestPath()
 * Translates the adjacent cell position
 * params:
 *    i - direction (north, south, east, west)
 *    row and column of the cell
 * assumptions: none
 * return:
 *    true/false if valid
 */
static edge* getCellPos(int i, edge row, edge col){
    edge currRow, currCol;
    switch(i){
      case 0: // North Wall
        currRow = row - 1;
        currCol = col;
        break;
      case 1: // South Wall
        currRow = row + 1;
        currCol = col;
        break;
      case 2: // East Wall
        currRow = row;
        currCol = col + 1;
        break;
      case 3: // West Wall
        currRow = row;
        currCol = col - 1;
        break;
    }
    edge* result = new edge[2];
    result[0] = currRow;
    result[1] = currCol;
    return result;
}

/*
 * Helper function for getAdjVal()
 * Checks for immediate out of bounds
 * params:
 *    row and column of the cell
 * assumptions: none
 * return:
 *    true/false if valid
 */
static bool isInBounds(edge row, edge col){
    if(col < 0 || col > COLS) return false; // Column is out of bounds
    if(row < 0 || row > GRRW) return false; // Row is out of bounds
    return true;
}

/*
 * Helper function for calcShortestPath()
 * Gets the path values of the reachable cells given a cell
 * params:
 *    row and column of the cell
 * assumptions: none
 * return:
 *    array of reachable cell values
 */
edge* Graph::getAdjVal(edge row, edge col){
  int N = ROWS*COLS - 1;
  edge* result = new edge[4];
  edge gridRow, gridCol;
  for(int i = 0; i < 4; i++){ // Iterate through adjacent cells
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
    if(isInBounds(gridRow, gridCol)){ // Make sure adjacent cell is valid
      if(gridRow == GRRW) gridRow--; // Edge case
      edge wallVal = walls[gridRow][gridCol];
      if(!(wallVal & (WALL|UNEXPL))){ // Adjacent cell is reachable
        edge* neighbours = getCellPos(i, row, col);
        result[i] = paths[neighbours[0]][neighbours[1]];
      } else { // Adjacent cell is unreachable
        result[i] = N+1; // No value
      }
    } else { // Adjacent cell is unreachable
      result[i] = N+1; // No value
    }
  }
  return result;
}

/*
 * Private helper function for calcShortestPath()
 * Populate 'paths' using the flood fill algorithm
 * params: none
 * assumptions: none
 * return: none
 */
void Graph::floodFill(){
  // Initialise each cell in paths to be the maximum value
  int N = ROWS*COLS - 1;
  for(int i = 0; i < ROWS; i++){
    for(int j = 0; j < COLS; j++){
      paths[i][j] = N;
      if(i == startRow && j == startCol) paths[i][j] = 0; // Goal
    }
  }  
  edge goal = 0;        // Value we want to reach
  edge currentVal = 0;  // value we currently have
  int mazeVal = 1;      // flag for checking if paths has updated

  while(mazeVal){
    mazeVal = 0;
    for(int i = 0; i < ROWS; i++){
      for(int j = 0; j < COLS; j++){
        if(paths[i][j] == currentVal){
          // Check if a wall exists for each direction
          edge* neighbours = getAdjVal(i, j); // get the adjcent values for this cell
          for(int k = 0; k < 4; k++){
            if(neighbours[k] == N){ // Update the value accordingly
              edge* currPos = getCellPos(k, i, j);
              paths[currPos[0]][currPos[1]] = currentVal+1;
              mazeVal = 1;
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
 * Return: true if a valid path exists
 */
bool Graph::calcShortestPath(){
  floodFill(); // populate the paths structure
    
  int N = ROWS*COLS - 1;
  edge currVal = paths[ROWS/2][COLS/2]; // Value at the center of the map (the goal)
  if(currVal == N) return false; // No valid path exists

  edge minPath[ROWS][COLS]; // Minimum turn path
  edge curPath[ROWS][COLS]; // Current path
  for(int i = 0; i < ROWS; i++){ // Initialise curPath and minPath
    for(int j = 0; j < COLS; j++){
      curPath[i][j] = N;
      minPath[i][j] = N;
    }
  }
  edge* curPathPos = new edge[2*ROWS*COLS]; // Positions of the current path's cells
  for(int i = 0; i < 2*ROWS*COLS; i++) curPathPos[i] = N;
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
            edge neigCell[2] = {0};
            edge* neighbours = getAdjVal(curCell[0], curCell[1]); // get the adjcent values for this cell
            // Find the location of the next cell
            for(int k = 0; k < 4; k++){
              if(neighbours[k] == currVal-1){ // Potential next cell found, add it to array
                  // store the location of the neighbour
                  edge* next = getCellPos(k, curCell[0], curCell[1]);
                  
                  neigCell[0] = next[0];
                  neigCell[1] = next[1];
                  count++;
                  int i;
                  for(i = 0; visited[i] != 0; i++);
                  visited[i] = currVal;
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
            curPathPos[shortCount] = neigCell[0];
            curPathPos[shortCount+1] = neigCell[0];
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
            for(int i = 2*2; curPathPos[i] != N; i += 2){
                // Get every second cell
                edge pRow = curPathPos[i-4];
                edge pCol = curPathPos[i-3];
                edge cRow = curPathPos[i];
                edge cCol = curPathPos[i+1];

                edge difRow = cRow - pRow;
                edge difCol = cCol - pCol;
                if(difRow == 0 && difCol == 0) curTurns++;
            }
        }
        if(curTurns == 0) break;
        for(int i = 0; i < 2*ROWS*COLS; i++) curPathPos[i] = N;

        Serial.print("========== CURRENT PATH ==========\n");
        for(int i = 0; i < ROWS; i++) {
            Serial.print("    ");
            for(int j = 0; j < COLS; j++) {
                if(curPath[i][j] == 44) Serial.print("*");
                else Serial.print(curPath[i][j]);
                Serial.print(" ");
            }
            Serial.print("\n");
        }
        Serial.print("Number of turns: ");
        Serial.println(curTurns);
        Serial.print("==================================\n");

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

    return true;
}



/*
 * Private helper function for printGraph()
 * Converts cell position into a printable symbol
 */
String Graph::getSymbol(edge row, edge col){
  edge wall = walls[row][col];
  edge path = paths[row/2][col];

  // Start and End edge case
  if(row == (ROWS/2 + 2) && col == COLS/2){ // Center of the maze
    switch(wall){
      case WALL:
        return " X |";
      case NOWALL:
        return " X  ";
      case UNEXPL:
        return " X *";
      default:
        return " X *";
    }
  }
  if(row == startRow && col == startCol) {
    switch(wall){
      case WALL:
        return " S |";
      case NOWALL:
        return " S  ";
      case UNEXPL:
        return " S *";
      default:
        return " S *";
    }
  }

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
    if(path == ROWS*COLS - 1){ // No path value
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
        return (path/10) ? String(path) + " |" : " " + String(path) + " |";
      case NOWALL:
        return (path/10) ? String(path) + "  " : " " + String(path) + "  ";
      case UNEXPL:
        return (path/10) ? String(path) + " *" : " " + String(path) + " *";
      default:
        return "   *";
    }
  }
}

/*
 * Print graph
 * params: none
 * assumptions: none
 * return: none
 */
void Graph::printGraph(){
  Serial.print("  --- --- --- --- --- --- --- --- ---\n");
  for(int i = 0; i < GRRW; i++){
    if(i%2 == 0) Serial.print("| ");
    else Serial.print("  ");
    for(int j=0; j < COLS; j++){
      Serial.print(getSymbol(i, j));
    }
    if(i%2 == 0) Serial.print(" |");
    Serial.print("\n");
  }
  Serial.println("  --- --- --- --- --- --- --- --- --- ");
}

bool isExplored(Graph* g){
  Graph exploredGraph = *g; // create a copy of the graph
  for(int i = 0; i < GRRW; i++){
    for(int j = 0; j < COLS; j++){
      if(i%2 && j == COLS) continue;
      if(exploredGraph.walls[i][j] == UNEXPL){
        exploredGraph.walls[i][j] = NOWALL;
      }
    }
  }
  exploredGraph.calcShortestPath();

  for(int i = 0; i < ROWS; i++){
    for(int j = 0; j < COLS; j++){
      if(exploredGraph.paths[i][j] - g->paths[i][j] != 0){
        // some discrepancies in shortest path exist, graph is not sufficiently explored
        Serial.println("Graph is not sufficiently explored, a shorter path exists: ");
        exploredGraph.printGraph();
        return false;
      }
    }
  }

  return true; // paths is the exact same, graph is sufficiently explored
}
