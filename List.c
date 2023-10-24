#include "List.h"

List* createList() {

	List* list = (List*)malloc(sizeof(List));
	Process* array = (Process*)malloc(sizeof(Process) * 5);
	list->size = 0;
	list->capacity = 5;
	list->array = array;
	
	return list;
}

void addProcess(List* list, Process* proc) {
	
	if (list->size >= list->capacity)
		increaseCapacity(list);
	
	/*Client client;
	client.id = (char*)malloc(sizeof(char) * (strlen(id) + 1));
	strcpy(client.id, id);
	client.socket_id = socket_id;
	client.process_id = process_id;*/
	
	list->array[list->size++] = *proc;
}

void removeProcess(List* list, char* processName) {
	int flagFound = 0;
	for (int i = 0; i < list->size; i++) {
	
		if(flagFound)
			list->array[i-1] = list->array[i];	
		
		if (strcmp(list->array[i].processName, processName) == 0)
			flagFound=1;
		
	}
	if(flagFound){
		list->size--;
	}
	else
		printf("Process not found to remove");
	printList(list);
}

void increaseCapacity(List* list) {
	
	list->capacity += 5;
	list->array = (Process*)realloc(list->array, sizeof(list->capacity));
}



Process* getProcess(List* list, char* processName) {
	
	for (int i = 0; i < list->size; i++) {
		
		if (strcmp(list->array[i].processName, processName) == 0)
			return &list->array[i];
	}
	printf("Process not found");
	return NULL;
}



void printList(List* list) {
	
	for (int i = 0; i < list->size; i++)
		printf("<Process_Name: %s, qid: %d > - ", list->array[i].processName, list->array[i].qid);
	printf("\n");	
}





