#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <sys/stat.h>


int server_handshake( int *from_client){
  //1) SErver creates FIFO (wkp)
  
  mkfifo("mario", 0644);
  printf("[SERVER] Made WKP\n");
  //2)connect wkp and client
  printf("[Server] trying to connect to client\n");
  int pd = open("mario",O_RDONLY);
  
  //6)server recieves client's fifo
  char pn[13];
  read(pd, pn, 13);
  printf("[SERVER] Connected to Client %s\n", pn);

  //6.5) creating sub server
  int f = fork();
  //6.55) removing wkp
  if (f == 0){
    char * commands[3];
    commands[0] = "rm";
    commands[1] = "mario";
    commands[2] = NULL;
    printf("[SUBSERVER %s] removed wkp", pn);
    int check = execvp (commands[0],commands);
  }
  //7) send confirmation code
  int cp = open(pn, O_WRONLY);
  write(cp, "Hello Client", strlen("Hello Client")+1);
  printf("[Server] sending confirmation message\n");

  return cp;
}


int client_handshake( int *to_server){
  //3)client creats private FIFO
  char p[100];
  int pid = getpid();
  sprintf(p, "%d", pid);
  mkfifo(p, 0644);
  printf("[Client %s] created private named pipe: %s \n", p,p);
  
  //4) client connects to server and sends private fifo
  int pd = open ("mario", O_WRONLY);
  printf("[Client %d] connected to server\n", pid);
  write(pd, p, strlen(p));
  printf("[Client %d] sending information: %s\n", pid, p);
  
  //8) client recieves server's message throught private pipe
  int cp;
  cp= open (p, O_RDONLY);
  printf("[Client %d] connected privately to server", pid);

  char smessage[26];
  read (cp, smessage, 26);
  printf("[Client %d] recieved message: %s\n", pid, smessage);

  int f = fork();
  if (f == 0){
    char * commands[3];
    commands[0] = "rm";
    commands[1] = smessage;
    commands[2] = NULL;
    printf("[Client %d] removed private pipe\n", pid);
  }
  return cp;
}
