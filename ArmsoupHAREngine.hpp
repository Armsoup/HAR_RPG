// Copyright © Armsoup 2026
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <timeapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <filesystem>
#pragma comment(lib, "winmm.lib")

using namespace std;

namespace fs = filesystem;

// GLOBAL VARIABLES 
extern string PlayerName;
extern int PlayerFood;
extern int PlayerFoodInventory;
extern float PlayerX;
extern float PlayerY;
extern int GlobalEvent;
extern int KillMonsters;
extern int FoundFood;
extern int Distancecovered;
int Success = 0;
static HANDLE hBufferA = INVALID_HANDLE_VALUE;
static HANDLE hBufferB = INVALID_HANDLE_VALUE;
static int nBufferIndex = 0;

string trim(const string& s) {
	size_t start = s.find_first_not_of(" \t\r\n");
	if (start == string::npos) return "";
	size_t end = s.find_last_not_of(" \t\r\n");
	return s.substr(start, end - start + 1);
}

string getCurrentDateTime() {
	auto now = chrono::system_clock::now();
	time_t tt = chrono::system_clock::to_time_t(now);
	struct tm tm_info;
	localtime_s(&tm_info, &tt);
	char buf[64];
	strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &tm_info);
	return string(buf);
}

// File system functions for the game
bool fileExists(string_view path) {
	return fs::exists(path) && fs::is_regular_file(path);
}

bool dirExists(string_view path) {
	return fs::exists(path) && fs::is_directory(path);
}

void writeFile(string_view path, string_view content) {
	ofstream f(path);
	if (f.is_open()) {
		f << content;
		f.close();
	}
}

void writeLog(string_view message) {
	ofstream log("log.log", ios::app);
	if (log.is_open()) {
		log << "[" << getCurrentDateTime() << "] " << message << endl;
	}
}

void Achievements(string_view message) {
	cout << message << endl;
	if (!fileExists("Achievements.txt")) {
		writeFile("Achievements.txt", "");
	}
	ofstream file("Achievements.txt", ios::app);
	if (file.is_open()) {
		file << "[" << getCurrentDateTime() << "] " << message << endl;
	}
}

string readFile(string_view path) {
	ifstream f(path);
	if (!f.is_open()) return "";
	stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

// Input/output functions for the game
void pause() {
	cout << "Press any key to continue...";
	char TEMP = _getch();
	cout << endl;
}

void ErrorGame(string_view message, int action) { // 1 - exit, 2 - abort, 3 – nothing (the developer will choose the action)
	cout << "The game crashed, the reason: " << message << endl;
	cout << endl;
	cout << "For support, contact the game creator" << endl;
	pause();
	if (action == 1) exit(0);
	else if (action == 2) abort();
	else if (action == 3) {}
}

bool isKeyPressed(int vKey) {
	return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}

void print_slow(string_view text, int delay) { // Recommended delay - 3
	timeBeginPeriod(1);
	for (char c : text) {
		cout << c << flush;
		Sleep(delay);
	}
	cout << endl;
	timeEndPeriod(1);
}

void SetConsoleWidthHeight(int newWidth, int newHeight) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

	short currentHeight = newHeight;

	COORD newBufferSize = { (short)newWidth, csbi.dwSize.Y };
	SetConsoleScreenBufferSize(hOut, newBufferSize);

	SMALL_RECT rect;
	rect.Left = csbi.srWindow.Left;
	rect.Top = csbi.srWindow.Top;
	rect.Right = csbi.srWindow.Left + (short)newWidth - 1;
	rect.Bottom = csbi.srWindow.Top + currentHeight - 1;

	SetConsoleWindowInfo(hOut, TRUE, &rect);
}

void clearScreen() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
	DWORD dwConSize = (csbi.srWindow.Right - csbi.srWindow.Left + 1) *
		(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	WriteConsoleOutputCharacterA(hConsole, "", 0, coordScreen, &cCharsWritten);
	FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
}

void setColor(const string& colorCode) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	try {
		int color = stoi(colorCode, nullptr, 16);
		SetConsoleTextAttribute(hConsole, (WORD)color);
		clearScreen();
	}
	catch (...) {
		SetConsoleTextAttribute(hConsole, 7);
	}
}

// Save/Load Game Functions
void saveGame() {
	stringstream ss;
	ss << "PlayerName=" << PlayerName << endl;
	ss << "PlayerFood=" << PlayerFood << endl;
	ss << "PlayerFoodInventory=" << PlayerFoodInventory << endl;
	ss << "PlayerX=" << PlayerX << endl;
	ss << "PlayerY=" << PlayerY << endl;
	ss << "GlobalEvent=" << GlobalEvent << endl;
	ss << "MonstersKilled=" << KillMonsters << endl;
	ss << "FoodFound=" << FoundFood << endl;
	ss << "DistanceCov=" << Distancecovered << endl;

	writeFile("save.txt", ss.str());
	if (Success == 0) { Success = 1; writeLog("Saved successfully."); }
}

void loadGame() {
	string savefilePath = "save.txt";
	int Success = 0;

	ifstream save(savefilePath);
	string line;
	while (getline(save, line)) {
		size_t sep = line.find('=');
		if (sep == string::npos) continue;
		string key = line.substr(0, sep);
		string value = line.substr(sep + 1);

		if (value.empty()) continue;

		try {
			if (key == "PlayerName") PlayerName = value;
			else if (key == "PlayerFood") PlayerFood = stoi(value);
			else if (key == "PlayerFoodInventory") PlayerFoodInventory = stoi(value);
			else if (key == "PlayerX") PlayerX = stoi(value);
			else if (key == "PlayerY") PlayerY = stoi(value);
			else if (key == "GlobalEvent") GlobalEvent = stoi(value);
			else if (key == "MonstersKilled") KillMonsters = stoi(value);
			else if (key == "FoodFound") FoundFood = stoi(value);
			else if (key == "DistanceCov") Distancecovered = stoi(value);
			Success = 1;
		}
		catch (...) {
			continue;
		}
	}
	if (Success == 1) writeLog("Successfully loaded");
}

// 3D
void render3D_Ultimate(float playerAngle, const string& mapData, int mapWidth, int mapHeight) {

	if (hBufferA == INVALID_HANDLE_VALUE) {
		hBufferA = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		hBufferB = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	}

	HANDLE hCurrentBuffer = hBufferA;
	if (nBufferIndex == 1) {
		hCurrentBuffer = hBufferB;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) return;

	int screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	int renderHeight = screenHeight - 6;

	COORD bufferSize = { (short)screenWidth, (short)screenHeight };
	SetConsoleScreenBufferSize(hCurrentBuffer, bufferSize);

	vector<char> screen(screenWidth * screenHeight, ' ');

	const float FOV_3D = 1.0f;
	const float MAX_DIST = 14.0f;

	for (int x = 0; x < screenWidth; x++) {
		float rayAngle = (playerAngle - FOV_3D / 2.0f) + ((float)x / (float)screenWidth) * FOV_3D;
		float distanceToWall = MAX_DIST;
		char wallChar = ' ';

		float fVx = sinf(rayAngle);
		float fVy = cosf(rayAngle);

		for (float d = 0.2f; d < MAX_DIST; d += 0.05f) {
			int checkX = (int)floorf(PlayerX + fVx * d);
			int checkY = (int)floorf(PlayerY + fVy * d);

			if (checkX >= 0 && checkX < mapWidth && checkY >= 0 && checkY < mapHeight) {
				if (mapData[checkY * mapWidth + checkX] == '#') {
					distanceToWall = d;
					if (d <= 3.5f)      wallChar = '#';
					else if (d <= 7.5f) wallChar = 'X';
					else                wallChar = '.';
					break;
				}
			}
		}

		distanceToWall = distanceToWall * cosf(rayAngle - playerAngle);

		int wallHeight = renderHeight / (distanceToWall + 0.1f);
		int ceiling = (renderHeight - wallHeight) / 2;
		if (ceiling < 0) ceiling = 0;
		int floor = renderHeight - ceiling;

		for (int y = 0; y < renderHeight; y++) {
			int targetIndex = y * screenWidth + x;
			if (y < ceiling) {
				screen[targetIndex] = ' ';
			}
			else if (y >= ceiling && y <= floor) {
				screen[targetIndex] = wallChar;
			}
			else {
				if (y > renderHeight - 3) screen[targetIndex] = '-';
				else if (y > renderHeight - 6) screen[targetIndex] = '`';
				else screen[targetIndex] = ' ';
			}
		}
	}

	stringstream ss1, ss2, ss3, ss4;
	ss1 << "=== 3D PLAYER STATUS ===";
	ss2 << "Player: " << PlayerName << " | Coordinates: X: " << (int)floorf(PlayerX) << ", Y: " << (int)floorf(PlayerY);
	ss3 << "Your hunger: " << PlayerFood << "/40 | Food in inventory: " << PlayerFoodInventory;
	ss4 << "Distance covered: " << Distancecovered * 0.5 << " km | Monsters killed: " << KillMonsters;

	string lines[4] = { ss1.str(), ss2.str(), ss3.str(), ss4.str() };

	for (int i = 0; i < 4; i++) {
		int targetRow = renderHeight + 1 + i;
		for (size_t c = 0; c < lines[i].size() && c < (size_t)screenWidth; c++) {
			screen[targetRow * screenWidth + c] = lines[i][c];
		}
	}

	string controls = "Controls: [W/S] Move, [A/D] Rotate Camera, [H] Eat, [ESC] Save & Exit";
	int controlRow = screenHeight - 1;
	for (size_t c = 0; c < controls.size() && c < (size_t)screenWidth; c++) {
		screen[controlRow * screenWidth + c] = controls[c];
	}

	DWORD written;
	COORD writeCoord = { 0, 0 };
	WriteConsoleOutputCharacterA(hCurrentBuffer, screen.data(), screenWidth * screenHeight, writeCoord, &written);

	SetConsoleActiveScreenBuffer(hCurrentBuffer);

	nBufferIndex = 1 - nBufferIndex;
}