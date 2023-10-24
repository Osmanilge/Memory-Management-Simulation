Osman İlge Ünaldı
Sami Berkan Akkaya

Process Scheduler Assignment

compile via: 
make -f makefile.c

run via:
./Schedule
./Process <user_process_name> <file_name>

Scheduler

The Scheduler is a single instance program responsible for managing a 512KB main memory, divided into 1000 frames, each of size 512 bytes. This memory is allocated for User processes' pages.

The Scheduler is non-interactive and serves as the memory management backend.
User Process

User processes are interactive and can have multiple instances. When a User process starts, it reads a text file, considering it as its executable image. The file is divided into pages, each 512 bytes in size. The file size determines the size of the process's page table. For example, a 32KB file will result in a page table of 64 pages.

User processes run in an infinite loop, continually interacting with the Scheduler. The program accepts two command line parameters: Process name and the name of the text file to be processed.
Expected Workflow

    When a User process starts, it sends its process name and the content of the text file to the Scheduler.
    The Scheduler creates a page table and allocates memory frames for the User process based on the page table's size.
    The User process no longer needs the text file after this initialization.
    In the loop, the User process will request specific page numbers, which are then sent to the Scheduler.
    The Scheduler retrieves the content of the requested page (up to 512 bytes) using the page table and sends it to the User process.
    Both the Scheduler and the User process display the requested page's content to the standard output.

If an invalid page number is requested, the User process will be terminated (memory protection). If there are no available frames for allocation, the process will not be accepted (long-term scheduler rule). This simulation does not include virtual memory management.
