#include "planning.hpp"

Position pathArray2[15];
Vector<Position> path2(pathArray2);

void setup(){
  
}

void loop(){
    Maze* maze2 = new Maze("S001H111111111101100010001010000010000101100010100111111111V10000010011100000111110001011110110000011100011001");
    maze2->shortestPath(path2);

    return 0;
}
