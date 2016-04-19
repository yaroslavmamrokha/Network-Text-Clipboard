#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H
#include<winsock2.h>
#include<Windows.h>
#include<iostream>
#include <conio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define IP_ADDRESS "127.0.0.1"
#define PORT 7777
#define  DEFAULT_LENGHT 10
#define LIB_NAME "ws2_32.lib"
#define LIB_VERSION 2
#pragma comment(lib,LIB_NAME)
using std::cerr;
using std::endl;
class Client_Network{
private:
	WSADATA wsa;
	SOCKET server_socket;
	struct sockaddr_in server_data;
	bool Initialise_Lib();
	bool Create_Socket();
	bool Connect_To_Server();
public:
	void Shutdown();
	explicit Client_Network();
	bool Send_Data(const char* send_data);
	bool Recv_Data(char* recv_data, int length = DEFAULT_LENGHT);
	~Client_Network();
};
#endif