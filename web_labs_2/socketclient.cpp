#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <ctime>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const char* HOST = "192.168.170.250";
const char* PORT = "8080";

int main(){
        system("chcp 65001");

    ofstream fout;
    fout.open("ClientLog.txt", ios_base::app);

    time_t rawtime = time(NULL);
    struct tm * timeinfo;

    WSADATA wsaData;
    ADDRINFO hints;
    ADDRINFO* addrResult = NULL;
    SOCKET ConnectSocket = INVALID_SOCKET;

    const char* sendBuffer = "Mironov Andrei Dmitrievich M3O-110B-23";
    char recvBuffer[1024];

    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(result!=0){
        cout << "WSAStartup failed, result = " << result << endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    result = getaddrinfo(HOST, PORT, &hints, &addrResult);
    if(result!=0){
        cout << "getaddrinfo failed with error: " << result << endl;
        WSACleanup();
        return 1;
    }

    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if(ConnectSocket == INVALID_SOCKET){
        cout << "Socket creatioin failed" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if(result == SOCKET_ERROR){
        fout << "Unable connect to server\n";
        cout << "Unable coonect to server\n";
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    fout << "Connection time: " << asctime(localtime(&rawtime));
    fout << "Connection host: " << HOST << '\n';
    Sleep(5000);
    rawtime = time(NULL);

    result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
    if(result == SOCKET_ERROR){
        fout << "Send failed, error: " << result << '\n';
        cout << "Send failed, error: " << result << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    fout << "Sending time: " << asctime(localtime(&rawtime));
    fout << "Sent message: " << sendBuffer << '\n';
    rawtime = time(NULL);

    cout << "Bytes sent: " << result << endl;

    result = shutdown(ConnectSocket, SD_SEND);
    if(result == SOCKET_ERROR){
        fout << "Shutdown error: " << result << '\n';
        cout << "Shutdown error: " << result << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }


    do{
        ZeroMemory(recvBuffer, sizeof(recvBuffer));
        result = recv(ConnectSocket, recvBuffer, sizeof(recvBuffer), 0);
        if(result > 0){
            rawtime = time(NULL);
            fout << "Receive time: " << asctime(localtime(&rawtime));
            fout << "Received message: " << recvBuffer << '\n';
            cout << "Bytes received " << result << endl;
            cout << "Received data: " << recvBuffer << endl;
        }
        else if(result == 0){
            fout << "Connection closed" << '\n';
            cout << "Connection closed" << endl;
        }
        else{
            fout << "recv failed with error" << '\n';
            cout << "recv failed with error" << endl;
        }
    }while(result > 0);

    closesocket(ConnectSocket);
    freeaddrinfo(addrResult);
    WSACleanup();

    fout << '\n' << endl;
    fout.close();
    return 0;
}
