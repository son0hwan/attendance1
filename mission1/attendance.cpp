#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Node {
	string name;
	string day;
};
const int NORMAL_GRADE = 0;
const int GOLD_GRADE = 1;
const int SILVER_GRADE = 2;
const int GOLD_GRADE_POINT = 50;
const int SILVER_GRADE_POINT = 30;
const int TRAINING_ADD_POINT = 3;
const int WEEKEND_ADD_POINT = 2;
const int TRAINING_FINAL_ADD_POINT = 10;
const int WEEKEND_FINAL_ADD_POINT = 10;
const int NUM_OF_DAY = 7;
const int MAX_PLAYER = 100;
const int MAX_INPUT_DATA = 500;
const string dayStrArray[NUM_OF_DAY] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

map<string, int> idList;
int id_cnt = 0;

int idDayData[MAX_PLAYER][MAX_PLAYER];
int points[MAX_PLAYER];
int grade[MAX_PLAYER];
string names[MAX_PLAYER];

int trainingDayCnt[MAX_PLAYER];
int weekendCnt[MAX_PLAYER];

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
		addPoint = TRAINING_ADD_POINT;
		trainingDayCnt[id] += 1;
	}
	else if (isWeekend(day)) {
		addPoint = WEEKEND_ADD_POINT;
		weekendCnt[id] += 1;
	}
	else 
		addPoint = 1;

	return addPoint;
}

int registerAndGetID(string name) {
	int curIDCnt = id_cnt + 1;
	if (curIDCnt > MAX_PLAYER)
		return 0;

	if (idList.count(name) == 0) {
		idList.insert({ name, ++id_cnt });

		names[id_cnt] = name;
	}
	return idList[name];
}

bool isInvalidID(int id) {
	return id == 0;
}

bool isInvalidDay(int dayIndex) {
	return dayIndex < 0 || dayIndex >= NUM_OF_DAY;
}

bool attendTrainingDayOver10(int id) {
	return idDayData[id][convertDayToIndex("wednesday")] >= 10;
}

bool attendWeekendOver10(int id) {
	return idDayData[id][convertDayToIndex("saturday")] + idDayData[id][convertDayToIndex("sunday")] >= 10;
}


int calculateFinalAddPoint(int id) {
	int finalAddPoint = 0;
	if (attendTrainingDayOver10(id))
		finalAddPoint += TRAINING_FINAL_ADD_POINT;

	if (attendWeekendOver10(id))
		finalAddPoint += WEEKEND_FINAL_ADD_POINT;
	
	return finalAddPoint;
}

int calculateGrade(int totalPoint) {
	if (totalPoint >= GOLD_GRADE_POINT)
		return GOLD_GRADE;
	if (totalPoint >= SILVER_GRADE_POINT)
		return SILVER_GRADE;

	return NORMAL_GRADE;
}

void printPlayerResult(int id)
{
	cout << "NAME : " << names[id] << ", ";
	cout << "POINT : " << points[id] << ", ";
	cout << "GRADE : ";

	if (grade[id] == GOLD_GRADE) {
		cout << "GOLD" << "\n";
	}
	else if (grade[id] == SILVER_GRADE) {
		cout << "SILVER" << "\n";
	}
	else {
		cout << "NORMAL" << "\n";
	}
}

bool shouldRemoved(int id) {
	return grade[id] != 1 && grade[id] != 2 && trainingDayCnt[id] == 0 && weekendCnt[id] == 0;
}

void printRemovedPlayer(void) {
	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int id = 1; id <= id_cnt; id++) {

		if (shouldRemoved(id)) {
			std::cout << names[id] << "\n";
		}
	}
}

void calculatePlayerPoint(string name, string day) {
	int id;
	int add_point = 0;
	int dayIndex;

	id = registerAndGetID(name);
	dayIndex = convertDayToIndex(day);

	if (isInvalidID(id))
		return;

	if (isInvalidDay(dayIndex))
		return;

	add_point = getAddPoint(id, day);

	idDayData[id][dayIndex] += 1;
	points[id] += add_point;
}

void input() {
	ifstream fin{ "attendance_weekday_500.txt" };
	for (int i = 0; i < MAX_INPUT_DATA; i++) {
		string name, day;
		fin >> name >> day;
		calculatePlayerPoint(name, day);
	}

	for (int id = 1; id <= id_cnt; id++) {
		points[id] += calculateFinalAddPoint(id);
		grade[id] = calculateGrade(points[id]);

		printPlayerResult(id);
	}

	printRemovedPlayer();
}

int main() {
	input();
}