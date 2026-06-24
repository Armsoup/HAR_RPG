// Copyright © Armsoup 2026
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <filesystem>
#include "ArmsoupHAREngine.hpp"

using namespace std;

namespace fs = filesystem;

// GLOBAL VARIABLES 
extern string PlayerName;
extern int PlayerFood;
extern int PlayerFoodInventory;
extern int PlayerX;
extern int PlayerY;
extern int GlobalEvent;

string trim(const string& s) {
	size_t start = s.find_first_not_of(" \t\r\n");
	if (start == string::npos) return "";
	size_t end = s.find_last_not_of(" \t\r\n");
	return s.substr(start, end - start + 1);
}

bool fileExists(string_view path) {
	return fs::exists(path) && fs::is_regular_file(path);
}

void writeFile(string_view path, string_view content) {
	ofstream f(path);
	if (f.is_open()) {
		f << content;
		f.close();
	}
}

void pause() {
	cout << "Press any key to continue...";
	char TEMP = _getch();
	cout << endl;
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

void saveGame() {
	stringstream ss;
	ss << "PlayerName=" << PlayerName << endl;
	ss << "PlayerFood=" << PlayerFood << endl;
	ss << "PlayerFoodInventory=" << PlayerFoodInventory << endl;
	ss << "PlayerX=" << PlayerX << endl;
	ss << "PlayerY=" << PlayerY << endl;
	ss << "GlobalEvent=" << GlobalEvent << endl;

	writeFile("save.txt", ss.str());
}

void loadGame() {
	string savefilePath = "save.txt";

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
		}
		catch (...) {
			continue;
		}
	}
}