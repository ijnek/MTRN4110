#include "planning.hpp"

#ifdef __DEBUG__
int main(){
    // Position pathArray[15];
    // Vector<Position> path(pathArray);

    // Maze* maze = new Maze("S001H111111111000000000000000000000000000000000000111111111V10000000011000000001100000000110000000011000000001");
    // maze->shortestPath(path);
    // maze->print();

    Position pathArray2[15];
    Vector<Position> path2(pathArray2);

    Maze* maze2 = new Maze("S001H111111111101100010001010000010000101100010100111111111V10000010011100000111110001011110110000011100011001");
    maze2->shortestPath(path2);
    maze2->print();

    return 0;
}
#endif
