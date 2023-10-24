#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct mesg_buffer {
    long mtype;
    int qid;
    int fileLength;
    int wantedPage;
    char process_name[50];
    char mtext[512];
    int error;
}message;



int main(int argc, char** argv) {

	

	char* file_name = (char*)malloc(sizeof(char) * (strlen(argv[2]) + 1));
	strcpy(file_name, argv[2]);

	FILE* fptr;

	fptr = fopen(file_name, "r");
	if (fptr == NULL) {
		printf("Error!");
		exit(1);
	}

	fseek(fptr, 0L, SEEK_END);
	int file_length = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
	
	//Create self message queue
	int qId, pid_client;
	pid_client = getpid();
	
	key_t key;
	if((key = ftok("/tmp", pid_client)) == -1)
	{
        	perror("process: self queue ftok failed!");
        	return EXIT_FAILURE;
    	}
	
	qId = msgget(key, 0666 | IPC_CREAT);
    	
    	//Fill the message
    	message.mtype = 1;
    	message.qid = qId;
    	message.fileLength = file_length-1;
    	
	strcpy(message.process_name, argv[1]);
	
	
    	struct mesg_buffer rcv;
    	//Get Scheduler qid
    	int msgid;
    	key_t keyS;
    	if((keyS = ftok("/tmp", 'S')) == -1)
	{
        	perror("process: public queue ftok failed!");
        	return EXIT_FAILURE;
    	}
    	
	msgid = msgget(keyS, 0666 | IPC_CREAT);
	if(msgsnd(msgid, &message, sizeof(struct mesg_buffer), 0) == -1){
		perror("msgsnd");

		return EXIT_FAILURE;
	}
	msgrcv(qId, &rcv, sizeof(struct mesg_buffer), 0, 0);
	if(rcv.error){
		printf("Dosyanın boyutu için yeterli alan bulunmamaktadır.\n");
		fclose(fptr);
		if (msgctl(qId, IPC_RMID, NULL) == -1) {
			perror("msgctl");

			return EXIT_FAILURE;
		}
      	  	exit(1);
	}
	int k=0;
	
	while(k<file_length-1){
		message.mtype=2;
		message.qid = qId;
    		message.fileLength = file_length-1;
    		strcpy(message.process_name, argv[1]);
		for(int i=0; i<512; i++){
			if(++k<file_length)
				message.mtext[i]=fgetc(fptr);
			else
				message.mtext[i]=0;
		}
		if(msgsnd(msgid, &message, sizeof(struct mesg_buffer), 0) == -1){
			perror("msgsnd");

			return EXIT_FAILURE;
		}
	}
	fclose(fptr);
	char content[513];
	content[513]='\0';
	while(1){
		message.mtype = 3;
		message.qid = qId;
    		message.fileLength = file_length-1;
    		strcpy(message.process_name, argv[1]);
		int index;
		printf("Erişmek istediğiniz sayfa indexini(sayı) giriniz :");
		scanf("%d",&index);
		message.wantedPage = index;
		
		if(msgsnd(msgid, &message, sizeof(message), 0) == -1){
			perror("msgsnd");

			return 1;
		}
		msgrcv(qId, &rcv, sizeof(rcv), 3, 0);
		if(rcv.error){
			printf("Erişmek istediğiniz indexe erişiminiz yok.\n");
			if (msgctl(qId, IPC_RMID, NULL) == -1) {
				perror("msgctl");

				return EXIT_FAILURE;
			}
      	  		exit(1);
		}
		strncpy(content,rcv.mtext,512);
		printf("%d numaralı page'deki bilgi: ", index);
		for (int i = 0; i < 512; i++) {
        		printf("%c", content[i]);
   		}
   		printf("\n");
	}
	if (msgctl(qId, IPC_RMID, NULL) == -1) {
		perror("msgctl");

		return EXIT_FAILURE;
	}
	return 0;
}



