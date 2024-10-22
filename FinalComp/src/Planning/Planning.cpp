#include "Planning.hpp"
#include "../Exploration/ExplorationEnums.h"

/*
 * Graph constructor
 * params:
 *    str - the maze layout and the starting location and heading of the robot
 * return: none
 */
Maze::Maze(String str){
    // Parse str
    int Spos = str.indexOf("S");    // Position and heading
    int Hpos = str.indexOf("H");    // Horizontal walls
    int Vpos = str.indexOf("V");    // vertical walls

    // Serial.print("HPos: ");
    // Serial.println(Hpos);
    
    #ifdef __DEBUG__
        String S = str.substring(Spos+1, Hpos-1);
        String H = str.substring(Hpos+1, Vpos-Hpos-1);
        String V = str.substring(Vpos+1);
    #else
        String S = str.substring(Spos+1, Hpos);
        String H = str.substring(Hpos+1, Vpos);
        String V = str.substring(Vpos+1);
    #endif

    // Serial.print("S:: ");
    // Serial.println(S);
    // Serial.print("H:: ");
    // Serial.println(H);
    // Serial.print("V:: ");
    // Serial.println(V);

    // Fill origin, heading, hNum and vNum
    origin = new Position;
    origin->row = S[0] - 48;
    origin->col = S[1] - 48;
    heading = S[2] - 48;
    hNum = H.length();
    // Serial.print("hNUm: ");
    // Serial.println(hNum);
    vNum = V.length();
    // Serial.print("vNum: ");
    // Serial.println(vNum);

    // Figure out the orientation
    orientation = new Position;
    if(hNum == 54){ // 5x9 orientation
        orientation->row = 5;
        orientation->col = 9;
    } else { // 9x5 orientation
        orientation->row = 9;
        orientation->col = 5;
    }

    // Allocate space for the walls
    hWalls = new int[hNum];
    vWalls = new int[vNum];

    // Fill the walls
    for(int i = 0; i < hNum; i++){
        hWalls[i] = H[i] - 48;
    }

    for(int i = 0; i < vNum; i++){
        vWalls[i] = V[i] - 48;
    }

    // Allocate space for the cells
    cells = new int*[orientation->row];
    for(int i = 0; i < orientation->row; i++) cells[i] = new int[orientation->col];

    // Initialise cells
    for(int i = 0; i < orientation->row; i++) {
        for(int j = 0; j < orientation->col; j++) {
            cells[i][j] = N;
        }
    }
}

/*
 * Graph constructor
 * params: none
 * return: none
 */
Maze::~Maze(){
    delete vWalls;
    delete hWalls;
    delete cells;
}

int* Maze::getAdjCells(Position p){
    int* result = new int[4];
    // Look at adjacent walls and check if there is a wall or not
    int wallVal, row, col;
    for(int i = 0; i < 4; i++){ // Iterate through adjacent cells
        // Make sure adjacent cell is within bounds
        if(!(p.col < 0 || p.col > orientation->col || p.row < 0 || p.row > orientation->row)){
            switch(i){
                case NORTH: // North
                    wallVal = hWalls[p.col + (orientation->col*p.row)];
                    row = p.row - 1;
                    col = p.col;
                    break;
                case SOUTH: // South
                    wallVal = hWalls[p.col + (orientation->col*p.row) + orientation->col];
                    row = p.row + 1;
                    col = p.col;
                    break;
                case EAST: // East
                    wallVal = vWalls[((orientation->col+1)*p.row) + p.col + 1];
                    row = p.row;
                    col = p.col + 1;
                    break;
                case WEST: // West
                    wallVal = vWalls[((orientation->col+1)*p.row) + p.col];
                    row = p.row;
                    col = p.col - 1;
                    break;
            }

            if(!(wallVal & (WALL|UNEXPL))){ // Adjacent cell has no wall
                if(!(col < 0 || col > orientation->col || row < 0 || row > orientation->row)){ //Check that adj cell is in bounds
                    result[i] = cells[row][col];
                    //std::cout << i << " " << result[i] << "\t";
                } else {
                    result[i] = N+1;
                }
            } else { // Adjacent cell is unreachable
                result[i] = N+1; // No value
            }
        } else { // Adjacent cell is unreachable
            result[i] = N+1; // No value
        }
    }
    return result;
}

int getRow(int n){
    switch(n){
        case 0:
            return -1;
        case 1:
            return 1;
        case 2:
        case 3:
            return 0;
    }
    return 0;
}

int getCol(int n){
    switch(n){
        case 0:
        case 1:
            return 0;
        case 2:
            return 1;
        case 3:
            return -1;
    }
    return 0;
}

/*
 * Private helper function for shortestPath()
 * Populate 'cells' using the flood fill algorithm
 * params: none
 * assumptions: none
 * return: none
 */
void Maze::floodFill(){
    // Initialise each cell to be the maximum value
    for(int i = 0; i < orientation->row; i++) {
        for(int j = 0; j < orientation->col; j++) {
            cells[i][j] = N;
        }
    }

    // Serial.print("orietntation->row: ");
    // Serial.println(orientation->row);

    // Serial.print("orietntation->col: ");
    // Serial.println(orientation->col);

    // Serial.println("mapbefore:");
    // for(int i = 0; i < orientation->row; i++) {
    //     for(int j = 0; j < orientation->col; j++) {
    //         Serial.print(cells[i][j]);
    //     }
    //     Serial.println("");
    // }
    // Serial.println("");


    cells[origin->row][origin->col] = 0; // Starting cell
    int currVal = 0;        // Value we currently have
    int updated = 1;       // Flag to check if cells has been updated

    while(updated){
        updated = 0;
        for(int i = 0; i < orientation->row; i++){
            for(int j = 0; j < orientation->col; j++){
                if(cells[i][j] == currVal){
                    // Check if a wall exists for each direction
                    Position p;
                    p.row = i; p.col = j;
                    int* adjWalls = getAdjCells(p); // get the adjcent values for this cell
                    for(int k = 0; k < 4; k++){
                        if(adjWalls[k] == N){ // Update the value accordingly
                            cells[i + getRow(k)][j + getCol(k)] = currVal+1;
                            updated = 1;
                        }
                    }
                }
            }
        }
        // Serial.println("updated");
        currVal++;
    }

    // Serial.println("map:");
    // for(int i = 0; i < orientation->row; i++) {
    //     for(int j = 0; j < orientation->col; j++) {
    //         Serial.print(cells[i][j]);
    //     }
    //     Serial.println("");
    // }
    // Serial.println("");
}


void Maze::getPath(int val, int currPos, Vector<Position>& v){
    if(val == 1) return;
    
    // find the most suitable neighbour and add it to v
    Position pos[4];
    Vector<Position> candidates(pos);
    Position p = v.at(currPos);
    int* adjVals = getAdjCells(p);
    for(int i = 0; i < 4; i++){ // Get potential neighbours
        if(adjVals[i] == val-1){
            Position toAdd;
            toAdd.row = p.row + getRow(i);
            toAdd.col = p.col + getCol(i);
            candidates.push_back(toAdd);
        }
    }
    if(v.size() > 1){
        int size = v.size();
        for(int i = 0; i < candidates.size(); i++){ // Get the one that takes the least turns
            int difRow = abs(candidates.at(i).row - v.at(currPos-1).row);
            int difCol = abs(candidates.at(i).col - v.at(currPos-1).col);
            if(difRow != 1 && difCol != 1){ // Goes straight, add it
                v.push_back(candidates.at(i));
                break;
            }
        }
        if(v.size() == size){ // Nothing was added, just add the first Position in candidates
            v.push_back(candidates.at(0));
        }
    } else { // Just add the first Position in candidates
        v.push_back(candidates.at(0));
    }


    return getPath(val-1, currPos+1, v);
}

void Maze::shortestPath(Vector<Position>& path){
    floodFill();

    // Serial.print(orientation->row);
    // Serial.print(", ");
    // Serial.print(orientation->col);

    int goalVal = cells[orientation->row/2][orientation->col/2];
    if(goalVal == N){
        // Serial.println("no valid path");
        return; // No valid path exists
    }

    Position p;
    p.row = orientation->row/2;
    p.col = orientation->col/2;
    path.push_back(p); // Add the goal

    // Get the shortest, least turn path
    getPath(goalVal, 0, path);

    // Add the origin
    path.push_back(*origin);

    // Reverse the path
    int end = path.size()-1;
    for (int i = 0; i < path.size()/2; i++) {
        Position temp = path.at(i);
        path.at(i) = path.at(end);
        path.at(end) = temp;
        end--;
    }

    #ifdef __DEBUG__
      for(int i = 0; i < path.size(); i++){
          std::cout << path.at(i).row << " " << path.at(i).col << std::endl;
      }
    #endif
}

#ifdef __DEBUG__
  void Maze::print(){
      std::cout << "------------------------------------------" << std::endl;
      std::cout << "Orientation: " << orientation->row << " " << orientation->col << std::endl;
      std::cout << "Origin: " << origin->row << " " << origin->col << std::endl;
      std::cout << "Heading: " << heading << std::endl;
  
      std::cout << "----------------- WALLS -------------------" << std::endl;
      int hCount = 0;
      int vCount = 0;
      for(int i = 0; i < orientation->row; i++){
          for(int j = 0; j < orientation->col; j++){
              std::cout << " ";
              int h = hWalls[hCount++];
              if(h == 2){
                  std::cout << "***";
              } else {
                  std::cout << (h ? "---" : "   ");
              }
          }
          std::cout << std::endl;
          for(int j = 0; j < orientation->col+1; j++){
              int v = vWalls[vCount++];
              if(v == 2){
                  std::cout << "*";
              } else {
                  std::cout << (v ? "|" : " ");
              }
              std::cout << "   ";
          }
          std::cout << std::endl;
      }
      for(int j = 0; j < orientation->col; j++){
          std::cout << " ";
          int h = hWalls[hCount++];
          if(h == 2){
              std::cout << "***";
          } else {
              std::cout << (h ? "---" : "   ");
          }
      }
      std::cout << std::endl;
      std::cout << "------------------------------------------" << std::endl;
      std::cout << "----------------- MAZE -------------------" << std::endl;
      for(int i = 0; i < orientation->row; i++) {
          for(int j = 0; j < orientation->col; j++) {
              std::cout << cells[i][j] << " ";
          }
          std::cout << std::endl;
      }
      std::cout << "------------------------------------------" << std::endl;
  }
#endif