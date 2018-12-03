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


void TimeNow(string &ret);
int ReadRecordFromKB(string& fName, string& eventName);
int WriteRecordToFile(ofstream& outFile, string fName, string eventName);
int ReadRecordFromFile(ifstream& inFile, string& fName, string& eventName);
void WriteRecoredToScreen(string fName, string eventName);
int ReadStr(string& str);
int AddRecordToList(vector<string> &pName, vector<string> &eName);
int WriteAllToFile(vector<string> &nameList, vector<string> &eventList);
int ReadAllFromFile(vector<string> &nameList, vector<string> &eventList);
int SearchAllFromFile(vector<string> v, string inName);
int DisplayAllOnScreen(vector<string> &nameList, vector<string> &eventList);
void FindAndDisplay(vector<string> &nameList, vector<string> &eventList);
void DeleteByName(vector<string> &nameList, vector<string> &eventList);
int GetCommand();

int main()
{
	//출력을 위한 객체 outFile을 선언하고 파일 "photo.dat"을 오픈
	vector<string> nameList, eventList;

	int command;
	while ((command = GetCommand())) {
		switch (command) {
		case 0:
			return 1;
		case 1:
			AddRecordToList(nameList, eventList);
			break;
		case 2:
			ReadAllFromFile(nameList, eventList);
			break;
		case 3:
			WriteAllToFile(nameList, eventList);
			break;
		case 4:
			DisplayAllOnScreen(nameList, eventList);
			break;
		case 5:
			FindAndDisplay(nameList, eventList);
			break;
		case 6:
			DeleteByName(nameList, eventList);
			break;
		default:
			cout << "Invalid Operation \n";
			break;
		}
	}
	return 0;
}
void TimeNow(string &ret)
{
	//Declaring argument for time()
	time_t tt;
	//Declaring variable to store return value of localtime()
	struct tm * ti;
	//Applying time()
	time(&tt);
	//Using localtime()
	ti = localtime(&tt);

	stringstream ss;
	ss << setw(4) << ti->tm_year + 1900 << setfill('0') << setw(2) << ti->tm_mon + 1 << setfill('0') << setw(2) << ti->tm_mday << setfill('0') << setw(2) << ti->tm_hour << setfill('0') << setw(2) << ti->tm_min << setfill('0') << setw(2) << ti->tm_sec << "\0";

	ret = ss.str();
}
int ReadStr(string& str) {
	string tmp;
	cin >> tmp;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return 0;
	}
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	str = tmp;
	return 1;
}
int ReadRecordFromKB(string& fName, string& eventName)
{
	TimeNow(fName); // Auto File Naming
	if (!ReadStr(eventName)) return 0;
	return 1;
}
int WriteRecordToFile(ofstream& outFile, string fName, string eventName)
{
	outFile << fName << " " << eventName <<endl;
	if (outFile.bad())return 0;
	return 1;
}
int ReadRecordFromFile(ifstream& inFile, string& fName, string& eventName)
{
	if(!(inFile >> fName >> eventName))return 0; //infile.eof()쓰면 한번 더 입력?;;
	else { return 1; }
}
void WriteRecoredToScreen(string fName, string eventName)
{
	cout << fName << " , " << eventName << endl;
}
int AddRecordToList(vector<string> &pName, vector<string> &eName)
{
	string fName, eventName;
	if (!ReadRecordFromKB(fName, eventName))	return 0;
	
	pName.push_back(fName);
	eName.push_back(eventName);
	return 1;
	//사진 파일 레코드를 받아서 vector list에 저장
}
int WriteAllToFile(vector<string> &nameList, vector<string> &eventList)
{
	ofstream outFile("photo.dat");

	for (unsigned i = 0; i < nameList.size(); i++) {
		if (!WriteRecordToFile(outFile, nameList[i], eventList[i])) {
			outFile.close();
			return 0;
		}
	}
	outFile.close();
	return 1;
}
int ReadAllFromFile(vector<string> &nameList, vector<string> &eventList)
{
	ifstream inFile("photo.dat");
	string fName, eName;
	
	nameList.clear();
	eventList.clear();
	cout << "namesize" << nameList.size() << endl;
	cout << "eventsize" << eventList.size() << endl;
	while (1) {
		if (!ReadRecordFromFile(inFile, fName, eName)) {
			inFile.close();
			cout << "after read size" << nameList.size() << endl;
			return 0;
		}
		nameList.push_back(fName);
		eventList.push_back(eName);
	}
}
int SearchAllFromFile(vector<string> v, string inName)
{
	//index를 return , 못찾으면 -1을 return
	for (unsigned i = 0; i < v.size(); i++) {
		if (v[i] == inName)	return i;
	}
	return -1;
}
int DisplayAllOnScreen(vector<string> &nameList, vector<string> &eventList)
{
	for (unsigned i = 0; i < nameList.size(); i++) {
		cout << nameList[i] << ", " << eventList[i] << endl;
	}
	return 1;
}
void FindAndDisplay(vector<string> &nameList, vector<string> &eventList)
{
	string sf;
	cout << "TYPE FILE NAME : ";
	cin >> sf;

	int id = SearchAllFromFile(nameList, sf);
	if (id != -1) {
		cout << eventList[SearchAllFromFile(nameList, sf)] << endl;
	}
}
void DeleteByName(vector<string> &nameList, vector<string> &eventList)
{
	string sd;
	cout << "TYPE FILE NAME : ";
	cin >> sd;

	int id = SearchAllFromFile(nameList, sd);
	if (id != -1) {
		nameList.erase(nameList.begin() + id);
		eventList.erase(eventList.begin() + id);
	}
}
int GetCommand()
{
	int temp;
	while (1) {
		cout << endl << endl;
		cout << "\t---- ID - Command ------" << endl;
		cout << "\t     1. Add a new record to list" << endl;
		cout << "\t     2. Read all records from disk" << endl;
		cout << "\t     3. Write all records to disk" << endl;
		cout << "\t     4. Display all record on screen" << endl;
		cout << "\t     5. Search a record by file name and display on screen" << endl;
		cout << "\t     6. Search a record by file name and delete it from list" << endl;
		cout << "\t     0. Quit" << endl << endl;
		cout << "\t Choose a Command-->";
		if (cin.fail() || !(cin >> temp) || temp < 0 || temp > 6)
		{
			cout << "\t    ### Invalid Command Number. Select again ###" << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
		else
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			return temp;
		}
	}
}