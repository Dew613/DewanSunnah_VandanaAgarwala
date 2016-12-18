#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <sys/stat.h>


#include "pipe_networking.h"

int server_handshake( int *x){
  //1) SErver creates FIFO (wkp)
  
  mkfifo("mario", 0644);
  printf("[SERVER] Made WKP\n");
  //2)connect wkp and client
  printf("[Server] trying to connect to client\n");
  int pd = open("mario",O_RDONLY);
  
  //6)server recieves client's fifo
  char pn[100];
  read(pd, pn, sizeof(pn));
  printf("[SERVER] Connected to Client %s\n", pn);

  //6.5) creating sub server
  int f = fork();
  //6.55) removing wkp
  if (f == 0){
    char * commands[3];
    commands[0] = "rm";
    commands[1] = "mario";
    commands[2] = NULL;
    int e = execvp(commands[0], commands);
    printf("[SUBSERVER %s] removed wkp\n", pn);
  }
  //7) send confirmation code
  int cp = open(pn, O_WRONLY);
  write(cp, "Hello Client", strlen("Hello Client"));

  return cp;
}


int client_handshake( int *x){
  //3)client creats private FIFO
  char p[100];
  int pid = getpid();
  sprintf(p, "%d", pid);
  mkfifo(p, 0644);
  printf("[Client %s] created private named pipe: %s \n", p,p);
  
  //4) client connects to server and sends private fifo
  int pd = open ("mario", O_WRONLY);
  printf("[Client %s] connected to server\n", p);
  write(pd, p, strlen(p));
  printf("[Client %s] sending information\n", p);
  
  //8) client recieves server's message throught private pipe
  int cp;
  cp= open (p, O_RDONLY);
  char smessage[100];
  read (cp, smessage, 100);
  printf("[Client %s] recieved message: %s\n", p, smessage);
  //9) client removes private pipe
  int f= fork();
  if (f==0){
    char * commands[3];
    commands[0] = "rm";
    commands[1] = p;
    commands[2] = NULL;
    int e = execvp(commands[0], commands);
    printf ("[Client %s] removed private pipe\n", p);
}
  //10)client sends confirmation
  write (cp, "Hello SERVER", 13);
  printf("[Client %s] sending message: Hello SERVER", p);


  return cp;
}
