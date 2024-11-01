#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

// Creates the graph
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->nodeMap = createHashMap(INITIAL_TABLE_SIZE); 
    graph->edgeMap = createHashMap(INITIAL_TABLE_SIZE); 

    graph->lastNode = NULL;

    return graph;
}

// Function to create a new node and add it to the graph
Node* addNode(Graph* graph, const char* id) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->id = strdup(id);
    newNode->edges = NULL;
    newNode->next = graph->lastNode;
    graph->lastNode = newNode;
    graph->nodeCount++;
    addValue(&graph->nodeMap, (void*)newNode, newNode->id);

    return newNode;
}

// Takes in multiple strings and a number and conbines them to one string
// By changing the input of this function i have the flexibility to create
// the key based on any parameters of (the edge in this case) i want
char* edgeHashString(const char* src, const char* dest, int amount, const char* date) {
    int intLen = 0, amountCopy = amount;
    while ((amountCopy / 10) - (amountCopy % 10) != 0) {
        amountCopy /= 10;
        intLen++;
    }
    char toString[intLen];
    sprintf(toString, "%d", amount);

    char* result = (char *)malloc((strlen(src) * strlen(dest) * strlen(date) * intLen + 1) * sizeof(char));

    strcpy(result, src);
    strcat(result, " ");
    strcat(result, dest);
    strcat(result, " ");
    strcat(result, toString);
    strcat(result, " ");
    strcat(result, date);

    return result;
}


// Function to create a new edge and add it to a node
Edge* addEdge(Graph* graph, const char* src, const char* dest, int amount, const char* date) {
    Node *sourceNode = findNode(graph, src);
    if (sourceNode == NULL) {
        sourceNode = addNode(graph, src);
    }
    Node *destNode = findNode(graph, dest);
    if (destNode == NULL) {
        destNode = addNode(graph, dest);
    }
    
    Edge *newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = destNode;
    newEdge->amount = amount;
    newEdge->date = strdup(date);
    newEdge->next = sourceNode->edges;
    sourceNode->edges = newEdge;
    char* edgeString = edgeHashString(src, dest, amount, date);
    addValue(&graph->edgeMap, (void*)newEdge, edgeString);
    free(edgeString);

    return newEdge;
}

// Function to remove a node
Node* removeNode(Graph* graph, const char* id) {
    Node *previousNode = graph->lastNode;
    Node *currentNode = graph->lastNode;

    while (currentNode != NULL) {
        if (strcmp(currentNode->id, id) != 0) {
            previousNode = currentNode;
            currentNode = currentNode->next;
        }else { break; }
    }

    // Node not found
    if (currentNode == NULL) {
        printf("Node %s not found in graph\n", id);
        return NULL;
    }

    Edge *edge = currentNode->edges;
    while (edge != NULL) {
        Edge *tempEdge = edge;
        edge = edge->next;
        char* edgeString = edgeHashString(currentNode->id, tempEdge->destination->id, tempEdge->amount, tempEdge->date);
        removeValue(graph->edgeMap, edgeString);
        free(edgeString);
        free(tempEdge->date);
        free(tempEdge);
    }
    
    if (strcmp(currentNode->id, previousNode->id) != 0) { previousNode->next = currentNode->next; }
    else { graph->lastNode = currentNode->next; } 
    removeValue(graph->nodeMap, currentNode->id);
    free(currentNode->id);
    free(currentNode);

    return 0;
}

// Function to remove an edge
Edge* removeEdge(Graph* graph, const char* src, const char* dest, int amount, const char* date) {
    Node* sourceNode = findNode(graph, src);
    if (sourceNode == NULL) {
        printf("The node of the transaction is not found\n");
        return NULL;
    }

    Edge* currentEdge = sourceNode->edges;
    Edge* previousEdge = sourceNode->edges;
    while (currentEdge != NULL) {
        if (strcmp(dest, currentEdge->destination->id) != 0 || strcmp(date, currentEdge->date) != 0 || currentEdge->amount != amount) {
            previousEdge = currentEdge;
            currentEdge = currentEdge->next;
        }else { break; }

        // Node not found
        if (currentEdge == NULL) {
            printf("Edge not found in node %s \n", src);
            return NULL;
        }

    }

    if (previousEdge == currentEdge) { sourceNode->edges = currentEdge->next; }
    else { previousEdge->next = currentEdge->next; }
    char* edgeString = edgeHashString(src, dest, amount, date);
    removeValue(graph->edgeMap, edgeString);
    free(edgeString);
    free(currentEdge->date);
    free(currentEdge);

    return 0;
}

// Function to find a node by its ID
Node* findNode(Graph* graph, const char* id) {
    KeyValue* keyValue = findKeyValue(graph->nodeMap, id);
    if (keyValue == NULL) { return NULL; }
    return (Node*)findKeyValue(graph->nodeMap, id)->value;
}

// Function to find an edge by its ID
Edge* findEdge(Graph* graph, const char* src, const char* dest, int amount, const char* date) {
    char* edgeString = edgeHashString(src, dest, amount, date);
    KeyValue* keyValue = findKeyValue(graph->edgeMap, edgeString);
    if (keyValue == NULL) { return NULL; }
    Edge* edge = (Edge*)findKeyValue(graph->edgeMap, edgeString)->value;
    free(edgeString);
    return edge;
}

// Function to print the graph on the output file
void printGraph(FILE* file, Graph* graph) {
    Node *node = graph->lastNode;
    while (node != NULL) {
        Edge *edge = node->edges;
        if (edge == NULL) { fprintf(file, "%s\n", node->id); }
        while (edge != NULL) {
            fprintf(file, "%s %s %d %s\n", node->id, edge->destination->id, edge->amount, edge->date);
            edge = edge->next;
        }
        node = node->next;
    }
}

// Function to free all memory allocated for the graph
void freeGraph(Graph* graph) {
    Node *node = graph->lastNode;
    while (node != NULL) {
        Node *tempNode = node;
        node = node->next;

        // Free all edges of this node
        Edge *edge = tempNode->edges;
        while (edge != NULL) {
            Edge *tempEdge = edge;
            edge = edge->next;
            free(tempEdge->date);
            free(tempEdge);
        }
        free(tempNode->id);
        free(tempNode);
    }
    destroyHashMap(graph->nodeMap);
    destroyHashMap(graph->edgeMap);
    free(graph);
}