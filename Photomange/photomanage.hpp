#ifndef _PHOTOMANAGE_
#define _PHOTOMANAGE_
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <stdio.h>
#include <sstream>
#pragma warning(disable:4996)

using namespace std;
///class RecordType =================================
class RecordType {
public:
	int ReadRecordFromKB(); //read a record from kb
	int ReadPhotoNameFromKB();//NEW ONE
	int ReadEventNameFromKB();//read a eventname from kb
	int ReadSizeFromKB();//read a size(int) from kb
	int ReadContentsFromKB();// read a contents from kb
	int ReadRecordFromFile(ifstream &inFile); //read A record from file
	int WriteRecordToFile(ofstream &outFile);//write a record to file
	void GenFileName(string &name);//Generate File name using TimeNow funcion
	void DisplayRecord(); //Display records on Screen
	string GetPhotoName(); //returns photoname
	string GetEventName();
	string GetContents();
	bool IsEqual(string name); //compare photoNmae with input string
	bool IsEqualEvent(string name);//compare event name
	bool IsContentsIncluded(string name);//check this word is included
	int Compare(string nameKey);
	bool operator<(RecordType inItem);
	bool operator>(RecordType inItem);
private:
	string photoName;
	string eventName;
	int size;
	string contents;
};
///class eventType =================================
class eventType {
public:
	eventType() {};
	~eventType() {};
	string GetEventName();
	vector<string> GetFileNameList();
	void AddFileName(const string&);
	void SetEventName(string);
private:
	string eventName;
	vector<string> memPhotoList;
	int numOfphoto;
};
///class ApplicationType =================================
class ApplicationType {
public:
	ApplicationType() {
		photoList.clear();
		length = 0;
		inFileName = "photo.dat";
		outFileName = "photo.dat";
	}
	int AddRecordTolist();//read a record from keyboard and add to the list
	int ReadAllFromFile();
	void DisplayAllOnScreen();
	int WriteAllToFile();
	int GetCommand();
	int Run();
	int SearchByPrimaryKey(const string &inName); //search by file name
	int RetrieveByPhotoName();
	int DeleteByPhotoName();
	int RetrieveByEvent();
	int RetrieveByContents();
	int BinarySearchByPrimaryKey(string inName);//find key by binary search
	int AddEventToList(eventType &); //event를 evenlist에 추가
	int SearchEventList(const string&);
	void DisplayEventList();
	int RetrieveFromEventList();

	//void FindAndDisplay();
	//void DeleteByName();
private:
	vector<RecordType> photoList;
	int length;
	string inFileName, outFileName;
	vector<eventType> eventList;
};

#endif