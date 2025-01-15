/*
Jash Thakkar
Homework 2
Account: jthakkar3
Due date: oct, 20

Input: n/a
Output: Current Process ID, Parent Process ID, and Group ID, and the ps data before and after a sleep function

Usage: to see your personal system's currenct process data and other relevent informations
*/

#include <stdio.h>
#include <unistd.h>  
#include <sys/types.h>
#include <stdlib.h>  
#include <string.h> 

int main() {

    printf("Starting C program\n");

    pid_t pid = getpid();  
    pid_t ppid = getppid(); 
    gid_t gid = getgid();
    uid_t uid = getuid();   

    printf("Process ID (PID): %d\n", pid);
    printf("Parent Process ID (PPID): %d\n", ppid);
    printf("Group ID (GID): %d\n", gid);


    uid_t my_uid = 501;  
    if (uid == my_uid) {
        printf("Welcome, Jash!\n");
        system("grep 501 /etc/passwd"); 
    } else {
        printf("User ID does not match Jash's account. The UID that is accociated with this account is: %d\n", uid);
    }

    printf("\nBefore Sleep\n");
    int returnValue = system("ps");

    returnValue = system("sleep 3");

    printf("\nAfter Sleep\n");
    returnValue = system("ps");

    printf("Ending C program\n");

    return 0;
}
