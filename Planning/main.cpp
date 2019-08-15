#include "planning.hpp"

#ifdef __DEBUG__
int main(){
    Position pathArray[15];
    Vector<Position> path(pathArray);

    Maze* maze = new Maze("S001H111111111101112222010012222010001222010010222111111111V10000122211010122221100110222111011002211000001221");
    maze->shortestPath(path);
    maze->print();

    // Position pathArray2[15];
    // Vector<Position> path2(pathArray2);

    // Maze* maze2 = new Maze("S081H111111111101100010001010000010000101100010100111111111V10000010011100000111110001011110110000011100011001");
    // maze2->shortestPath(path2);
    // maze2->print();

    return 0;
}
#endif
