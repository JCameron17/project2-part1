#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  ClientProcess(int []);

void deposit(int* accountNum);
void withdraw(int* accountNum);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
int fd ,i , nloop=5,zero=0,*counter_ptr;
    sem_t *mutex; 
  
  fd = open("log.txt",O_RDWR|O_CREAT,S_IRWXU);
  write(fd,&zero,sizeof(int));
  counter_ptr = mmap(NULL,sizeof(int),PROT_READ |PROT_WRITE,MAP_SHARED,fd,0);
  close(fd);

  /* create, initialize semaphore */
 if ((mutex = sem_open("examplesaaaebbmaphore", O_CREAT, 0644, 1)) == SEM_FAILED) {
    perror("semaphore initilization");
    exit(1);
  }
  
    pid = fork();
    if (pid > 0) {
        for (i = 0; i>-1; i++){
            sleep(rand()%6);
            printf("Dear Old Dad: Attempting to Check Balance\n");\
            sem_wait(mutex);
            int rnum = rand()%101;
            if (rnum % 2 == 0){
              if (*counter_ptr<100){
                depositMoney(counter_ptr);
              } else {
                printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", *counter_ptr);
              }
            }else{
              printf("Dear Old Dad: Last Checking Balance = $%d\n", *counter_ptr);
            }
            sem_post(mutex);              
        }
        
        exit(1);
    }
    else if (pid == 0) {
        for (i = 0; i>-1; i++){
            sleep(rand()%6);
            printf("Poor Student: Attempting to Check Balance\n");  
            sem_wait(mutex);
            int rnum = rand();
            if (rnum%2 == 0){
              withdrawMoney(counter_ptr);
            }else{
              printf("Poor Student: Last Checking Balance = $%d\n", *counter_ptr);
            }
            sem_post(mutex);
            
        }
        printf("   Client is about to exit\n");
        exit(0);
    }

    wait(&status);
    printf("Server has detected the completion of its child...\n");
    printf("Server has detached its shared memory...\n");
    printf("Server has removed its shared memory...\n");
    printf("Server exits...\n");
    exit(0);
}

void deposit(int* accountNum){
  int balance = *accountNum;
  int amount = rand() % 101;
  if (amount%2 == 0){
    balance+= amount;
    printf("Dear old Dad: Deposits $%d / Balance = $%d\n", amount, balance);
    *accountNum = balance;
  }
  else{
    printf("Dear old Dad: Doesn't have any money to give\n");
  }
}

void withdraw(int* accountNum){
  int balance = *accountNum;
  int extra = rand() % 51;
  printf("Poor Student needs $%d\n", extra);
  if (extra <= balance){
    balance-=extra;
    printf("Poor Student: Withdraws $%d / Balance = $%d\n", extra, balance);
    *accountNum = balance;
  }else{
    ("Poor Student: Not Enough Cash ($%d)\n", balance);
  }
}