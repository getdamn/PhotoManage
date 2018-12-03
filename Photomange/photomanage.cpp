#include "photomanage.hpp"

int ReadStr(string& str);//Read a string
int ReadInt(int& num);
void TimeNow(string &ret);//Return Time by string parameter

int RecordType::ReadRecordFromKB() {
	cout << "\t ********* [ Add a Record to list ] *********" << endl;
	//if any function occurs errors, return 0
	if (ReadPhotoNameFromKB() && ReadEventNameFromKB() && ReadSizeFromKB() && ReadContentsFromKB())
		return 1;
	else
		return 0;
}
void RecordType::GenFileName(string &name) {
	//generate File name TimeNow
	TimeNow(name);
}
int RecordType::ReadPhotoNameFromKB()
{
	cout << "\t Photo Name(Enter 'gen' to generate) \t\t\t--> ";
	if (!ReadStr(this->photoName)) { return 0; }
	if(photoName == "gen")
		GenFileName(this->photoName);
	return 1;
}
int RecordType::ReadEventNameFromKB() {
	cout << "\t Event Name \t\t\t--> ";
	if (ReadStr(this->eventName)) { return 1; }
	return 0;
}
int RecordType::ReadSizeFromKB() {
	cout << "\t Size \t\t\t\t--> ";
	if (ReadInt(this->size)) { return 1; }
	return 0;
}
int RecordType::ReadContentsFromKB() {
	cout << "\t Contents \t\t\t--> ";
	getline(cin, this->contents);
	if (cin.bad())
		return 0;
	return 1;
}
int RecordType::ReadRecordFromFile(ifstream &inFile) {
	if (inFile >> this->photoName >> this->eventName >> this->size)
	{
		getline(inFile, this->contents);
		return 1;
	}
	return 0;
}
int RecordType::WriteRecordToFile(ofstream &outFile) {
	if (outFile << this->photoName << " " << this->eventName << " " << this->size << " " << this->contents << endl)
		return 1;
	return 0;
}
void RecordType::DisplayRecord() {
	cout << setw(20) << this->photoName << ", " << this->eventName << ", " << this->size << ", " << this->contents << endl;
}
string RecordType::GetPhotoName() {
	return this->photoName;
}
string RecordType::GetEventName(){
	return eventName;
}
string RecordType::GetContents(){
	return contents;
}
bool RecordType::IsEqual(string name){
	if (this->photoName == name) { return true; }
	return false;
}
bool RecordType::IsEqualEvent(string name)
{
	if (this->eventName == name) { return true; }
	return false;
}
bool RecordType::IsContentsIncluded(string name)
{
	size_t found;
	size_t flag = 0;
	while ((found = this->contents.find(name, flag)) != string::npos) 
	{
		if (found == 0) 
		{
			if (found + name.size() == this->contents.size())
				return true;
			else if (this->contents[found + name.size()] == ' ')
				return true;
			else
			{
				flag = found + name.size();
				continue;
			}
		}
		else {
			if (this->contents[found - 1] == ' ') {
				if (found + name.size() == this->contents.size())
					return true;

				else
				{
					if (this->contents[found + name.size()] == ' ')
						return true;
					else
					{
						flag = found + name.size();
						continue;
					}
				}
			}
			else
			{
				flag = found + name.size();
				continue;
			}
		}
	}
	return false;
}
int RecordType::Compare(string nameKey)
{
	if(this->photoName == nameKey)
		return 0;

	int i = 0;
	while (1)
	{
		if ((i == this->photoName.size()) || (i == nameKey.size()))
			break;
		if (nameKey[i] > this->photoName[i])
			return 1;
		if (nameKey[i] < this->photoName[i])
			return -1;
		i++;
	}
	if (nameKey.size() > this->photoName.size())
		return 1;
	else
		return -1;
}
bool RecordType::operator<(RecordType inItem) 
{
	int i = 0;
	while (1)
	{
		if ((i == this->photoName.size()) || (i == inItem.photoName.size()))
			break;
		if (inItem.photoName[i] > this->photoName[i])
			return true;
		if (inItem.photoName[i] < this->photoName[i])
			return false;
		i++;
	}
	if (inItem.photoName.size() > this->photoName.size())
		return true;
	else
		return false;
}
bool RecordType::operator>(RecordType inItem)
{
	int i = 0;
	while (1)
	{
		if ((i == this->photoName.size()) || (i == inItem.photoName.size()))
			break;
		if (inItem.photoName[i] < this->photoName[i])
			return true;
		if (inItem.photoName[i] > this->photoName[i])
			return false;
		i++;
	}
	if (inItem.photoName.size() < this->photoName.size())
		return true;
	else
		return false;
}

int ApplicationType::AddRecordTolist() {
	RecordType record;//record to push back
	eventType EventRec;
	if (!record.ReadRecordFromKB()) { return 0; }

	for (RecordType ele : photoList)//check is primary
	{
		if (!ele.Compare(record.GetPhotoName()))
			return 0;
	}

	cout << "\t Input <= ";
	record.DisplayRecord();
	
	//EventList에 추가
	EventRec.SetEventName(record.GetEventName());
	EventRec.AddFileName(record.GetPhotoName());
	AddEventToList(EventRec);

	unsigned i = 0;
	for (i; i < photoList.size(); i++)//check is primary
	{
		if (photoList[i] > record)
			break;
	}
	photoList.insert(photoList.begin() + i, record);
	return 1;
}
int ApplicationType::ReadAllFromFile() {
	cout << "\t ********* [ Read from disk ] *********" << endl;
	photoList.clear();
	RecordType record;
	

	ifstream inFile(this->inFileName);
	while (record.ReadRecordFromFile(inFile)) {
		photoList.push_back(record);
		//record를 추가할때 event도 추가함.
		eventType EventRec;
		EventRec.SetEventName(record.GetEventName());
		EventRec.AddFileName(record.GetPhotoName());
		AddEventToList(EventRec);
	}
	inFile.close();
	return 1;
}
void ApplicationType::DisplayAllOnScreen() {
	cout << "\t ********* [ Records in the List ] *********" << endl;
	for (unsigned i = 0; i < photoList.size(); i++) {
		cout << "\t Record " << i << "\t: ";
		photoList[i].DisplayRecord();
		cout << endl;
	}
}
int ApplicationType::WriteAllToFile() {
	cout << "\t ********* [ Write to disk ] *********" << endl;
	ofstream outFile(outFileName);
	for (unsigned i = 0; i < photoList.size(); i++) {
		photoList[i].WriteRecordToFile(outFile);
	}
	outFile.close();
	return 1;
}
int ApplicationType::GetCommand() {
	int temp;
	while (1) {
		cout << endl << endl;
		cout << "\t---- ID - Command ------" << endl;
		cout << "\t     1. Add a new record to list" << endl;
		cout << "\t     2. Read all records from disk" << endl;
		cout << "\t     3. Write all records to disk" << endl;
		cout << "\t     4. Display all record on screen" << endl;
		cout << "\t     5. Retrieve by photo name" << endl;
		cout << "\t     6. Delete by photo name" << endl;
		cout << "\t     7. Retrieve by event" << endl;
		cout << "\t     8. Retrieve by contents" << endl;
		cout << "\t     9. Display Event List" << endl;
		cout << "\t     10. Retrieve from event list" << endl;
		cout << "\t     0. Quit" << endl << endl;
		cout << "\t Choose a Command-->";
		if (cin.fail() || !(cin >> temp) || temp < 0 || temp > 10)
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
int ApplicationType::Run() {
	//출력을 위한 객체 outFile을 선언하고 파일 "photo.dat"을 오픈
	int command;

	this->ReadAllFromFile();
	this->DisplayAllOnScreen();
	while ((command = this->GetCommand())) {
		switch (command) {
		case 0:
			return 1;
		case 1:
			this->AddRecordTolist();
			break;
		case 2:
			this->ReadAllFromFile();
			break;
		case 3:
			this->WriteAllToFile();
			break;
		case 4:
			this->DisplayAllOnScreen();
			break;
		case 5:
			this->RetrieveByPhotoName();
			break;
		case 6:
			this->DeleteByPhotoName();
			break;
		case 7:
			this->RetrieveByEvent();
			break;
		case 8:
			this->RetrieveByContents();
			break;
		case 9:
			this->DisplayEventList();
			break;
		case 10:
			this->RetrieveFromEventList();
			break;
		default:
			cout << "Invalid Operation \n";
			break;
		}
	}
	return 0;
}
int ApplicationType::SearchByPrimaryKey(const string &inName) {
	for (unsigned i = 0; i < photoList.size(); i++) {
		if (photoList[i].GetPhotoName() == inName) { return i; }
	}
	return -1;
}
int ApplicationType::RetrieveByPhotoName()
{
	string sf;
	cout << "\t ********* [ Find a record in the List ] *********" << endl;
	cout << "\t TYPE FILE NAME : ";
	cin >> sf;

	int id = BinarySearchByPrimaryKey(sf);
	if (id != -1) {
		cout << "\t Record :";
		photoList[id].DisplayRecord();
		return 1;
	}
	else {
		cout << "\t No Record" << endl;
		return 0;
	}
}
int ApplicationType::DeleteByPhotoName()
{
	string sd;
	cout << "\t ********* [ Delete a record by name ] *********" << endl;
	cout << "\t TYPE FILE NAME : ";
	cin >> sd;

	int id = BinarySearchByPrimaryKey(sd);
	if (id != -1) {
		photoList.erase(photoList.begin() + id);
	}
	else
		cout << "No Record" << endl;
	return 0;
}
int ApplicationType::RetrieveByEvent()
{
	string sf;
	bool CHK_NOREC = true;
	cout << "\t ********* [ Find a record in the List ] *********" << endl;
	cout << "\t TYPE EVENT NAME : ";
	cin >> sf;

	for (unsigned i = 0; i < photoList.size(); i++)
	{
		if (photoList[i].GetEventName() == sf)
		{
			cout << "\t Record " << i+1 << "\t: ";
			photoList[i].DisplayRecord();
			CHK_NOREC = false;
		}
	}
	if(CHK_NOREC) 
	{
		cout << "\t No Record" << endl;
		return 0;
	}
	return 1;
}
int ApplicationType::RetrieveByContents()
{
	string sf;
	bool CHK_NOREC = true;//var to check no record
	cout << "\t ********* [ Find a record in the List ] *********" << endl;
	cout << "\t TYPE CONTENTS NAME : ";
	getline(cin, sf);

	if (cin.bad())
		return 0;

	for (unsigned i = 0; i < photoList.size(); i++)
	{
		if (photoList[i].IsContentsIncluded(sf))
		{
			cout << "\t Record : ";
			photoList[i].DisplayRecord();
			CHK_NOREC = false;
		}
	}
	if (CHK_NOREC)
	{
		cout << "\t No Record" << endl;
		return 0;
	}
	return 1;
}
int ApplicationType::BinarySearchByPrimaryKey(string inName)
{
	int st = 0, end = photoList.size() - 1;
	int mid = (st + end) / 2;
	while (1)
	{
		mid = (st + end) / 2;
		if (photoList[mid].Compare(inName) == 0)
			return mid;
		if (st == end)
			break;
		else if (photoList[mid].Compare(inName) == -1)
			end = mid;
		else
			st = mid + 1;
	}
	return -1;
}
int ApplicationType::AddEventToList(eventType &eve)
{
	//event를 받고 알아서 event리스트 대입
	//parameter로 event를 (동적으로) 받고 알아서 대입
	//출력은 event가 존재 하면 1
	//존재하지 않으면 0출력
	//외부 에서 AddAll이 필요하다면
	//looop 만 써서 넣어주면 ㄱㄴ.
	int idx = SearchEventList(eve.GetEventName());
	vector<string> evePhotoList = eve.GetFileNameList();
	if (idx != -1)
	{
		for (vector<string>::size_type i = 0; i < evePhotoList.size(); i++)
		{
			eventList[SearchEventList(eve.GetEventName())].AddFileName(evePhotoList[i]);
		}
		return 1;
	}
	eventList.push_back(eve);
	return 0;
}

int ApplicationType::SearchEventList(const string &en)
{
	for (vector<string>::size_type i = 0; i < eventList.size(); i++)
	{
		if (en == eventList[i].GetEventName())
			return i;
	}
	return -1;
}

void ApplicationType::DisplayEventList()
{
	cout << "\t ********* [ Display Event List ] *********" << endl;
	vector<string> eveFileList;
	
	for (unsigned i = 0; i < eventList.size(); i++) {
		cout << "\t Event " << i << "\t: ";
		cout << eventList[i].GetEventName();
		cout << endl;

		cout << "\t Members : ";
		eveFileList = eventList[i].GetFileNameList();
		for (vector<string>::size_type j = 0; j < eveFileList.size(); j++)
		{
			cout << eveFileList[j] << " ,";
		}
		cout << endl;
	}
}

int ApplicationType::RetrieveFromEventList()
{
	string en;

	cout << "\t ********* [ Retrieve From Event List ] *********" << endl;
	cout << "\t TYPE EVENT NAME : ";
	if (!ReadStr(en)) { return 0; }
	//인덱스를 받는다.
	int idx = SearchEventList(en);
	//event가 존재하지 않을경우
	if (idx == -1) {
		cout << "\t No Data" << endl;
		return 0; 
	}
	//event가 존재하는 경우
	vector<string> evePhotoList = eventList[idx].GetFileNameList();
	for (unsigned i = 0; i < evePhotoList.size(); i++) {
		cout << "\t Record " << i << "\t: ";
		//binary search로 인덱스를 찾은다음 인덱스로 레코드파일을 찾는다.
		photoList[BinarySearchByPrimaryKey(evePhotoList[i])].DisplayRecord();
		cout << endl;
	}
	return 1;
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
int ReadInt(int &num)
{
	int tmp;
	cin >> tmp;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return 0;
	}
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	num = tmp;

	return 1;
}

string eventType::GetEventName()
{
	return eventName;
}

vector<string> eventType::GetFileNameList()
{
	vector<string> FileName = memPhotoList;
	return FileName;
}

void eventType::AddFileName(const string &photoName)
{
	memPhotoList.push_back(photoName);
	numOfphoto++;
}

void eventType::SetEventName(string en)
{
	eventName = en;
}

