#include "planning.h"



void setup(){
  Serial.begin(9600);

  Serial.println("Data recieved:");
  // (char*)"S00H000000000000000000000000000000000000V0000000000000000000000000000000000000000"
  // (char*)"S00H111111111111111111111111111111111111V1111111111111111111111111111111111111111"
  // (char*)"S00H022111222000010222101111222222222222V1222222212100122000101222222222222222222"
  // (char*)"S00H101112222010012222010001222010010222V0000122201012222001102221011002200000122"
  // (char*)"S00H101112222010012222010001222010010222V0000122201002222001102221011002200000122"
  processGraph((char*)"S00H101112222010012222010001222010010222V0000122201012222001102221011002200000122");

}

void loop(){
  // Wait to recv a string from bluetooth
  // Process the graph
}

void processGraph(char* map){
  Graph* graph = new Graph(map);
  if(graph->calcShortestPath()) {
    Serial.println("\t---> Graph is has a valid path");
    Serial.println("\t---> The shortest, least #turn path is:");
    graph->printGraph();
    if(isExplored(graph)) Serial.println("\t---> Graph is sufficiently explored");
  } else {
    Serial.println("\tGraph is does not have a valid path");
  }
  delete graph;
}
