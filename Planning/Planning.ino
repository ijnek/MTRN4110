#include "planning.h"

Graph* graph = new Graph((char*)"H101112222010012222010001222010010222V0000122201002222001102221011002200000122");
void setup(){
  Serial.begin(9600);
  graph->calcShortestPath(0, 0);
  graph->printGraph();
}

void loop(){
}
