#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <time.h>

#include "List.h"

struct mesg_buffer {
    long mtype;
    int qid;
    int fileLength;
    int wantedPage;
    char process_name[50];
    char mtext[512];
    int error;
}message;

char pages[512*1000] = {0};
int pageAvailable[1000];
int main() {
	for(int i = 0; i < 1000; i++)pageAvailable[i]=1;
	struct mesg_buffer rec_message;
	struct mesg_buffer sent_message;
	//Get Scheduler qid
    	int msgid;
    	key_t keyS;
    	if((keyS = ftok("/tmp", 'S')) == -1)
	{
        	perror("Scheduler: public queue ftok failed!");
        	return EXIT_FAILURE;
    	}
	
	msgid = msgget(keyS, 0666 | IPC_CREAT);
	
	printf("msgid: %d\n",msgid);
	List* users = createList();
	int flag=1;
	while(flag){
		if (msgrcv(msgid, &rec_message, sizeof(struct mesg_buffer), 0, 0) < 0)
		{
			perror("CLIENT: msgrcv function failed!");
			exit(1);
		}
		// msgrcv to receive message
    		
    		
    		//printf("%ld %d %d %s %s\n",rec_message.mtype, rec_message.qid, rec_message.fileLength, rec_message.process_name, rec_message.mtext);
    		Process* proc = malloc(sizeof(Process));
    		int k=0;
    		int start;
    		int state = (int)rec_message.mtype;
    		switch(state){
    		
    		case 1:	
    			sent_message.mtype=1;
    			printf("\ncase 1 began add user\n");
    			proc->qid = rec_message.qid;
    			strcpy(proc->processName,rec_message.process_name);
    			
    			printf("qid: %d, file lentgh: %d, process name: %s is in the system.\n", rec_message.qid, rec_message.fileLength, rec_message.process_name);
    			
    			int neededPageNumber = (int)((rec_message.fileLength-1)/512) + 1;
    			proc->sizeOfAllocatedPage = neededPageNumber;
    			k=0;
    			//is Memory available
    			int memoryAvaliable=0;
    			printf("needed: %d\n", neededPageNumber);
    			for(int i = 0; i < 1000; i++){
    				if(pageAvailable[i]){
    					k++;
    				}
    				else k=0;
    				
    				if(k == neededPageNumber){
    					proc->globalPageZeroIndex = i-k+1;
    					for(int j = proc->globalPageZeroIndex; j <= i; j++)pageAvailable[j]=0;
    					
    					memoryAvaliable = 1;
    					break;
    				}
    			}
    			if(memoryAvaliable){
    				proc->lastFittedPageIndex=0;
    				addProcess(users, proc);
    				sent_message.error=0;
    			}
    			else{
    				sent_message.error=1;
    			}
    			printf("errr%d",sent_message.error);
    			if(msgsnd(rec_message.qid, &sent_message, sizeof(sent_message), 0) == -1){
					perror("msgsnd");
				}
			printf("%d\n",rec_message.qid);
			printf("case 1 end\n");
    			break;
    		
    		case 2:
    			printf("\ncase 2 began get data\n");
    			proc = getProcess(users, rec_message.process_name);
    			if(proc == NULL){
    				printf("process is not found");
    				break;
    			}
    			
    			if(proc->lastFittedPageIndex == proc->sizeOfAllocatedPage){
    				printf("hata case 2\n");
    			}
    			
    			start = proc->globalPageZeroIndex + proc->lastFittedPageIndex++;
    			
    			
    			k=0;
    			char content[513];
    			content[513]='\0';
    			strncpy(content, rec_message.mtext, 512);
    			strncpy(pages+start*512, rec_message.mtext, 512);
    			
    			printf("%s, %d index (%dth page): ", proc->processName, proc->lastFittedPageIndex - 1, start);
    			for (int i = 0; i < 512; i++) {
        			printf("%c", content[i]);
   			 }
   			 printf("\n");
    			printf("case 2 end\n");
    			break;	
    			
    		case 3:	
    			printf("\ncase 3 began give data\n");
    			proc = getProcess(users, rec_message.process_name);
    			
    			
    			
    			
    			if(rec_message.wantedPage < proc->sizeOfAllocatedPage && rec_message.wantedPage >= 0){
    				k=0;
    				start = rec_message.wantedPage + proc->globalPageZeroIndex;
    				/*for(int i = start*512; i < start + 512; i++){
    					sent_message.mtext[k++] = pages[i]; 
    				}*/
    				strncpy(sent_message.mtext, pages+start*512, 512);
    				sent_message.error=0;
    				printf("%d numarlı index(%dth page) %s process için gönderildi:\n", rec_message.wantedPage, start, proc->processName);
    				for (int i = 0; i < 512; i++) {
        				printf("%c", sent_message.mtext[i]);
   				}
   			 	printf("\n");
    			}
    			else{	
    				printf("hata %s erişimi sonlandırıldı", proc->processName);
    				sent_message.error=1;
    				for(int i=0; i < proc->sizeOfAllocatedPage; i++)pageAvailable[i+proc->globalPageZeroIndex]=1;
    				removeProcess(users, proc->processName);
    				
    			}
    			sent_message.mtype=3;
    			if(msgsnd(rec_message.qid, &sent_message, sizeof(sent_message), 0) == -1){
					perror("msgsnd");
				}
    			printf("case 3 end\n");
    			break;
    		
    		}
    	
    	}


	return 0;
}
