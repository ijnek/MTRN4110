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

  // Initialise paths
  for(int i = 0; i < ROWS; i++)
    for(int j = 0; j < COLS; j++)
      paths[i][j] = ROWS*COLS - 1;

  // Allocate and initialise memory for pathDir
  pathDir = new edge[ROWS*COLS];
  for(int i = 0; i < ROWS*COLS; i++) pathDir[i] = NONE;
  
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

  // Allocate memory and copy pathDir
  pathDir = new edge[ROWS*COLS];
  for(int i = 0; i < ROWS*COLS; i++)
    pathDir[i] = oldGraph.pathDir[i];
  

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
  String horizontal = map_str.substring(posH+1, posV);
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
      if(i == startRow && j == startCol) paths[i][j] = 0; // Starting cell
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
 * The shortest path requires the least number of turns
 * params: none
 * assumptions: none
 * return: true if a valid path exists
 */
bool Graph::calcShortestPath(){
  floodFill(); // populate the paths structure
    
  int N = ROWS*COLS - 1;
  edge goalVal = paths[ROWS/2][COLS/2];
  if(goalVal == N) return false; // No valid path exists

  // Initialise the minimum turn path array
  edge minPath[ROWS][COLS];
  for(int i = 0; i < ROWS; i++)
    for(int j = 0; j < COLS; j++)
      minPath[i][j] = N;
   
  // Array for the cell positions in minPath
  edge minPathPos[goalVal][2];
  for(int i = 0; i < goalVal; i++)
    for(int j = 0; j < COLS; j++)
      minPathPos[i][j] = N;
  minPathPos[0][0] = ROWS/2; minPathPos[0][1] = COLS/2;
  int minPathPosCount = 1; // index for minPathPos

  // Locations of next possible cells (4 possible availible cells * (row, column))
  edge nextCells[4][2];
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 2; j++)
      nextCells[i][j] = 10;
  int nextCellCount = 0;      // index for nextCells
  bool nextCellFlag = false;  // flag to say if the next cell has been found

  edge curVal = goalVal;                // current cell value to look for
  edge curCell[2] = { ROWS/2, COLS/2}; // current cell location
  int prev2cur[2] = {0}; // difference between the previous and current cell position
  int cur2next[2] = {0}; // difference between the current and next cell position
 
  // Loop until the current cell value is 1
  while(curVal > 1){
    edge* neighbours = getAdjVal(curCell[0], curCell[1]); // get the adjacent cell values
    
    // Find the potential next cells to go to (and fill into nextCells)
    for(int i = 0; i < 4; i++){
      if(neighbours[i] == curVal - 1){
        edge* next = getCellPos(i, curCell[0], curCell[1]);
        nextCells[nextCellCount][0] = next[0];
        nextCells[nextCellCount][1] = next[1];
        nextCellCount++;
      }
    }
    
    // Find if we can go straight for any of the nextCells
    for(int i = 0; i < nextCellCount; i++){
      cur2next[0] = nextCells[i][0] - curCell[0];
      cur2next[1] = nextCells[i][1] - curCell[1];
      
      // Next cell is either straight ahead or the one closest to the goal
      if((prev2cur[0] == cur2next[0] && prev2cur[1] == cur2next[1]) || (curVal == goalVal)){
        nextCellFlag = true;
        for(int j = 0; j < 2; j++){ // Update all values
          prev2cur[j] = cur2next[j];
          curCell[j] = nextCells[i][j];
          minPathPos[minPathPosCount][j] = nextCells[i][j];
        }
        minPath[nextCells[i][0]][nextCells[i][1]] = curVal-1;
        minPathPosCount++;
        break;
      }
    }
    // Could not drive forward, need to turn
    if(!nextCellFlag){
      for(int i = 0; i < 2; i++){ // Update all values
        prev2cur[i] = nextCells[0][i] - curCell[i];
        curCell[i] = nextCells[0][i];
        minPathPos[minPathPosCount][i] = nextCells[0][i];
      }
      minPath[nextCells[0][0]][nextCells[0][1]] = curVal-1;
      minPathPosCount++;
    }

    // Reset everything
    for(int i = 0; i < 4; i++)
      for(int j = 0; j < 2; j++)
        nextCells[i][j] = 0;
    nextCellCount = 0;
    nextCellFlag = false;
    curVal--;    
  }

  // Copy the minPath into paths
  for(int i = 0; i < ROWS; i++)
    for(int j = 0; j < COLS; j++)
      paths[i][j] = minPath[i][j];

  // Calculate the directions for pathDir
  minPathPos[minPathPosCount][0] = startRow; minPathPos[minPathPosCount][1] = startCol;
  pathDir[0] = STRT;
  for(int i = minPathPosCount-1; i > 0; i--){
    for(int j = 0; j < 2; j++){ // Update values
      prev2cur[j] = minPathPos[i+1][j] - minPathPos[i][j];
      cur2next[j] = minPathPos[i][j] - minPathPos[i-1][j]; 
    }
    if(prev2cur[1] - cur2next[1] == 0){ // going straight
      pathDir[minPathPosCount-i] = STRT;
    } else if(prev2cur[1] - cur2next[1] > 0) {
      pathDir[minPathPosCount-i] = LEFT;
    } else if(prev2cur[1] - cur2next[1] < 0){
      pathDir[minPathPosCount-i] = RGHT;
    }
  }
  for(int i = 1; i < minPathPosCount-1; i++){
    edge temp = pathDir[i+1];
    pathDir[i] = temp;
  }
  pathDir[minPathPosCount-1] = NONE;
  
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
    if(col == COLS-1){
      if(path >= ROWS*COLS - 1 || path <= 0){
        return "   ";
      }
      return (path/10) ? String(path) + " " : " " + String(path) + " ";
    }
    if(path >= ROWS*COLS - 1 || path <= 0){ // No path value
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

/*
 * Print the directions for the minimum turn path
 * params: none
 * assumptions: none
 * return: none
 */
void Graph::printDirections(){
  if(pathDir[0] == NONE){ // No directions exist
    Serial.println("\t--> No shortest path exists");
    return;
  }
  Serial.println("\t--> Directions to get to the goal");
  for(int i = 0; pathDir[i] != NONE; i++){
    Serial.print("\t");
    Serial.print(i+1);
    Serial.print(") ");
    switch(pathDir[i]){
      case STRT:
        Serial.println("Forward");
        break;
      case LEFT:
        Serial.println("Left");
        break;
      case RGHT:
        Serial.println("Right");
        break;
    }
  }
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
        Serial.println("\t--> Graph is not sufficiently explored, a shorter path exists: ");
        exploredGraph.printGraph();
        return false;
      }
    }
  }

  return true; // paths is the exact same, graph is sufficiently explored
}
