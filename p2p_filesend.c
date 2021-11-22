#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>

#define MAX 256
#define END_LEN 3

char END[] = "END";
char IP_ADDR[] = "\0";
char PORT_NUM[] = "\0";


void main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("p2p_filesend (IP_ADDR) (PORT_NUM)");
		exit(1);
	}
	memcpy(IP_ADDR, argv[1], strlen(argv[1]));
	memcpy(PORT_NUM, argv[2], strlen(argv[2]));

	FILE* fp;

	WSADATA wsaData;
	SOCKET recvSock, sendSock;
	SOCKADDR_IN recvAddr, sendAddr;
	char buf[MAX + 1] = "\0";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup() error.\n");
		exit(1);
	}

	// First, try to connect
	// socket setting
	sendSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sendSock == INVALID_SOCKET) {
		printf("socket() error.\n");
		WSACleanup();
		exit(1);
	}

	// address seetting
	memset(&sendAddr, 0, sizeof(sendAddr));
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_addr.S_un.S_addr = inet_addr(IP_ADDR);
	sendAddr.sin_port = htons(atoi(PORT_NUM));

	if (connect(sendSock, (SOCKADDR*)&sendAddr, sizeof(sendAddr)) != SOCKET_ERROR) {
		printf("connect succeed.\n");

		char fileName[32] = "\0";
		printf("Please enter file name: ");
		gets_s(fileName, 32);
		fp = fopen(fileName, "r");
		while (feof(fp) == 0) {
			fread(buf, sizeof(char), MAX, fp);
			send(sendSock, buf, strlen(buf), 0);
			memset(buf, 0, MAX + 1);
		}
		send(sendSock, END, sizeof(END), 0);

		printf("File send complete.\n");
		fclose(fp);

		Sleep(1000);
	}
	else { // Second, go to listen step
		printf("can't found other peer.\n");
		closesocket(sendSock);

		// socket setting
		sendSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sendSock == INVALID_SOCKET) {
			printf("socket() error.\n");
			WSACleanup();
			exit(1);
		}

		// address setting
		memset(&sendAddr, 0, sizeof(sendAddr));
		sendAddr.sin_family = AF_INET;
		sendAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		sendAddr.sin_port = htons(atoi(PORT_NUM));

		if (bind(sendSock, (SOCKADDR*)&sendAddr, sizeof(sendAddr)) == SOCKET_ERROR) {
			printf("bind() error.\n");
			closesocket(sendSock);
			WSACleanup();
			exit(1);
		}

		if (listen(sendSock, 5) == SOCKET_ERROR) {
			printf("listen() error.\n");
			closesocket(sendSock);
			WSACleanup();
			exit(1);
		}

		printf("listen succeed.\n");
		int szRecvAddr = sizeof(recvAddr);
		recvSock = accept(sendSock, (SOCKADDR*)&recvAddr, &szRecvAddr);
		if (recvSock == INVALID_SOCKET || recvSock == SOCKET_ERROR) {
			printf("accept() error.\n");
			closesocket(sendSock);
			exit(1);
		}

		fp = fopen("encoded.txt", "wb");

		char compare[END_LEN + 1] = "\0";
		int l;
		while (l = recv(recvSock, buf, MAX, 0)) {
			for (int i = 1; i <= END_LEN; i++) {
				compare[END_LEN - i] = buf[strlen(buf) - i];
			}
			if (strcmp(compare, END) == 0) {
				fwrite(buf, sizeof(char), strlen(buf) - END_LEN, fp);
				break;
			}
			fwrite(buf, sizeof(char), l, fp);
		}
		fclose(fp);
	}

}
