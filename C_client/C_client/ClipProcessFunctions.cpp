#include "ClipProcess.h"
#include<fstream>
using std::ifstream;
using std::ofstream;
using std::getline;
ClipProcess::ClipProcess(){
	InComeData = 0;
	OutComeData = 0;
	dont_dealloc = 0; //to avoid deallocating memory of loacl buffer
}

ClipProcess::~ClipProcess(){
}



void ClipProcess::Set_Clipboard_Data(){
	Set_Copy();
	string temporary = Get_Income_Data();
	if (OpenClipboard(NULL))
	{
		InComeData = GlobalAlloc(GMEM_DDESHARE, temporary.length()+1);
		char * pData = (char*)GlobalLock(InComeData);

		string copy_data = temporary;
		strcpy(pData, copy_data.c_str()); //copy to clipboard
		GlobalUnlock(InComeData);

	OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, InComeData);
		CloseClipboard();
	}
	
}


void ClipProcess::Get_Clipboard_Data(){
	if (IsClipboardFormatAvailable(CF_TEXT)){
		if (OpenClipboard(NULL))
		{
			OutComeData = GetClipboardData(CF_TEXT);
			Set_Clip_Data((char*)OutComeData);
			CloseClipboard();
		}
	}
}
void ClipProcess::isClipDataChanged(){
	cout << endl << "Checking if clipboard data changed!" << endl;
	while (true){
		Sleep(1000);    //1 sec pause used to stop multiple checks that block clipboard use in other functions
		Get_Clipboard_Data();
		string _copy = Get_Copy();
		string _clip_data = Get_Clip_Data();
		if (_copy != _clip_data){
			Set_Copy();
			int len = _clip_data.length();
			Set_Storage(to_string(len));
			cout << "Sending data!" << endl;

			if (!protocol.Send_Data(Get_Storage().c_str())){
				cerr << "Failed to send data, disconnecting from server..." << endl;
				protocol.Shutdown();
				_getch();
				exit(1);
			}

			if (!protocol.Send_Data(_clip_data.c_str())){
				cerr << "Failed to send data, disconnecting from server..." << endl;
				protocol.Shutdown();
				_getch();
				exit(1);
			}
		}
	}
}

void ClipProcess::isDataReceived(){
	cout << endl << "Checking if data received!" <<endl;
	while (true){
		Sleep(200);
		char * len = new char[DEFAULT_LENGHT];
		memset(len, 0, DEFAULT_LENGHT);
		if (!protocol.Recv_Data(len)){
			cerr << "Failed to receive data, closing connection...." << endl;
			Clear_Data(dont_dealloc, len);
			protocol.Shutdown();
			_getch();
			exit(1);
		}
		cout << "Data Received" << endl;
		char * buffer = new char[atoi(len) + 1];
		memset(buffer, 0, atoi(len) + 1);

		if (!protocol.Recv_Data(buffer, atoi(len))){
			cerr << "Failed to receive data, closing connection...." << endl;
			Clear_Data(buffer, len);
			protocol.Shutdown();
			_getch();
			exit(1);
		}

		Set_Income_Data(buffer);
		Set_Clipboard_Data();

		Clear_Data(buffer, len);

	}

	
}
void ClipProcess::Clear_Data(char*& buffer, char*& len){
	if (buffer != 0){
		delete[]buffer;
	}
	delete[]len;
}

void ClipProcess::Start(){

	

	Set_Income_Data(" "); //setting clipboard to " " symbol to avoid program crash if clipboard is empty !!! == 0
	Set_Clipboard_Data();
	Get_Clipboard_Data();
	Set_Copy();

	thread t_ClipboardChange(&ClipProcess::isClipDataChanged, this);
	thread t_DataReceive(&ClipProcess::isDataReceived, this);
	t_ClipboardChange.join();
	t_DataReceive.join();


}
void main(){
	system("color F0");
	SetConsoleTitle(TEXT("<<<<Network Clipboard Client>>>>"));
	ClipStorage * process = new ClipProcess;
	process->Start();
}