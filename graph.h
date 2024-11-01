#include "hashMap.h"

typedef struct Graph Graph;
typedef struct Node Node;
typedef struct Edge Edge;

// Graph stucture (it will use the hash table to access each node quickly)
typedef struct Graph {
    HashMap* nodeMap;
    HashMap* edgeMap;
    Node* lastNode;
    int nodeCount;
} Graph;

// Node structure
typedef struct Node {
    char* id;
    Edge* edges;
    Node* next;
} Node;

// Edge structure
typedef struct Edge {
    char* date;
    int amount;
    Node* destination;
    Edge* next;
} Edge;

// Functions
Graph* createGraph();
Node* addNode(Graph* graph, const char* id);
Node* removeNode(Graph* graph, const char* id);
Edge* addEdge(Graph* graph, const char* src, const char* dest, int amount, const char* date);
Edge* removeEdge(Graph* graph, const char* src, const char* dest, int amount, const char* date);
Node* findNode(Graph* graph, const char* id);
Edge* findEdge(Graph* graph, const char* src, const char* dest, int amount, const char* date);
char* edgeHashString(const char* src, const char* dest, int amount, const char* date);
void printGraph(FILE* file, Graph* graph);
void freeGraph(Graph* graph);