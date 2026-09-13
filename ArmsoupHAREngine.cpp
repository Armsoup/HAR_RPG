// Copyright © Armsoup 2026
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <timeapi.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <conio.h>
#include <random>
#include <filesystem>
#pragma comment(lib, "winmm.lib")

#undef min
#undef max

// GLOBAL VARIABLES 
extern std::string PlayerName;
extern int PlayerFood;
extern int PlayerFoodInventory;
extern float PlayerX;
extern float PlayerY;
extern float PlayerZ;
extern int GlobalEvent;
extern int KillMonsters;
extern int FoundFood;
extern int Distancecovered;
extern bool IsAutoPhysics;

static int Success = 0;
static HANDLE hBufferA = INVALID_HANDLE_VALUE;
static HANDLE hBufferB = INVALID_HANDLE_VALUE;
static int nBufferIndex = 0;

namespace HEngine {
	class System {
	public:
		static std::string trim(const std::string& s);
		static std::string getCurrentDateTime();
		static int getrand(int min, int max);
	};
	// File system functions for the game
	class Filesystem {
	public:
		static bool fileExists(std::string_view path);
		static bool dirExists(std::string_view path);
		static void writeFile(const std::string& path, std::string_view content);
		static void writeLog(std::string_view message);
		static void Achievements(std::string_view message);
		static std::string readFile(const std::string& path);
	};
	// Input/output functions for the game
	class IO {
	public:
		static void pause();
		static void ErrorGame(std::string_view message, std::string_view game_creator, int action);
		static bool isKeyPressed(int vKey);
		static void print_slow(std::string_view text, int delay);
		static void SetConsoleWidthHeight(int newWidth, short newHeight);
		static void clearScreen();
		static void setColor(const std::string& colorCode);
		static void setLanguage(const std::string& Name);
	};
	// Save/Load Game Functions
	class SLgame {
	public:
		static void saveGame();
		static bool loadGame();
	};
	// Physics (NOT A PhysX!)
	class Physics {
	public:
		static bool check_collission(std::string_view Map, float nextX, float nextZ);
		static void Gravity();
		static float GetSpeed();
		static float GetVelocity(std::string_view coordinate);
	};
	// 3D
	class Vector3D {
	public:
		static void returnBuffer();
		static void render3D_Ultimate(float playerAngle, const std::string& mapData, int mapWidth, int mapHeight);
	};
}
std::string HEngine::System::trim(const std::string& s) {
	size_t start = s.find_first_not_of(" \t\r\n");
	if (start == std::string::npos) return "";
	size_t end = s.find_last_not_of(" \t\r\n");
	return s.substr(start, end - start + 1);
}

std::string HEngine::System::getCurrentDateTime() {
	auto now = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(now);
	struct tm tm_info;
	localtime_s(&tm_info, &tt);
	char buf[64];
	strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &tm_info);
	return std::string(buf);
}
int HEngine::System::getrand(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}


bool HEngine::Filesystem::fileExists(std::string_view path) {
	return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}
bool HEngine::Filesystem::dirExists(std::string_view path) {
	return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}
void HEngine::Filesystem::writeFile(const std::string& path, std::string_view content) {
	std::ofstream f(path);
	if (f.is_open()) {
		f << content;
		f.close();
	}
}
void HEngine::Filesystem::writeLog(std::string_view message) {
	std::ofstream log("log.log", std::ios::app);
	if (log.is_open()) {
		log << "[" << System::getCurrentDateTime() << "] " << message << std::endl;
	}
}
void HEngine::Filesystem::Achievements(std::string_view message) {
	std::cout << message << std::endl;
	if (!fileExists("Achievements.txt")) {
		writeFile("Achievements.txt", "");
	}
	std::ofstream file("Achievements.txt", std::ios::app);
	if (file.is_open()) {
		file << "[" << System::getCurrentDateTime() << "] " << message << std::endl;
	}
}
std::string HEngine::Filesystem::readFile(const std::string& path) {
	std::ifstream f(path);
	if (!f.is_open()) return "";
	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

void HEngine::IO::pause() {
	std::cout << "Press any key to continue...";
	char TEMP = _getch();
	std::cout << std::endl;
}
void HEngine::IO::ErrorGame(std::string_view message, std::string_view game_creator, int action) { // 1 - exit, 2 - abort, 3 – nothing (the developer will choose the action)
	std::cout << "The game crashed, the reason: " << message << std::endl;
	std::cout << std::endl;
	std::cout << "Contact the " << game_creator << " BEFORE reaching out to Armsoup." << std::endl;
	pause();
	if (action == 1) exit(0);
	else if (action == 2) abort();
	else if (action == 3) {}
}
bool HEngine::IO::isKeyPressed(int vKey) {
	return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}
void HEngine::IO::print_slow(std::string_view text, int delay) { // Recommended delay - 3
	timeBeginPeriod(1);
	for (char c : text) {
		std::cout << c << std::flush;
		Sleep(delay);
	}
	std::cout << std::endl;
	timeEndPeriod(1);
}
void HEngine::IO::SetConsoleWidthHeight(int newWidth, short newHeight) { // !Not recommended for Windows 11!
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
void HEngine::IO::clearScreen() {
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
void HEngine::IO::setColor(const std::string& colorCode) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	try {
		int color = std::stoi(colorCode, nullptr, 16);
		SetConsoleTextAttribute(hConsole, (WORD)color);
		clearScreen();
	}
	catch (...) {
		SetConsoleTextAttribute(hConsole, 7);
	}
}
void HEngine::IO::setLanguage(const std::string& Name) {
	std::string Lang = HEngine::System::trim(Name);
	if (Lang == "English" || Lang == "english") {
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);
		setlocale(LC_ALL, "en_US.UTF-8");
	}
	else if (Lang == "Russian" || Lang == "russian") {
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		setlocale(LC_ALL, "Russian_Russia.1251");
	}
	else if (Lang == "Chinese" || Lang == "chinese") {
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);
		setlocale(LC_ALL, "zh_CN.UTF-8");
	}
	else if (Lang == "Japanese" || Lang == "japanese") {
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);
		setlocale(LC_ALL, "ja_JP.UTF-8");
	}
	else if (Lang == "Arabic" || Lang == "arabic") { // It is recommended to use English instead of Arabic.
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);
		setlocale(LC_ALL, "ar_SA.UTF-8");
	}
	else {
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);
		setlocale(LC_ALL, "en_US.UTF-8");
		std::cout << "ERROR: Hi, player! The game developer misspelled the language name or entered the name of a non-existent language, so the game will remain in English." << std::endl;
		std::cout << "Info: \"" << Lang << " + " << Name << "\"" << std::endl;
	}
}

void HEngine::SLgame::saveGame() {
	std::stringstream ss;
	ss << "PlayerName=" << PlayerName << std::endl;
	ss << "PlayerFood=" << PlayerFood << std::endl;
	ss << "PlayerFoodInventory=" << PlayerFoodInventory << std::endl;
	ss << "PlayerX=" << PlayerX << std::endl;
	ss << "PlayerY=" << PlayerY << std::endl;
	ss << "PlayerZ=" << PlayerZ << std::endl;
	ss << "GlobalEvent=" << GlobalEvent << std::endl;
	ss << "MonstersKilled=" << KillMonsters << std::endl;
	ss << "FoodFound=" << FoundFood << std::endl;
	ss << "DistanceCov=" << Distancecovered << std::endl;

	Filesystem::writeFile("save.txt", ss.str());
	if (Success == 0) { Success = 1; Filesystem::writeLog("Saved successfully."); }
}

bool HEngine::SLgame::loadGame() {
	std::string savefilePath = "save.txt";
	int success = 0;

	std::ifstream save(savefilePath);
	std::string line;
	while (std::getline(save, line)) {
		size_t sep = line.find('=');
		if (sep == std::string::npos) continue;
		std::string key = line.substr(0, sep);
		std::string value = line.substr(sep + 1);

		if (value.empty()) continue;

		try {
			if (key == "PlayerName") PlayerName = value;
			else if (key == "PlayerFood") PlayerFood = std::stoi(value);
			else if (key == "PlayerFoodInventory") PlayerFoodInventory = std::stoi(value);
			else if (key == "PlayerX") PlayerX = std::stof(value);
			else if (key == "PlayerY") PlayerY = std::stof(value);
			else if (key == "PlayerZ") PlayerZ = std::stof(value);
			else if (key == "GlobalEvent") GlobalEvent = std::stoi(value);
			else if (key == "MonstersKilled") KillMonsters = std::stoi(value);
			else if (key == "FoodFound") FoundFood = std::stoi(value);
			else if (key == "DistanceCov") Distancecovered = std::stoi(value);
			success = 1;
		}
		catch (...) {
			return false;
		}
	}
	if (success == 1) { Filesystem::writeLog("Successfully loaded"); return true; }
}

bool HEngine::Physics::check_collission(std::string_view Map, float nextX, float nextZ) {
	if (Map[(int)floorf(nextZ) * 16 + (int)floorf(nextX)] == '.' || Map[(int)floorf(nextZ) * 16 + (int)floorf(nextX)] == '\0') {
		return true;
	}
	else {
		return false;
	}
}
static float gravity = 0.1f;
static float PlayerVelocityY = 0.0f;
void HEngine::Physics::Gravity() {
	const int STEPS = 8;
	for (int i = 0; i < STEPS; i++) {
		if (PlayerY > 0.0f) {
			PlayerVelocityY += gravity * 0.01f;
			PlayerY -= PlayerVelocityY;
		}
	}
	if (PlayerY <= 0.0f) {
		PlayerY = 0.0f;
		PlayerVelocityY = 0.0f;
	}
}
float HEngine::Physics::GetSpeed() {
	static float oldX = PlayerX, oldY = PlayerY, oldZ = PlayerZ;
	float speed = fabs(PlayerX - oldX) + fabs(PlayerY - oldY) + fabs(PlayerZ - oldZ);
	oldX = PlayerX;
	oldY = PlayerY;
	oldZ = PlayerZ;
	return speed;
}
float HEngine::Physics::GetVelocity(std::string_view coordinate) {
	if (coordinate == "X") {
		static float oldX = PlayerX;
		float speed = fabs(PlayerX - oldX);
		oldX = PlayerX;
		return speed;
	}
	else if (coordinate == "Y") {
		static float oldY = PlayerY;
		float speed = fabs(PlayerY - oldY);
		oldY = PlayerY;
		return speed;
	}
	else if (coordinate == "Z") {
		static float oldZ = PlayerZ;
		float speed = fabs(PlayerZ - oldZ);
		oldZ = PlayerZ;
		return speed;
	}
	else {
		return 0;
	}
}

void HEngine::Vector3D::returnBuffer() {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleActiveScreenBuffer(hStdOut);
}
void HEngine::Vector3D::render3D_Ultimate(float playerAngle, const std::string& mapData, int mapWidth, int mapHeight) {
	if (IsAutoPhysics) {
		HEngine::Physics::Gravity();
	}
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

	std::vector<char> screen(screenWidth * screenHeight, ' ');

	const float FOV_3D = 1.0f;
	const float MAX_DIST = 14.0f;

	for (int x = 0; x < screenWidth; x++) {
		float rayAngle = (playerAngle - FOV_3D / 2.0f) + ((float)x / (float)screenWidth) * FOV_3D;
		float distanceToWall = MAX_DIST;
		char wallChar = ' ';

		float fVx = sinf(rayAngle);
		float fVz = cosf(rayAngle);

		for (float d = 0.2f; d < MAX_DIST; d += 0.05f) {
			int checkX = (int)floorf(PlayerX + fVx * d);
			int checkZ = (int)floorf(PlayerZ + fVz * d);

			if (mapData[checkZ * mapWidth + checkX] != '.' && mapData[checkZ * mapWidth + checkX] != '\0') {
				distanceToWall = d;
				float hitX = PlayerX + fVx * d;
				float hitZ = PlayerZ + fVz * d;
				float blockX = hitX - floorf(hitX);
				float blockZ = hitZ - floorf(hitZ);
				float distX = std::min(blockX, 1.0f - blockX);
				float distZ = std::min(blockZ, 1.0f - blockZ);
				bool isVerticalWall = (distX < distZ);
				if (d <= 3.5f) {
					wallChar = isVerticalWall ? 'X' : '#';
				}
				else if (d <= 7.5f) {
					wallChar = isVerticalWall ? '.' : 'X';
				}
				else if (d <= 9.5f) {
					wallChar = isVerticalWall ? '.' : '.';
				}
				else {
					wallChar = isVerticalWall ? ' ' : '.';
				}
				break;
			}
		}

		distanceToWall = distanceToWall * cosf(rayAngle - playerAngle);
		int wallHeight = renderHeight / (distanceToWall + 0.1f);
		int ceiling = (renderHeight - wallHeight) / 2 - (int)(PlayerY * 2);
		// int wallHeight = renderHeight / (distanceToWall + 0.1f);
		// int ceiling = (renderHeight - wallHeight) / 2;
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

	std::stringstream ss1, ss2, ss3, ss4;
	ss1 << "=== 3D PLAYER STATUS ===";
	ss2 << "Player: " << PlayerName << " | Coordinates: X: " << (int)floorf(PlayerX) << ", Y: " << (int)floorf(PlayerY) << ", Z: " << (int)floorf(PlayerZ);
	ss3 << "Your hunger: " << PlayerFood << "/40 | Food in inventory: " << PlayerFoodInventory;
	ss4 << "Distance covered: " << Distancecovered * 0.5 << " km | Monsters killed: " << KillMonsters << " | Speed: " << HEngine::Physics::GetSpeed();

	std::string lines[4] = { ss1.str(), ss2.str(), ss3.str(), ss4.str() };

	for (int i = 0; i < 4; i++) {
		int targetRow = renderHeight + 1 + i;
		for (size_t c = 0; c < lines[i].size() && c < (size_t)screenWidth; c++) {
			screen[targetRow * screenWidth + c] = lines[i][c];
		}
	}

	std::string controls = "Controls: [W/S] Move, [A/D] Rotate Camera, [H] Eat, [ESC] Save & Exit";
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