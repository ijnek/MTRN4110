#include "planning.h"

// Sample Inputs
// S00H000000000000000000000000000000000000V0000000000000000000000000000000000000000
// S00H111111111111111111111111111111111111V1111111111111111111111111111111111111111
// S00H022111222000010222101111222222222222V1222222212100122000101222222222222222222
// S00H101112222010012222010001222010010222V0000122201012222001102221011002200000122
// S00H101112222010012222010001222010010222V0000122201002222001102221011002200000122
// S00H000001110000010100000000000000000000V1010000011101001111111111111111101010000
// S00H110122222000011222000010022100010122V0000222200102222101101220110002201000122
// S08H011011000000010000100010022000001022V0010000000010101011101220101012210100022
// S77H000001022100010022000010000011011000V1010001201010112011101020001010100100000
// S40H101011010011010101001001010110011022V0100000000101000000101001011001200100002

String command;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  command = "";
  Serial.println("Ready...");
}

void loop(){  
  if (Serial1.available()) { // Read bluetooth serial
     char c = (char)Serial1.read();
     command += c;
  }

  if(command.length() == 81){ // Process the data
    Serial.print("Data recieved: ");
    Serial.println(command);
    processGraph((char*)command.c_str());
    command = "";
  }
  
}

void processGraph(char* map){
  Graph* graph = new Graph(map);
  graph->printGraph();
  if(graph->calcShortestPath()) {
    Serial.println("\t---> Graph has a valid path");
    Serial.println("\t---> The shortest, least #turn path is:");
    graph->printGraph();
    graph->printDirections();
    if(isExplored(graph)) Serial.println("\t---> Graph is sufficiently explored");
  } else {
    Serial.println("\tGraph does not have a valid path");
  }
  delete graph;
}
