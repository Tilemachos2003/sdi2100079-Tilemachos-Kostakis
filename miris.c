#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"


// Reads the input File
void readFile(const char* fileName, Graph* graph) {
  FILE* file = fopen(fileName, "r");
  if (!file) {
    printf("Error while opening read file %s\n", fileName);
    return;
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = 0;

    char* src = strtok(line, " ");
    char* dest = strtok(NULL, " ");
    char* amount = strtok(NULL, " ");
    char* date = strtok(NULL, " ");

    int sum = atoi(amount);

    addEdge(graph, src, dest, sum, date);
  }

  fclose(file);
  return;
}

// Writes on the output File
void writeFile(const char* fileName, Graph* graph) {
  FILE* file = fopen(fileName, "w");
  if (!file) {
    printf("Error while opening write file %s\n", fileName);
    return;
  }

  printGraph(file, graph);
  fclose(file);
  return;
}

//Finds a string in prompt
int findInPrompt(int argc, char** argv, char* searchChar) {
  int i = 0;
  while (i < argc) {
    if (strcmp(searchChar, argv[i]) == 0) { break; }
    i++;
  }

  if (i == argc) { return -1; }
  return i;
}

// The main of miris project
int main(int argc, char** argv) {
  int inPos, outPos;
    
  Graph* graph = createGraph();

  inPos = findInPrompt(argc, argv, "-i") + 1;
  outPos = findInPrompt(argc, argv, "-o") + 1;
  
  if (inPos != 0 && outPos != 0) {
    readFile(argv[inPos], graph);
  }else {
    printf("The program should be initiated like this:>> ./miris -i inputFile -o outputFile\n");
  }

  promptUser(graph);

  writeFile(argv[outPos], graph);
  freeGraph(graph);

  return 0;
}