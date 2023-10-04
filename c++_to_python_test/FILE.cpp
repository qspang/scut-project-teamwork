#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
#include<iomanip>
#include<cmath>

using namespace std;
#define buffersize 80//读取文件时的临时缓冲区
#define linesize 36//用于文件指针移动，不可改动。
#define IDlength 400
#define PI 3.14159
#define maxDistanceLimit 0.0225
#define roadMax 13935

extern "C" struct singlePoint
{
	string time = "";
	float longitude = 0;


	float latitude = 0;
	inline singlePoint() {}
	inline singlePoint(string time, float longitude, float latitude) {
		this->time = time;
		this->longitude = longitude;
		this->latitude = latitude;
	}
};

extern "C" struct record
{
	int ID = 0;
	int length = 0;
	singlePoint* pos = nullptr;
	inline record() {}
	inline record(int ID, int length) {
		this->ID = ID;
		this->length = length;
		pos = new singlePoint[length];
	}
	inline ~record() {
		//delete[]pos;
	}
};

extern "C" struct point
{
	float longitude = 0;
	float latitude = 0;
	inline point() {}
	inline point(float longitude, float latitude) {
		this->longitude = longitude;
		this->latitude = latitude;
	}
};

//static string filename = "C:\\Users\\hubia\\Desktop\\TXT\\1-200.txt";
extern "C" extern record* roadDataSet;


extern "C" extern record *IDpoint;


extern "C" void initIDpoint(record* IDpoint, string filename, int &PID, bool isCarRecord = true);//IDpoint固定输入，

//int getIDlength(char* ID, string filename);
//fstream goFirstLineByID(char* ID, string filename);
extern "C" void READ_FILE();


extern "C" void destroyArray(record*&arr);
extern "C" void clearArray(record *&arr);

//#endif
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//---------------------以上为FILE.h



//#pragma once

extern "C" struct retangle
{
	point upleft;
	point downright;
	inline retangle() {}
	inline retangle(point upleft, point downright) {
		this->upleft = upleft;
		this->downright = downright;
	}
};

extern "C" string getTime();
//string getTimeEnd();


extern "C" float carDensity(record *IDP, string start, string end, retangle retangleOne, retangle retangleTwo);

extern "C" bool isInRetangle(singlePoint pos, retangle ret);
extern "C" float getPeriod(string start, string end);//单位为小时,


extern "C" record* roadDataSet = new record[roadMax];


extern "C" record *IDpoint = new record[IDlength];

///////////////////////////////////////////////////////////////
////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////////////////

//#pragma once
//#include"problemF5.h"


extern "C" float carDensity(record *IDP, string start, string end, retangle retangleOne);
///////////////////////////////////////////////////////
///////////////////////////////////////////////


//#pragma once
//#include"FILE.h"
#include<vector>
//#define roadMax 13935

//static record* roadDataSet = new record[roadMax];


extern "C" int oneCarFrequenceInOneRoad(record, record);//返回某ID车在某ID道路上行驶的总次数。

extern "C" bool isTwoPointSame(point, point);

extern "C" float FIND_DISTANCE(float long1, float lat1, float long2, float lat2);

//将道路的原始数据写入数组之中。
extern "C" void initRoadRecord(string filename, record *roadDataSet);

//将道路的频度按照顺序写入一个TXT文档中。
extern "C" void writeRoadFrequenceToTxt(record *IDpoint, record *roadDataSet, string filename);

//将道路频度TXT文档中数据写入道路数组之中。
extern "C" void roadFrequenceArrayInit(string filename, int *roadFrequenceArray);

//返回符合条件的道路和其对应的频繁度的集合。
extern "C" vector<pair<record, int>> statisticsTopKRoadsWithLengthMoreThanX(record *roadDataSet, int*roadFrequenceArray, int k, int x);


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//////////////////////////////////////////////////

//#pragma
//#include"problemF7.h"
//#include"problemF5.h"


extern "C" vector<pair<record, int>> statisticsTopKRoadsFromRetangleAtoRetangleB(int k, record*roadDataSet, int *roadFrequenceArray, retangle A, retangle B);

extern "C" bool isPointInRetangle(point p, retangle r);

extern "C" bool isRoadConnectedtoRetangle(record road, retangle retan);


///////////////////////////////////////////////
////////////////////////////////////
//////////////////////////////////////////

/////////////////////////////////////
//////////////////////////////////////////////
///////////////////////////


void initIDpoint(record* IDpoint, string filename, int &pID, bool isCarRecord)
{
	fstream file(filename, ios::in);
	if (!file) {
		cout << "error: open file" << endl;
		abort();
	}
	file.seekg(0, ios::beg);
	char buffer[buffersize] = { '\0' };
	//int pID = 0;
	while (file) {
		memset(buffer, '\0', buffersize);
		file.read(buffer, sizeof(char) * 5);
		int idcode = atoi(buffer);
		//file.read(buffer, sizeof(char));
		char tem;
		file.read(&tem, sizeof(char));//去除ID和长度之间空格，后面指针要多减一。
		int counterBit = 0;
		file.read(&tem, sizeof(char));
		counterBit--;
		while (tem != '*') {//确定长度有几位。
			file.read(&tem, sizeof(char));
			counterBit--;
		}
		file.seekg(counterBit, ios::cur);
		memset(buffer, '\0', buffersize);
		file.read(buffer, sizeof(char)*(-1 - counterBit));
		int length = atoi(buffer);
		file.seekg(linesize + counterBit - 1, ios::cur);//将指针移动到下一行开头，
		IDpoint[pID] = record(idcode, length);
		for (int i = 0; i < length; ++i) {
			file.seekg(7, ios::cur);//指针右移6，
			memset(buffer, '\0', buffersize);
			file.read(buffer, sizeof(char) * 14);//指针右移14，
			if (isCarRecord == true) {
				IDpoint[pID].pos[i].time = buffer;
			}
			file.seekg(1, ios::cur);//指针右移1，
			memset(buffer, '\0', buffersize);
			file.read(buffer, sizeof(char) * 9);//指针右移9，
			IDpoint[pID].pos[i].longitude = strtof(buffer, nullptr);
			file.seekg(1, ios::cur);//指针右移1，
			memset(buffer, '\0', buffersize);
			file.read(buffer, sizeof(char) * 8);//指针右移8，
			IDpoint[pID].pos[i].latitude = strtof(buffer, nullptr);
			file.seekg(1, ios::cur);//指针右移2，移动到下一行准备读取第一个字符。
		}
		file.seekg(1, ios::cur);
		pID++;
		//cout << pID << endl;
	}
	file.close();
	return;
}


void destroyArray(record*&arr)
{
	int pos = 0;
	while (arr[pos].ID != 0) {
		delete[]arr[pos].pos;
		arr[pos].pos = nullptr;
		arr[pos].ID = 0;
		arr[pos].length = 0;
		pos++;
	}
	delete[]arr;
	arr = nullptr;
	return;
}

void clearArray(record *&arr)
{
	int pos = 0;
	while (arr[pos].ID != 0) {
		delete[]arr[pos].pos;
		arr[pos].pos = nullptr;
		arr[pos].ID = 0;
		arr[pos].length = 0;
		pos++;
	}
	return;
}

//void READ_FILE()
//{
//	int COUNT_INIT = 1, PID = 0;
//	string filename;
//	while (COUNT_INIT <= 200)
//	{
//
//		//int tem = PID;
//		filename = "C:\\Users\\hubia\\Desktop\\TXT\\" + to_string(COUNT_INIT) + "-" + to_string(COUNT_INIT + 199) + ".txt";
//		initIDpoint(IDpoint, filename, PID);
//		cout << filename << endl << endl;
//		COUNT_INIT += 200;
//		//PID = tem + PID;
//	}
//}





//int main() {
//	READ_FILE();
//	string start = "20080202153608";
//	string end = "20080208153608";
//	retangle retangleOne = retangle(point(115.51172, 40.40965), point(116.78660, 39.92123));
//	retangle retangleTwo = retangle(point(0, 90), point(180, 0));
//	string roadDatasetFile = "C:\\Users\\hubia\\Desktop\\bj_tt_end.txt";
//	initRoadRecord(roadDatasetFile, roadDataSet);
//
//
//	for (int ii = 0; ii < 20; ++ii) {
//		cout << roadDataSet[ii].ID << "	" << roadDataSet[ii].length << endl;
//		for (int i = 0; i < roadDataSet[ii].length; ++i) {
//			cout << roadDataSet[ii].pos[i].time << "  " << fixed << setprecision(5)
//				<< roadDataSet[ii].pos[i].longitude << "  " << fixed << setprecision(5)
//				<< roadDataSet[ii].pos[i].latitude << endl;
//		}
//		cout << endl;
//	}
//
//	for (int ii = 0; ii < 3; ++ii) {
//		cout << IDpoint[ii].ID << "	" << IDpoint[ii].length << endl;
//		for (int i = 0; i < IDpoint[ii].length; ++i) {
//			cout << IDpoint[ii].pos[i].time << "  " << fixed << setprecision(5)
//				<< IDpoint[ii].pos[i].longitude << "  " << fixed << setprecision(5)
//				<< IDpoint[ii].pos[i].latitude << endl;
//		}
//		cout << endl;
//	}
//	string frequenceTxtFilename = "C:\\Users\\hubia\\Desktop\\roadFrequence.txt";
//	//writeRoadFrequenceToTxt(IDpoint, roadDataSet, frequenceTxtFilename);
//
//}


string getTime()
{
	string start = "";
	cin >> start;
	string res = "";
	res += start.substr(0, 4);
	res += start.substr(5, 2);
	res += start.substr(8, 2);
	res += start.substr(11, 2);
	res += start.substr(14, 2);
	res += start.substr(17, 2);
	return res;
}


float getPeriod(string start, string end)
{
	//string res = "";
	int year = stoi(end.substr(0, 4)) - stoi(start.substr(0, 4));
	int month = stoi(end.substr(4, 2)) - stoi(start.substr(4, 2));
	int day = stoi(end.substr(6, 2)) - stoi(start.substr(6, 2));
	int hour = stoi(end.substr(8, 2)) - stoi(start.substr(8, 2));
	int minute = stoi(end.substr(10, 2)) - stoi(start.substr(10, 2));
	int second = stoi(end.substr(12, 2)) - stoi(start.substr(12, 2));
	int sum = year * 365 * 24 + month * 30 * 24 + day * 24
		+ hour + minute / 60. + second / 3600.;
	return sum;
}


bool isInRetangle(singlePoint pos, retangle ret)
{
	if (pos.longitude >= ret.upleft.longitude&&pos.longitude <= ret.downright.longitude&&
		pos.latitude >= ret.downright.latitude&&pos.latitude <= ret.upleft.latitude) {
		return true;
	}
	return false;
}



float carDensity(record *IDP, string start, string end, retangle retangleOne, retangle retangleTwo)
{
	int pos = 0;
	int counter = 0;
	while (IDP[pos].ID != 0) {
		int last = 0;//1表示本次或最近一次在矩形区域1之中，当两者不相等时计数加一。
		int cur = 0;
		for (int i = 0; i < IDP[pos].length; ++i) {
			if (IDP[pos].pos[i].time >= start && IDP[pos].pos[i].time <= end) {
				if (isInRetangle(IDP[pos].pos[i], retangleOne)) {
					last = cur;
					cur = 1;
					if (last * cur == 2) {
						counter++;
					}
				}
				if (isInRetangle(IDP[pos].pos[i], retangleTwo)) {
					last = cur;
					cur = 2;
					if (last * cur == 2) {
						counter++;
					}
				}
			}

		}
		pos++;
	}
	float period = getPeriod(start, end);
	return counter / period;
}





float carDensity(record *IDP, string start, string end, retangle retangleOne)
{
	int pos = 0;
	int counter = 0;
	while (IDP[pos].ID != 0) {
		int last = 0;//1表示本次或最近一次在矩形区域1之中，当两者不相等时计数加一。
		int cur = 0;
		for (int i = 0; i < IDP[pos].length; ++i) {
			if (IDP[pos].pos[i].time >= start && IDP[pos].pos[i].time <= end) {
				if (isInRetangle(IDP[pos].pos[i], retangleOne)) {
					last = cur;
					cur = 1;
					if (last * cur == 2) {
						counter++;
					}
				}
				if (!isInRetangle(IDP[pos].pos[i], retangleOne)) {
					last = cur;
					cur = 2;
					if (last * cur == 2) {
						counter++;
					}
				}
			}

		}
		pos++;
	}
	float period = getPeriod(start, end);
	return counter / period;
}






float FIND_DISTANCE(float long1, float lat1, float long2, float lat2) {

	// 地球半径的平均值，

	//float R = 6371393;
	//lat1 = lat1 * PI / 180.0;
	//lat2 = lat2 * PI / 180.0;
	//float a = lat1 - lat2;
	//float b = (long1 - long2) * PI / 180.0;
	//float sa2 = sin(a / 2.0);
	//float sb2 = sin(b / 2.0);
	////cout << 2 * R * asin(sqrt(sa2 * sa2 + cos(lat1) * cos(lat2) * sb2 * sb2)) << endl;
	//return 2 * R * asin(sqrt(sa2 * sa2 + cos(lat1) * cos(lat2) * sb2 * sb2));
	return ((long1 - long2)*(long1 - long2) + (lat1 - lat2)*(lat1 - lat2)) * 1232100;
}


bool isTwoPointSame(singlePoint one, singlePoint two)
{
	if (FIND_DISTANCE(one.longitude, one.latitude, two.longitude, two.latitude) <= maxDistanceLimit)
		return true;
	return false;
}

int oneCarFrequenceInOneRoad(record car, record road)
{
	int test = 0;
	int counter = 0;
	int last = 0;//-1表示上个时态不在路径上，1则表示在。
	int cur = 0;
	for (int i = 0; i < car.length; ++i) {
		for (int ii = 0; ii < road.length; ++ii) {
			//cout << "第"<<ii<<"个循环" << endl;
			if (isTwoPointSame(road.pos[ii], car.pos[i])) {
				//cout << "进入循环" << ++test << endl;
				last = cur;
				cur = 1;
				if (cur == 1 && last != 1) {
					counter++;
					break;
				}
			}
			else {
				last = cur;
				cur = -1;
			}
			//return true;
		}
	}
	//return false;
	return counter;
}

void initRoadRecord(string filename, record *roadDataSet)
{
	int pos = 0;
	initIDpoint(roadDataSet, filename, pos, false);
	return;
}

void writeRoadFrequenceToTxt(record *carDataSet, record *roadDataSet, string filename)
{
	fstream file(filename, ios::out | ios::app);
	if (!file) {
		cout << "no such file!" << endl;
		//abort();
	}

	int roadPos = 0;
	while (roadDataSet[roadPos].ID != 0) {
		int carPos = 0;
		int counter = 0;
		while (carDataSet[carPos].ID != 0) {
			//cout << "----------------------" << endl;
			counter += oneCarFrequenceInOneRoad(carDataSet[carPos], roadDataSet[roadPos]);
			carPos++;
		}
		file << counter << endl;
		roadPos++;
	}
	file.close();
	return;
}

void roadFrequenceArrayInit(string filename, int *roadFrequenceArray)
{
	fstream file(filename, ios::in);
	if (!file) {
		cout << "no such file!" << endl;
		//abort();
	}
	int buffer;
	int pos = 0;
	while (file >> buffer) {
		roadFrequenceArray[pos] = buffer;
		pos++;
	}
	file.close();
	return;
}

vector<pair<record, int>> statisticsTopKRoadsWithLengthMoreThanX(record *roadDataSet, int*roadFrequenceArray, int k, int x)
{
	int roadFrequenceOrder[roadMax];//将路径id根据其频繁度从大到小排序。其存储值为对应id路径的下标。
	for (int i = 0; i < roadMax; ++i) {
		int tem = i;
		for (int ii = i; ii < roadMax; ++ii) {
			if (roadFrequenceArray[tem] < roadFrequenceArray[ii]) {
				tem = ii;
			}
		}
		roadFrequenceOrder[i] = tem;
	}
	int counter = 0;
	vector<pair<record, int>> res;
	int pos = 0;
	while (roadDataSet[pos].ID != 0) {
		if (roadDataSet[pos].length >= x) {
			for (int i = 0; i < k; ++i) {
				if (roadDataSet[pos].ID == roadFrequenceOrder[i] + 1) {//下标和ID之间差1.
					res.push_back(make_pair(roadDataSet[pos], roadFrequenceArray[roadFrequenceOrder[i]]));
					counter++;
					break;
				}
			}
		}
		if (counter == k)break;
		pos++;
	}
	return res;
}





bool isPointInRetangle(point p, retangle r)
{
	if (p.longitude >= r.upleft.longitude&&p.longitude <= r.downright.longitude&&
		p.latitude >= r.downright.latitude&&p.latitude <= r.upleft.latitude) {
		return true;
	}
	return false;
}


bool isRoadConnectedtoRetangle(record road, retangle retan)
{
	for (int i = 0; i < road.length; ++i) {
		if (isInRetangle(road.pos[i], retan))
			return true;
	}
	return false;
}





vector<pair<record, int>> statisticsTopKRoadsFromRetangleAtoRetangleB(int k, record*roadDataSet, int *roadFrequenceArray, retangle A, retangle B)
{
	int roadFrequenceOrder[roadMax];//将路径id根据其频繁度从大到小排序。其存储值为对应id路径的下标。
	for (int i = 0; i < roadMax; ++i) {
		int tem = i;
		for (int ii = i; ii < roadMax; ++ii) {
			if (roadFrequenceArray[tem] < roadFrequenceArray[ii]) {
				tem = ii;
			}
		}
		roadFrequenceOrder[i] = tem;
	}
	int counter = 0;
	vector<pair<record, int>>res;
	int pos = 0;
	while (roadDataSet[pos].ID != 0) {
		if (isRoadConnectedtoRetangle(roadDataSet[pos], A) && isRoadConnectedtoRetangle(roadDataSet[pos], B)) {
			for (int i = 0; i < k; ++i) {
				if (roadDataSet[pos].ID == roadFrequenceOrder[i] + 1) {
					res.push_back(make_pair(roadDataSet[pos], roadFrequenceArray[roadFrequenceOrder[i]]));
					counter++;
					break;
				}
			}
		}
		if (counter == k)break;
		pos++;
	}
	return res;
}



//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

//#include"FILE.h"
#include"RTree.h"
//#include"problemF8.h"

typedef int IDvalue;

struct RtreeRoadPoint
{
	int ID = 0;
	float longitude = 0;
	float latitude = 0;
	inline RtreeRoadPoint() {}
	inline RtreeRoadPoint(int ID, float longitude, float latitude) {
		this->ID = ID;
		this->longitude = longitude;
		this->latitude = latitude;
	}
};

int frequenceArray[roadMax] = { 0 };



float getMinX(singlePoint one, singlePoint two)
{
	return one.longitude < two.longitude ? one.longitude : two.longitude;
}

float getMinY(singlePoint one, singlePoint two)
{
	return one.latitude < two.latitude ? one.latitude : two.latitude;
}

float getMaxX(singlePoint one, singlePoint two)
{
	return one.longitude > two.longitude ? one.longitude : two.longitude;
}

float getMaxY(singlePoint one, singlePoint two)
{
	return one.latitude > two.latitude ? one.latitude : two.latitude;
}



void initRoadRTree(RTree<pair<IDvalue, int>, float, 2, float>*tree, record*roadDataSet)
{
	int pos = 0;
	while (roadDataSet[pos].ID != 0) {
		for (int i = 0; i < roadDataSet[pos].length - 1; ++i) {
			float min[2] = { 0 };
			float max[2] = { 0 };
			min[0] = getMinX(roadDataSet[pos].pos[i], roadDataSet[pos].pos[i + 1]);
			min[1] = getMinY(roadDataSet[pos].pos[i], roadDataSet[pos].pos[i + 1]);
			max[0] = getMaxX(roadDataSet[pos].pos[i], roadDataSet[pos].pos[i + 1]);
			max[1] = getMaxY(roadDataSet[pos].pos[i], roadDataSet[pos].pos[i + 1]);

			tree->Insert(min, max, make_pair(roadDataSet[pos].ID, 0));
		}
		pos++;
	}
	return;
}


//static pair<IDvalue, int>*RtreeLeaf = nullptr;

bool MySearchCallback(const pair<IDvalue, int> &id)
{
	//cout << "Hit data rect " << id << "\n";
	//RtreeLeaf = &id;
	//cout << id.first << "   :" << id.second << endl;
	return false;
	//return true; // keep going
}

void getRoadFrequenceInRtree(record *carDataSet, RTree<pair<IDvalue, int>, float, 2, float>*tree)
{
	int pos = 0;
	pair<IDvalue, int> *leaf = nullptr;
	while (carDataSet[pos].ID != 0) {
		int lastCarPos = 0;
		int curCarPos = 0;
		//int lastRoad = 0;
		//int curRoad = 0;
		for (int i = 0; i < carDataSet[pos].length - 1; ++i) {
			float min[2] = { 0 };
			float max[2] = { 0 };
			min[0] = getMinX(carDataSet[pos].pos[i], carDataSet[pos].pos[i + 1]);
			min[1] = getMinY(carDataSet[pos].pos[i], carDataSet[pos].pos[i + 1]);
			max[0] = getMaxX(carDataSet[pos].pos[i], carDataSet[pos].pos[i + 1]);
			max[1] = getMaxY(carDataSet[pos].pos[i], carDataSet[pos].pos[i + 1]);
			leaf = nullptr;
			int res = tree->Search(min, max, MySearchCallback, leaf);
			/*if (leaf) {
				cout << leaf->first << "      :" << leaf->second << endl;
			}*/
			if (res >= 1) {
				lastCarPos = curCarPos;
				curCarPos = leaf->first;
				if (lastCarPos != curCarPos) {
					leaf->second++;
				}

			}
			else {
				lastCarPos = curCarPos;
				curCarPos = 0;
			}
		}
		pos++;
	}
	return;
}


void getFrequenceArray(RTree<pair<IDvalue, int>, float, 2, float>*tree, int *frequenceArray)
{
	RTree<pair<IDvalue, int>, float, 2, float>::Iterator it;
	for (tree->GetFirst(it); !tree->IsNull(it); tree->GetNext(it)) {
		frequenceArray[tree->GetAt(it).first] += tree->GetAt(it).second;
	}
	return;
}


void writeRoadFrequenceToTxt(int *frequenceArray, string filename)//frequenceArray传递失败。
{
	fstream file(filename, ios::out);
	if (!file) {
		cout << "no such file!" << endl;
		//abort();
	}
	for (int i = 0; i < roadMax; ++i) {
		file << frequenceArray[i] << endl;
	}
	file.close();
	return;
}
//record *IDpoint = new record[IDlength];

void READ_FILE()
{
	int COUNT_INIT = 1601, PID = 0;
	string filename;
	while (COUNT_INIT <= 2000)
	{

		//int tem = PID;
		filename = "C:\\Users\\hubia\\Desktop\\TXT\\" + to_string(COUNT_INIT) + "-" + to_string(COUNT_INIT + 199) + ".txt";
		initIDpoint(IDpoint, filename, PID);
		cout << filename << endl << endl;
		COUNT_INIT += 200;
		//PID = tem + PID;
	}
}

void READ_FILE(int COUNT_INIT, int MAX)
{
	int PID = 0;
	string filename;
	while (COUNT_INIT <= MAX)
	{

		//int tem = PID;
		filename = "C:\\Users\\hubia\\Desktop\\TXT\\" + to_string(COUNT_INIT) + "-" + to_string(COUNT_INIT + 199) + ".txt";
		initIDpoint(IDpoint, filename, PID);
		cout << filename << endl << endl;
		COUNT_INIT += 200;
		//PID = tem + PID;
	}
}




//int main() {
//	//READ_FILE();
//
//
//	//string start = "20080202153608";
//	//string end = "20080208153608";
//	//retangle retangleOne = retangle(point(115.51172, 40.40965), point(116.78660, 39.92123));
//	//retangle retangleTwo = retangle(point(0, 90), point(180, 0));
//
//
//	string roadDatasetFile = "C:\\Users\\hubia\\Desktop\\bj_tt_end.txt";
//	initRoadRecord(roadDatasetFile, roadDataSet);
//
//
//
//	//for (int ii = 0; ii < 100; ++ii) {
//	//	cout << roadDataSet[ii].ID << "	" << roadDataSet[ii].length << endl;
//	//	for (int i = 0; i < roadDataSet[ii].length; ++i) {
//	//		cout << roadDataSet[ii].pos[i].time << "  " << fixed << setprecision(5)
//	//			<< roadDataSet[ii].pos[i].longitude << "  " << fixed << setprecision(5)
//	//			<< roadDataSet[ii].pos[i].latitude << endl;
//	//	}
//	//	cout << endl;
//	//}
//
//
//	RTree<pair<IDvalue, int>, float, 2, float>tree;
//	initRoadRTree(&tree, roadDataSet);
//
//	int itIndex = 0;
//	RTree<pair<IDvalue, int>, float, 2, float>::Iterator it;
//	//for (tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it)) {
//	//	int value = tree.GetAt(it).first;
//	//	if (itIndex >= 100)break;
//	//	float boundsMin[2] = { 0,0 };
//	//	float boundsMax[2] = { 0,0 };
//	//	it.GetBounds(boundsMin, boundsMax);
//	//	//if(itIndex>=15800)
//	//	cout << "it[" << itIndex++ << "] " << value << " = (" << boundsMin[0] << "," << boundsMin[1]
//	//		<< "," << boundsMax[0] << "," << boundsMax[1] << ")\n";
//	//}
//
//	int COUNT_INIT = 1;
//	int MAX = 400;
//	static int tem[roadMax] = { 0 };
//	while (MAX <= 10400) {
//		clearArray(IDpoint);
//		READ_FILE(COUNT_INIT, MAX);
//		getRoadFrequenceInRtree(IDpoint, &tree);
//		getFrequenceArray(&tree, tem);
//		for (int i = 0; i < roadMax; ++i) {
//			frequenceArray[i] += tem[i];
//		}
//		COUNT_INIT += 400;
//		MAX += 400;
//	}
//
//	//getRoadFrequenceInRtree(IDpoint, &tree);
//	////static int frequenceArray[roadMax] = { 0 };
//	//getFrequenceArray(&tree, frequenceArray);
//
//
//	//for (int ii = 0; ii < 10; ++ii) {
//	//	cout << IDpoint[ii].ID << "	" << IDpoint[ii].length << endl;
//	//	for (int i = 0; i < IDpoint[ii].length; ++i) {
//	//		cout << IDpoint[ii].pos[i].time << "  " << fixed << setprecision(5)
//	//			<< IDpoint[ii].pos[i].longitude << "  " << fixed << setprecision(5)
//	//			<< IDpoint[ii].pos[i].latitude << endl;
//	//	}
//	//	cout << endl;
//	//}
//
//
//	string frequenceTxtFilename = "C:\\Users\\hubia\\Desktop\\roadFrequence.txt";
//	writeRoadFrequenceToTxt(frequenceArray, frequenceTxtFilename);
//	//fstream file(frequenceTxtFilename, ios::out);
//	//if (!file) {
//	//	cout << "no such file!" << endl;
//	//	//abort();
//	//}
//	//for (int i = 0; i < roadMax; ++i) {
//	//	file << frequenceArray[i] << endl;
//	//}
//	//file.close();
//
//	//destroyArray(IDpoint);
//	//destroyArray(roadDataSet);
//}


extern "C" void test()
{
	string roadDatasetFile = "C:\\Users\\hubia\\Desktop\\bj_tt_end.txt";
	initRoadRecord(roadDatasetFile, roadDataSet);

	RTree<pair<IDvalue, int>, float, 2, float>tree;
	initRoadRTree(&tree, roadDataSet);

	int itIndex = 0;
	RTree<pair<IDvalue, int>, float, 2, float>::Iterator it;


	int COUNT_INIT = 1;
	int MAX = 400;
	static int tem[roadMax] = { 0 };
	while (MAX <= 10400) {
		clearArray(IDpoint);
		READ_FILE(COUNT_INIT, MAX);
		getRoadFrequenceInRtree(IDpoint, &tree);
		getFrequenceArray(&tree, tem);
		for (int i = 0; i < roadMax; ++i) {
			frequenceArray[i] += tem[i];
		}
		COUNT_INIT += 400;
		MAX += 400;
	}

	string frequenceTxtFilename = "C:\\Users\\hubia\\Desktop\\roadFrequence.txt";
	writeRoadFrequenceToTxt(frequenceArray, frequenceTxtFilename);
}