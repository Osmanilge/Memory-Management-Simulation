#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int qid;
	char processName[50];
	
	int sizeOfAllocatedPage;
	int lastFittedPageIndex;
	int globalPageZeroIndex;
} Process;

typedef struct {
	Process* array;
	int size;
	int capacity;
} List;




List* createList();

void addProcess(List* list, Process* proc);

void removeProcess(List* list, char* processName);

void increaseCapacity(List* list);

Process* getProcess(List* list, char* processName);

void printList(List* list);
