#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Node {
	string w;
	string wk;
};
const int NUM_OF_DAY = 7;
const string dayStrArray[NUM_OF_DAY] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};


map<string, int> idList;
int id_cnt = 0;

//dat[사용자ID][요일]
int dat[100][100];
int points[100];
int grade[100];
string names[100];

int wed[100];
int weeken[100];

int convertDayToIndex(string day) {
	int idx = 0;
	for (idx = 0; idx < NUM_OF_DAY; idx++) {
		if (day == dayStrArray[idx])
			break;
	}

	return idx;
}

bool isTrainingDay(string day) {
	if (day == "wednesday")
		return true;

	return false;
}

bool isWeekend(string day) {
	if (day == "saturday")
		return true;

	if (day == "sunday")
		return true;

	return false;
}

int getAddPoint(int id, string day) {
	int addPoint;
	if (isTrainingDay(day)) {
		addPoint = 3;
		wed[id] += 1;
	}
	else if (isWeekend(day)) {
		addPoint = 2;
		weeken[id] += 1;
	}
	else 
		addPoint = 1;

	return addPoint;
}

void input2(string name, string day) {
	//ID 부여
	if (idList.count(name) == 0) {
		idList.insert({ name, ++id_cnt });

		names[id_cnt] = name;
	}
	int id = idList[name];

	int add_point = 0;
	int dayIndex = convertDayToIndex(day);
	add_point = getAddPoint(id, day);

	//사용자ID별 요일 데이터에 1씩 증가
	dat[id][dayIndex] += 1;
	points[id] += add_point;
}

void input() {
	ifstream fin{ "attendance_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		string t1, t2;
		fin >> t1 >> t2;
		input2(t1, t2);
	}

	for (int i = 1; i <= id_cnt; i++) {
		if (dat[i][2] > 9) {
			points[i] += 10;
		}

		if (dat[i][5] + dat[i][6] > 9) {
			points[i] += 10;
		}

		if (points[i] >= 50) {
			grade[i] = 1;
		}
		else if (points[i] >= 30) {
			grade[i] = 2;
		}
		else {
			grade[i] = 0;
		}

		cout << "NAME : " << names[i] << ", ";
		cout << "POINT : " << points[i] << ", ";
		cout << "GRADE : ";

		if (grade[i] == 1) {
			cout << "GOLD" << "\n";
		}
		else if (grade[i] == 2) {
			cout << "SILVER" << "\n";
		}
		else {
			cout << "NORMAL" << "\n";
		}
	}

	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int i = 1; i <= id_cnt; i++) {

		if (grade[i] != 1 && grade[i] != 2 && wed[i] == 0 && weeken[i] == 0) {
			std::cout << names[i] << "\n";
		}
	}
}

int main() {
	input();
}