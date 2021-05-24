#define _CRT_SECURE_NO_WARNINGS
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

	bind(soc, (const sockaddr *)&sa, sizeof(sa));
	listen(soc, 50);

	SOCKET client_socket;
	SOCKADDR_IN client_addr;

	while (1) {
		int client_addr_size = sizeof(client_addr);
		client_socket = accept(soc, (sockaddr*)&client_addr, &client_addr_size);
		if (client_socket == INVALID_SOCKET) {
			printf("trying to connect");
		}
		else {
			printf("connected");
		}
	}

	return 0;
}