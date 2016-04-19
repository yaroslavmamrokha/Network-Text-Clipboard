#include"Client_Network.h"
#include<string>
//for test

Client_Network::Client_Network(){
	server_socket = 0;
	memset(&server_data, 0, sizeof(server_data));
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
	if (!Connect_To_Server()){
		cerr << "Failed to connect to server, try again later..." << endl;
		closesocket(server_socket);
		_getch();
		exit(1);
	}
	std::cout << "Waiting second client....\n";
	char* p = new char[1];
	Recv_Data(p,1);
	std::string a{ p };
	if (!a.compare("g")){
		std::cout << "Client connected\n";
	}
	delete[] p;
	
	//std::cout << a << std::endl;
	//Send_Data("NAVI");
	//system("pause");

}

Client_Network::~Client_Network(){
	closesocket(server_socket);
	memset(&server_data, 0, sizeof(server_data));
	memset(&wsa, 0, sizeof(wsa));
}
void Client_Network::Shutdown(){						//same as destructor but used to shutdown network if error occured
	closesocket(server_socket);
	memset(&server_data, 0, sizeof(server_data));
	memset(&wsa, 0, sizeof(wsa));
}

bool Client_Network::Initialise_Lib(){
	if (WSAStartup(MAKEWORD(LIB_VERSION, LIB_VERSION), &wsa) < 0){
		return false;
	}
	return true;
}
bool Client_Network::Create_Socket(){
	server_data.sin_family = AF_INET;
	server_data.sin_port = htons(PORT);
	server_data.sin_addr.s_addr = inet_addr(IP_ADDRESS);

	if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		return false;
	}
	return true;
}

bool Client_Network::Connect_To_Server(){
	if (connect(server_socket, (struct sockaddr*)&server_data, sizeof(server_data)) < 0){
		return false;
	}
	return true;
}

bool Client_Network::Send_Data(const char* send_data){
	if (send_data == NULL){
		return false;
	}
	int data_length = strlen(send_data);
	if (send(server_socket, send_data, data_length, NULL) < 0){
		return false;
	}
	return true;
}

bool Client_Network::Recv_Data(char* recv_data, int length){
	while (true){
		if (recv(server_socket, recv_data, length, NULL) < 0){
			return false;
		}
		return true;
	}
}
