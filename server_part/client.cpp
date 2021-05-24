#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment (lib, "Ws2_32.lib")

int main() {
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET soc;
	soc = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(8800);

	sa.sin_addr.S_un.S_addr = inet_addr("192.168.3.10");
	
	connect(soc, (const sockaddr*)&sa, sizeof(sa));

	return 0;
}