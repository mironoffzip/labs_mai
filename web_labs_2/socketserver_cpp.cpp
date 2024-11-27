#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main()
{
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult = NULL;
	SOCKET ClientSocket = INVALID_SOCKET;	//Клиентский сокет
	SOCKET ListenSocket = INVALID_SOCKET;	//Слушающий сокет


	const char* sendBuffer = "Hello from server";

	char recvBuffer[512];	//Буфер для приема приветсвия от сервера

	int result;		//Эта переменная отвечает за коды возврата

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);	//Возвращает 0, если нет ошибок, число, если были ошибки
	if (result != 0) {
		cout << "WSAStartup failed, result = " << result << endl;
		return 1;
	}

	//Что необходимо передать в структуру hints для получения адреса
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;	//ipv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;	

	//Задаем параметры соединения (ответной части - в данном случае сервера)
	result = getaddrinfo(NULL, "666", &hints, &addrResult);		//1 параметр - адрес соединения, 2 - порт, на котором сервер слушает входные соединения, 3 - указатель на структуру addrinfo, которая содержит некоторые параметры соединения 
	if (result != 0) {
		cout << "getaddrinfo failed with error: " << result << endl;
		WSACleanup();
		return 1;
	}

	//Создаем сокет на базе addrResult
	ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);	//Эта функция верннет ID сокета
	if (ListenSocket == INVALID_SOCKET) {
		cout << "Socket creation failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	result = bind(ListenSocket, addrResult->ai_addr, addrResult->ai_addrlen);

	//Проверяем соединение
	if (result == SOCKET_ERROR) {
		cout << "Binding socket failed" << endl;
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	//Установка режима прослушивания сокета
	result = listen(ListenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR) {
		cout << "Listening socket failed" << endl;
		closesocket(ListenSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		cout << "Accepting socket failed" << endl;
		closesocket(ListenSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	closesocket(ListenSocket);

	do {
		ZeroMemory(recvBuffer, 512);
		result = recv(ClientSocket, recvBuffer, 512, 0);
		if (result > 0) {
			cout << "Received " << result << " bytes" << endl;
			cout << "Received data: " << recvBuffer << endl;

			result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
			if (result == SOCKET_ERROR) {
				cout << "Failed to send data back" << endl;
				closesocket(ClientSocket);
				freeaddrinfo(addrResult);
				WSACleanup();
				return 1;
			}
		}
		else if (result == 0) {
			cout << "Connection closing..." << endl;
		}
		else {
			cout << "recv failed with error" << endl;
			closesocket(ClientSocket);
			freeaddrinfo(addrResult);
			WSACleanup();
			return 1;
		}
	} while (result > 0);

	result = shutdown(ClientSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
		cout << "shutdown client socket failed" << endl;
		closesocket(ClientSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	freeaddrinfo(addrResult);
	WSACleanup();
	return 0;
}
