#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_CONNECTIONS 2
#define IP_ADDRESS "127.0.0.1"
#define PORT 7777
#define  DEFAULT_LENGHT 10
#define LIB_VERSION 2
#include <iostream>
#include<winsock2.h>
#include<thread>
#include <stdio.h>
#include<conio.h>
#include<mutex>
#pragma comment (lib, "ws2_32.lib")
using std::cerr;
using std::endl;
using std::cout;
using std::thread;
using std::mutex;
class Server_Network{
private:
	mutex guard_lock;
	char* dont_dealloc;
	SOCKET dont_free;
	WSADATA wsa;
	SOCKET server_socket, client_socket, client_socket2;
	struct sockaddr_in server_data, client_data;
	int client_data_size;
	bool Initialise_Lib();
	bool Create_Socket();
	bool Establish_Server();
	bool Accept_Connection();
	bool Send_Data(const char* send_data, SOCKET sock);
	bool Recv_Data(char* recv_data, SOCKET sock, int length = DEFAULT_LENGHT);
	void Recv_thread1();
	void Recv_thread2();
	void Err_Free(char*&, SOCKET&, char*& a);
public:
	explicit Server_Network();
	void Start();
	
	~Server_Network();
};
#endif