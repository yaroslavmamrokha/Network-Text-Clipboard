#ifndef CLIPSTORAGE_H
#define CLIPSTORAGE_H
#include <string>
using std::string;
class ClipStorage{
private:
	string income_data;
	string clip_data;
	string storage;
	string copy;
protected:
	string Get_Income_Data()const;
	void Set_Income_Data(string);

	string Get_Clip_Data()const;
	void Set_Clip_Data(string);

	string Get_Storage()const;
	void Set_Storage(string);

	string Get_Copy()const;
	void Set_Copy();
public:
	explicit ClipStorage();
	virtual ~ClipStorage();
	virtual void Start() = 0;
};




#endif