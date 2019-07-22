#include "planning.h"
#include <vector>

using namespace std;

int main(){
     vector<char*> testVec;
     testVec.push_back((char*)"H000000000000000000000000000000000000V0000000000000000000000000000000000000000");
     testVec.push_back((char*)"H111111111111111111111111111111111111V1111111111111111111111111111111111111111");
     testVec.push_back((char*)"H022111222000010222101111222222222222V1222222212100122000101222222222222222222");
     testVec.push_back((char*)"H101112222010012222010001222010010222V0000122201012222001102221011002200000122");
     testVec.push_back((char*)"H101112222010012222010001222010010222V0000122201002222001102221011002200000122");

     for(auto t : testVec){
          Graph* graph = new Graph(t);
          graph->getShortestPath(0, 0);
          cout << *graph << endl;
     }

     // Can check if the unexplored part can get a better layout if you treat them all as no wall

     return 0;
}