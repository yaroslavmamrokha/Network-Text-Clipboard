#include"Server_Network.h"

#include<fstream>
#include<string>
using std::ofstream;
using std::string;
Server_Network::Server_Network(){
	dont_dealloc = 0;  //NULL pointer used to check if we need to dealloc memory from local buffer or not;
	dont_free = 0;	//NULL socket used to check if we need to call closesocket fuction or not;
	server_socket = 0;
	client_socket = 0;
	client_socket2 = 0;
	client_data_size = sizeof(client_data);
	memset(&server_data, 0, sizeof(server_data));
	memset(&client_data, 0, sizeof(client_data));
	memset(&wsa, 0, sizeof(wsa));
	if (!Initialise_Lib()){
		cerr << "Failed to load winsock library, closing program..." << endl;
		_getch();
		exit(1);
	}
	if (!Create_Socket()){
		cerr << "Failed to create socket, closing program..." << endl;
		_getch();
		exit(1);
	}
	if (!Establish_Server()){
		cerr << "Failed to Create a server, try again later..." << endl;
		closesocket(server_socket);
		_getch();
		exit(1);
	}
	

}

Server_Network::~Server_Network(){
	closesocket(client_socket);
	closesocket(client_socket2);
	closesocket(server_socket);
	memset(&server_data, 0, sizeof(server_data));
	memset(&client_data, 0, sizeof(client_data));
	memset(&wsa, 0, sizeof(wsa));
}

void Server_Network::Err_Free(char*& buffer, SOCKET& sock,char*& len){
	if (buffer != 0){   
		delete[]buffer;
	}
	if (len){
		delete[]len;
	}
	if (sock != 0){
		closesocket(sock);
	}
}

bool Server_Network::Initialise_Lib(){
	if (WSAStartup(MAKEWORD(LIB_VERSION, LIB_VERSION), &wsa) < 0){
		return false;
	}
	return true;
}

bool Server_Network::Create_Socket(){
	
	server_data.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	server_data.sin_family = AF_INET;
	server_data.sin_port = htons(PORT);

	if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		return false;
	}
	return true;
}

bool Server_Network::Establish_Server(){
	if (bind(server_socket, (struct sockaddr*)&server_data, sizeof(server_data)) < 0){
		return false;
	}
	if (listen(server_socket, SOMAXCONN) < 0){
		return false;
	}
	return true;
}

bool Server_Network::Send_Data(const char* send_data, SOCKET clt_str){
	if (send_data == NULL){
		return false;
	}
	if (send(clt_str, send_data, strlen(send_data), NULL) < 0){
		return false;
	}
	return true;
}

bool Server_Network::Recv_Data(char* recv_data, SOCKET clt_str, int length){
	while (true){
		if (recv(clt_str, recv_data, length, NULL) < 0){
			return false;
		}
		return true;
	}
}

bool Server_Network::Accept_Connection(){
	if ((client_socket = accept(server_socket, (struct sockaddr*)&client_data, &client_data_size)) < 0){
		return false;
	}
	cout << "Accepted client with ip: " << inet_ntoa(client_data.sin_addr)<<std::endl;

	if ((client_socket2 = accept(server_socket, (struct sockaddr*)&client_data, &client_data_size)) < 0){
		return false;
	}
	cout << "Accepted client with ip: " << inet_ntoa(client_data.sin_addr) << std::endl;
	char buf[2] = "g";
	if (!Send_Data(buf, client_socket)){
		cerr << "Failed to send, disconnecting client......." << endl;
		Err_Free(dont_dealloc, client_socket, dont_dealloc);
	}
	Sleep(100);
	if (!Send_Data(buf, client_socket2)){
		cerr << "Failed to send, disconnecting client......." << endl;
		Err_Free(dont_dealloc, client_socket2, dont_dealloc);
	}
	Sleep(1000);
	return true;
}

void Server_Network::Recv_thread1(){   //client_socket 1
	while (true){
		char* len = new char[DEFAULT_LENGHT];

		memset(len, 0, 10);
		if (!Recv_Data(len, client_socket)){
			cerr << "Failed to receive, disconnecting client........" << endl;
			guard_lock.lock();
			Err_Free(dont_dealloc, client_socket, len);
			guard_lock.unlock();
			break;
		}

		char*buffer = new char[atoi(len) + 1];
		memset(buffer, 0, atoi(len) + 1);
		if (!Recv_Data(buffer, client_socket, atoi(len))){
			cerr << "Failed to receive, disconnecting client......." << endl;
			guard_lock.lock();
			Err_Free(buffer, client_socket, len);
			guard_lock.unlock();
			break;
		}

		if (!Send_Data(len, client_socket2)){
			cerr << "Failed to send, disconnecting client......." << endl;
			guard_lock.lock();
			Err_Free(buffer, client_socket2, len);
			guard_lock.unlock();
			break;
		}

		if (!Send_Data(buffer, client_socket2)){
			cerr << "Failed to send, disconnecting client......." << endl;
			guard_lock.lock();
			Err_Free(buffer, client_socket2, len);
			guard_lock.unlock();
			break;
		}

		Err_Free(buffer, dont_free, len);

	}
}

void Server_Network::Recv_thread2(){
	while (true){
		char* len = new char[DEFAULT_LENGHT];
		if (!Recv_Data(len, client_socket2)){
			cerr << "Failed to receive, disconnecting client....." << endl;
			guard_lock.lock();
			Err_Free(dont_dealloc, client_socket2, len);
			guard_lock.unlock();
			break;
		}

			char*buffer = new char[atoi(len) + 1];
			memset(buffer, 0, atoi(len) + 1);
			if (!Recv_Data(buffer, client_socket2, atoi(len))){
				cerr << "Failed to receive, disconnecting client....." << endl;
				guard_lock.lock();
				Err_Free(buffer, client_socket2, len);
				guard_lock.unlock();
				break;
			}

			if (!Send_Data(len, client_socket)){
				cerr << "Failed to send, disconnecting client......" << endl;
				guard_lock.lock();
				Err_Free(buffer, client_socket, len);
				guard_lock.unlock();
				break;
			}

			if (!Send_Data(buffer, client_socket)){
				cerr << "Failed to send, disconnecting client......" << endl;
				guard_lock.lock();
				Err_Free(buffer, client_socket, len);
				guard_lock.unlock();
				break;
			}
			Err_Free(buffer, dont_free, len);
		}
	}
	

void Server_Network::Start(){
	while (true){
		cout << endl << "*********************************************" << endl;
		cout << "Accepting connection...." << endl;
		Accept_Connection();
		
		cout << "Starting message manager...." << endl;
		thread thr1(&Server_Network::Recv_thread1, this);
		thread thr2(&Server_Network::Recv_thread2, this);
		thr1.join();
		thr2.join();
	


	}

}
void main(){
	system("color F0");
	SetConsoleTitle(TEXT("<<<<Network Clipboard Server>>>>"));
	Server_Network process;
	process.Start();

}
