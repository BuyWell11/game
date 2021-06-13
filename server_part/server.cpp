#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "Ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <SDL.h>
#include <SDL_image.h>

SOCKET Connections[50];
int conn_count = 0;

void ClientHandler(int index) {
	char msg[256];
	while (true) {
		recv(Connections[index], msg, sizeof(msg), NULL);
		for (int i = 0; i < conn_count; i++) {
			if (i == index) {
				continue;
			}

			send(Connections[i], msg, sizeof(msg), NULL);
		}
	}
}

int main(int argc, char* argv[]) {
	WSADATA ws;

	int data_count = 0;

	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET soc;
	soc = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(8800);

	bind(soc, (const sockaddr*)&sa, sizeof(sa));
	listen(soc, 50);

	SOCKET client_socket;
	SOCKADDR_IN client_addr;

	char buf[100] = { 0 };

	while (1) {
		int client_addr_size = sizeof(client_addr);
		client_socket = accept(soc, (sockaddr*)&client_addr, &client_addr_size);

		if (client_socket == INVALID_SOCKET) {
			printf("trying to connect");
			continue;
		}
		else {
			printf("connected %d\n", conn_count + 1);
			Connections[conn_count] = client_socket;
			send(Connections[conn_count], _itoa(conn_count, buf, 10), sizeof(buf), NULL);
			conn_count++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(conn_count - 1), NULL, NULL);
		}

	}

	system("pause");

	closesocket(soc);

	return 0;
}