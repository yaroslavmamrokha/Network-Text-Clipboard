#include "ClipStorage.h"

ClipStorage::ClipStorage(){
	income_data = "";
	clip_data = "";
	storage = "";
}
ClipStorage::~ClipStorage(){

}

void ClipStorage::Set_Income_Data(string in_data){
	income_data = in_data;
}

string ClipStorage::Get_Income_Data()const{
	return income_data;
}


void ClipStorage::Set_Clip_Data(string clp_data){
	clip_data = clp_data;
}

string ClipStorage::Get_Clip_Data()const{
	return clip_data;
}

void ClipStorage::Set_Storage(string stor_data){
	storage = stor_data;
}

string ClipStorage::Get_Storage()const{
	return storage;
}

void ClipStorage::Set_Copy(){
	copy = clip_data;
}
string ClipStorage::Get_Copy()const{
	return copy;
}