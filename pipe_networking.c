#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stats.h>

int to_server = open("server.c", O_CREAT, 0644);
int from_server = open("client.c",O_CREAT, 0664);
int to_client = from_server;
int from_client = to_server;

int client_handshake( int *x){
  if (to_server==x){  
  mkfifo("mario", 0644);
  printf("[SERVER] Made WKP");
  int fd = open("mario",O_WRONLY);
  read(
  }
  else{
    int p = getpid();
    mkfifo (p, 0664);
    open(
  
  
  return 0;
}
