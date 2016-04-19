#ifndef CLIPPROCESS_H
#define CLIPPROCESS_H
#include "Client_Network.h"
#include"ClipStorage.h"
#include<Windows.h>
#include <stdio.h>
#include <thread>
using std::to_string;
using std::thread;
using std::cout;
class ClipProcess : public ClipStorage{
private:
	HANDLE OutComeData;
	HGLOBAL InComeData;
	char* dont_dealloc;
	
public:
	Client_Network protocol;
	explicit ClipProcess();
	void isClipDataChanged();
	void Set_Clipboard_Data();
	void Get_Clipboard_Data();
	void Start();
	void isDataReceived();
	void Clear_Data(char*&, char*&);
	~ClipProcess();

};










#endif