program: Process.c Scheduler.c  List.h List.c
	gcc Process.c -o Process
	gcc Scheduler.c List.c -o Scheduler

