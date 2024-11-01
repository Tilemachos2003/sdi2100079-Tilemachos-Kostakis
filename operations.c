#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "operations.h"

// Returns the len of an int (ex: 1542 has len 4)
int intLen(int myInt) {
  int myIntCopy = myInt, intLen = 0;
  while ((myIntCopy / 10) - (myIntCopy % 10) != 0) {
      myIntCopy /= 10;
      intLen++;
  }

  return intLen;
}

int insert(Graph* graph, char* input) {
  if (strlen(input) < 4) {
    strcpy(input, "Format error: i(nsert) Ni [Nj Nk ...]");
    return -1;
  }

  char result[128] = "";
  char* token = strtok(input, " ");
  int checked = 0;
  Node* node = NULL;

  while (token != NULL) {
    if (token[strlen(token) -1] == '\n') { token[strlen(token) -1] = '\0'; }
    if ((token[0] == 'i' && !checked) || strcmp(token, "insert") == 0) { 
      token = strtok(NULL, " ");
      checked++;
      continue; 
    }
    if (findNode(graph, token) != NULL) {
      strcat(result, token);
      strcat(result, " ");
      token = strtok(NULL, " ");
      continue; 
    }
    if ((node = addNode(graph, token)) == NULL) { break; }
    strcat(result, token);
    strcat(result, " ");
    token = strtok(NULL, " ");
  }

  if (node == NULL) {
    strcpy(input, "IssueWith: ");
    strcat(input, result);
    return -1; 
  }

  strcpy(input, "Succ: ");
  strcat(input, result);

  return 0;
}

int insert2(Graph* graph, char* input) {
  int amount, count = 0, checked = 0;
  char src[10], dest[10], date[10];
  char* token = strtok(input, " ");

  while (token != NULL) {
    if (token[strlen(token) -1] == '\n') { token[strlen(token) -1] = '\0'; }
    if ((token[0] == 'n' && !checked) || strcmp(token, "insert2") == 0) { 
      token = strtok(NULL, " "); 
      checked++;
      continue; 
    }

    if (count == 0) {
      strcpy(src, token);
    }else if (count == 1) {
      strcpy(dest, token);
    }else if (count == 2) {
      amount = atoi(token);
    }else {
      strcpy(date, token);
    }

    count++;
    token = strtok(NULL, " ");
  }

  if (count != 4) {
    strcpy(input, "Format error: (i)n(sert) Ni Nj sum date");
    return -1;
  }

  strcpy(input, "");
  if (addEdge(graph, src, dest, amount, date) == NULL) { 
    strcat(input, src);
    strcat(input, " ");
    strcat(input, dest);
    return -1; 
  }

  return 0;
}

int delete(Graph* graph, char* input) {
  if (strlen(input) < 4) {
    strcpy(input, "Format error: d(elete) Ni [Nj Nk ...]");
    return -1;
  }

  char result[128] = "";
  char* token = strtok(input, " ");
  int checked = 0;

  while (token != NULL) {
    if (token[strlen(token) -1] == '\n') { token[strlen(token) -1] = '\0'; }
    if ((token[0] == 'd' && !checked) || strcmp(token, "delete") == 0) { 
      token = strtok(NULL, " ");
      checked++;
      continue; 
    }

    strcat(result, token);
    strcat(result, " ");
    removeNode(graph, token);
    token = strtok(NULL, " ");
  }

  strcpy(input, result);
  return 0;
}

int delete2(Graph* graph, char* input) {
  char command[8] = "", src[11] = "", dest[11] = "";
  sscanf(input, "%7s %10s %10s", command, src, dest);

  if (strlen(src) == 0 || strlen(dest) == 0) {
    strcpy(input, "Format error: (de)l(ete) Ni Nj sum date");
    return -1;
  }

  Node* node = findNode(graph, src);
  Edge* edge = node->edges;

  while (edge != NULL) {
    if (strcmp(edge->destination->id, dest) == 0) {
      removeEdge(graph, src, dest, edge->amount, edge->date);
      break;
    }
    edge = edge->next;
  }

  if (edge == NULL) {
    strcpy(input, "Non-existing node(s): ");
    strcat(input, src);
    strcat(input, " ");
    strcat(input, dest);
    return -1; 
  }

  return 0;
}

int modify(Graph* graph, char* input) {
  int count = 0, sum = 0, sum1 = 0;
  char* inputList[6];
  char* token = strtok(input, " ");

  while ((token = strtok(NULL, " ")) != NULL) {
    inputList[count] = (char*)malloc((strlen(token) + 1)* sizeof(char));
    strcpy(inputList[count], token);
    count++;
  }

  if (count != 6) {
    strcpy(input, "Format error: (m)o(dify) Ni Nj sum sum1 date date1");
    return -1;
  }

  sum = atoi(inputList[2]);
  sum1 = atoi(inputList[3]);

  Node* node = findNode(graph, inputList[0]);
  if (node == NULL) {
    strcpy(input, "Non-existing node(s): ");
    strcat(input, inputList[0]);
    strcat(input, " ");
    strcat(input, inputList[1]);
    return -1;
  }

  Edge* edge = node->edges;
  while (edge != NULL) {
    if (strcmp(edge->destination->id, inputList[1]) == 0 && edge->amount == sum && strcmp(edge->date, inputList[4]) == 0) {
      removeEdge(graph, node->id, edge->destination->id, edge->amount, edge->date);
      addEdge(graph, node->id, edge->destination->id, sum1, inputList[5]);
      break;
    }
    edge = edge->next;
  }

  if (edge == NULL) {
    strcpy(input, "Non-existing edge: ");
    strcat(input, inputList[0]);
    strcat(input, " ");
    strcat(input, inputList[1]);
    strcat(input, " ");
    strcat(input, inputList[2]);
    strcat(input, " ");
    strcat(input, inputList[3]);
    return -1;
  }

  for (int i = 0; i < 6; i++) {
    free(inputList[i]);
  }

  return 0;
}


int find(Graph* graph, char* input) {
  char command[7] = "", src[10] = "";
  sscanf(input, "%s %s\n", command, src);

  if (strlen(src) == 0) {
    strcpy(input, "Format error: f(ind) Ni");
    return -1;
  }

  Node* node = findNode(graph, src);
  if (node == NULL) {
    strcpy(input, "Non-existing node: ");
    strcat(input, src);
    return -1;
  }

  Edge* edge = node->edges;
  while (edge != NULL) {
    printf("%s %s %d %s\n", src, edge->destination->id, edge->amount, edge->date);
    edge = edge->next;
  }

  return 0;
}


// The prompt of the miris program it reads until the command e(xit)
// it identifies the type of the command and calls the needed function 
// to execute it
void promptUser(Graph* graph) {
  while (1) {
    char input[128];
    int result;
    printf("COMMAND: ");
    fgets(input, sizeof(input), stdin);

    if (strstr(input, "insert2") || (input[0] == 'n' && (input[1] == ' ' || input[1] == '\n'))) {
      if ((result = insert2(graph, input) == -1)) { printf("%s\n", input); }
    }else if (strstr(input, "insert") || (input[0] == 'i' && (input[1] == ' ' || input[1] == '\n') )) {
      result = insert(graph, input);
      printf("%s\n", input);
    }else if (strstr(input, "delete") || (input[0] == 'd' && (input[1] == ' ' || input[1] == '\n'))) {
      result = delete(graph, input);
      if (result == -1) { printf("Non-existing node(s): %s\n", input); }
    }else if (strstr(input, "delete2") || (input[0] == 'l' && (input[1] == ' ' || input[1] == '\n'))) {
      result = delete2(graph, input);
      if (result == -1) { printf("%s\n", input); }
    }else if (strstr(input, "modify") || (input[0] == 'm' && (input[1] == ' ' || input[1] == '\n'))) {
      result = modify(graph, input);
      if (result == -1) { printf("%s\n", input); }
    }else if (strstr(input, "find") || (input[0] == 'f' && (input[1] == ' ' || input[1] == '\n'))) {
      result = find(graph, input);
      if (result == -1) { printf("%s\n", input); }
    }else if (strstr(input, "exit") || (input[0] == 'e' && (input[1] == ' ' || input[1] == '\n')) ) {
      break;
    }else {
      printf("Unrecognized command\n");
    }
  }

  return;
}