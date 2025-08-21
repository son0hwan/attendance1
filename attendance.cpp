#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;
const int NUM_OF_DAY = 7;
const string dayStrArray[NUM_OF_DAY] = { "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday" };
const int NORMAL_GRADE = 0;
const int GOLD_GRADE = 1;
const int SILVER_GRADE = 2;
const int GOLD_GRADE_POINT = 50;
const int SILVER_GRADE_POINT = 30;

class Grade {
public:
	bool canGetGrade(int point) {
		return point >= gradePoint;
	}
	int getID() {
		return id;
	}
	virtual void print(void) = 0;
protected:
	int gradePoint = 0;
	int id = NORMAL_GRADE;
};

class GoldGrade : public Grade{
public:
	GoldGrade() {
		gradePoint = GOLD_GRADE_POINT;
		id = GOLD_GRADE;
	}
	void print(void) override {
		cout << "GOLD" << "\n";
	}
};

class SilverGrade : public Grade {
public:
	SilverGrade() {
		gradePoint = SILVER_GRADE_POINT;
		id = SILVER_GRADE;
	}
	void print(void) override {
		cout << "SILVER" << "\n";
	}
};

class NormalGrade : public Grade {
public:
	NormalGrade() {
		gradePoint = 0;
		id = NORMAL_GRADE;
	}
	void print(void) override {
		cout << "NORMAL" << "\n";
	}
};


class Player {
public:
	Player(string name, int id) : name(name), id(id) {
		point = 0;
		grade = NULL;
		trainingDayCnt = 0;
		weekendCnt = 0;
	}

	void addPoint(int point) {
		this->point += point;
	}
	void addTrainingDayCnt() {
		trainingDayCnt++;
	}
	void addWeekendCnt() {
		weekendCnt++;
	}
	void setGrade(Grade *grade) {
		this->grade = grade;
	}
	Grade* getGrade(void) {
		return grade;
	}
	int getPoint(void) {
		return point;
	}
	int getID(void) {
		return id;
	}
	int getTrainingDayCnt(void) {
		return trainingDayCnt;
	}
	int getWeekendCnt(void) {
		return weekendCnt;
	}
	const string& getName(void) {
		return name;
	}

	void print() {
		cout << "NAME : " << name << ", ";
		cout << "POINT : " << point << ", ";
		cout << "GRADE : ";

		grade->print();
	}
private:
	string name;
	int point;
	Grade* grade;
	int id;
	int trainingDayCnt;
	int weekendCnt;
};

class AttendenceManager {
public:
	AttendenceManager() {
		gradeList.push_back(new NormalGrade());
		gradeList.push_back(new SilverGrade());
		gradeList.push_back(new GoldGrade());
	}

	void input(string path) {
		ifstream fin{ path };
		for (int i = 0; i < MAX_INPUT_DATA; i++) {
			string name, day;
			fin >> name >> day;
			calculatePlayerPoint(name, day);
		}

		for (int id = 1; id <= id_cnt; id++) {
			Player* player = playerMapByID[id];
			
			player->addPoint(calculateFinalAddPoint(id));
			player->setGrade(calculateGrade(player->getPoint()));
			player->print();
		}

		printRemovedPlayer();
	}


	static const int TRAINING_ADD_POINT = 3;
	static const int WEEKEND_ADD_POINT = 2;
	static const int TRAINING_FINAL_ADD_POINT = 10;
	static const int WEEKEND_FINAL_ADD_POINT = 10;
	static const int MAX_PLAYER = 100;
	static const int MAX_INPUT_DATA = 500;
private:
	map<string, Player*> playerMapByString;
	map<int, Player*> playerMapByID;
	int id_cnt = 0;

	vector<Grade*> gradeList;
	int idDayData[MAX_PLAYER][MAX_PLAYER];


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

	int getAddPoint(Player *player, string day) {
		int addPoint;
		if (isTrainingDay(day)) {
			addPoint = TRAINING_ADD_POINT;
			player->addTrainingDayCnt();
		}
		else if (isWeekend(day)) {
			addPoint = WEEKEND_ADD_POINT;
			player->addWeekendCnt();
		}
		else
			addPoint = 1;

		return addPoint;
	}

	bool notRegistered(string name) {
		return playerMapByString.count(name) == 0;
	}

	void registerPlayer(string name) {
		int cur_id = ++id_cnt;
		Player* newPlayer = new Player(name, cur_id);
		playerMapByString.insert({ name, newPlayer });
		playerMapByID.insert({cur_id, newPlayer});
	}

	Player* registerAndGetPlayer(string name) {
		int curIDCnt = id_cnt + 1;
		if (curIDCnt >= MAX_PLAYER)
			return NULL;

		if (notRegistered(name)) {
			registerPlayer(name);
		}
		return playerMapByString[name];
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

	Grade *calculateGrade(int totalPoint) {
		Grade *finalGrade = NULL;
		for (Grade *grade : gradeList) {
			if (grade->canGetGrade(totalPoint))
				finalGrade = grade;
		}

		return finalGrade;
	}

	bool shouldRemoved(Player *player) {
		return player->getGrade()->getID() != 1 && player->getGrade()->getID() != 2 && player->getTrainingDayCnt() == 0 && player->getWeekendCnt() == 0;
	}

	void printRemovedPlayer(void) {
		std::cout << "\n";
		std::cout << "Removed player\n";
		std::cout << "==============\n";
		for (int id = 1; id <= id_cnt; id++) {
			Player* player = playerMapByID[id];
			if (shouldRemoved(player)) {
				std::cout << player->getName() << "\n";
			}
		}
	}

	void calculatePlayerPoint(string name, string day) {
		Player* player;
		int id;
		int add_point = 0;
		int dayIndex;

		player = registerAndGetPlayer(name);
		dayIndex = convertDayToIndex(day);
		if (player == NULL)
			return;

		id = player->getID();

		add_point = getAddPoint(player, day);

		idDayData[id][dayIndex] += 1;
		player->addPoint(add_point);
	}

};


