// Copyright © Armsoup 2026
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <filesystem>
#include "ArmsoupHAREngine.cpp"

using namespace std;
using namespace HEngine;

// GLOBAL VARIABLES 
string PlayerName;
int PlayerFood = 40;
int PlayerFoodInventory = 0;
float PlayerX = 0; // Position left/right
float PlayerY = 0; // Position Up/down
float PlayerZ = 0; // Position Forward/backward
int GlobalEvent = 0; // 0 - Quietly, 1 - Have you found food, 2 - We met a monster
bool IsAutoPhysics = true;
int KillMonsters = 0;
int FoundFood = 0;
int Distancecovered = 0;
int achievementskill1 = 0;
int achievementsfood1 = 0;
int achievementskill10 = 0;
int achievementsfood10 = 0;
int achievementskill100 = 0;
int achievementsfood100 = 0;
int achievementsdist10 = 0;
int achievementsdist100 = 0;
int achievementsdist1000 = 0;
int achievementsdist10000 = 0;
int achievementsdist40075 = 0;

void game() {
	IO::setColor("1f");
	if (Distancecovered == 0) {
		// Backstory...
		IO::print_slow("-Hi! I found something...", 75);
		Sleep(3500);
		IO::print_slow("-Hi, so what strange thing did you find?", 75);
		Sleep(3500);
		IO::print_slow("-It... looks like some kind of creature!", 75);
		Sleep(3500);
		IO::print_slow("-Hahaha, I don't believe you, so I'm heading into that forest myself!", 75);
		IO::pause();
		IO::clearScreen();
	}
	if (PlayerFood > 40) { cout << "Cheater, huh?" << endl; IO::pause(); exit(0); }
	if (PlayerFood == 0 && PlayerFoodInventory == 0) {
		IO::setColor("4f");
		cout << "=========================================" << endl;
		cout << "  YOU DIED OF HUNGER IN THE FOREST...   " << endl;
		cout << "=========================================" << endl;
		cout << "Your final score: " << Distancecovered << " steps." << endl;
		cout << "Monsters killed: " << KillMonsters << "." << endl;
		cout << "Food found: " << FoundFood << "." << endl;
		cout << "Kilometers covered: " << Distancecovered * 0.5 << endl;
		IO::pause();
		string ForestMap3D =
			"#..............."
			"#..............."
			"#..###....###..."
			"#..#........#..."
			"#..#........#..."
			"#......##......."
			"#......##......."
			"................"
			"#..............."
			"#..............."
			"#..............."
			"#..............."
			"#..#........#..."
			"#..............."
			"#..............."
			"#...............";
		float PlayerAngle3D = 0.0f;
		PlayerX = 2.0f;
		PlayerZ = 2.0f;
		PlayerFood = 0;
		try {
			while (true) { // Endless 3D walking mode

				if (IO::isKeyPressed('W')) {
					int hungry = 0;
					if (hungry == 0) {
						float nextX = PlayerX + sinf(PlayerAngle3D) * 0.08f;
						float nextZ = PlayerZ + cosf(PlayerAngle3D) * 0.08f;
						if (Physics::check_collission(ForestMap3D, nextX, nextZ)) {
							PlayerX = nextX;
							PlayerZ = nextZ;
						}
						Sleep(40);
					}
				}

				if (IO::isKeyPressed('S')) {
					int hungry = 0;
					if (hungry == 0) {
						float nextX = PlayerX - sinf(PlayerAngle3D) * 0.08f;
						float nextZ = PlayerZ - cosf(PlayerAngle3D) * 0.08f;
						if (Physics::check_collission(ForestMap3D, nextX, nextZ)) {
							PlayerX = nextX;
							PlayerZ = nextZ;
						}
						Sleep(40);
					}
				}
				if (IO::isKeyPressed('A')) { PlayerAngle3D -= 0.05f; Sleep(15); }
				if (IO::isKeyPressed('D')) { PlayerAngle3D += 0.05f; Sleep(15); }
				if (IO::isKeyPressed('P')) exit(0);
				Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16);
				Sleep(1);
			}

		}
		catch (...) {
			Vector3D::returnBuffer();
			DWORD error = GetLastError();
			stringstream ss;
			ss << "\n========================================================================" << endl;
			ss << "                                HAR BETA V6.3" << endl;
			ss << "The game was terminated unexpectedly due to an issue: " << strerror(errno) << endl;
			ss << "Windows error: " << error << endl;
			ss << "If you see \"0\" and \"No error\" instead of an error message," << endl;
			ss << "you may have gone too far," << endl;
			ss << "causing Windows to throw an ACCESS_VIOLATION exception!" << endl;
			ss << "Coordinates: X=" << PlayerX << ", Z=" << PlayerZ << endl;
			ss << "You literally walked into unallocated memory!" << endl;
			ss << "========================================================================\n" << endl;
			IO::ErrorGame(ss.str(), "Armsoup", 1);
		}
	}
	cout << "=== PLAYER STATUS ===" << endl;
	cout << "Coordinates: X: " << PlayerX << ", Z: " << PlayerZ << endl;
	cout << "Your hunger: " << PlayerFood << "/40" << endl;
	cout << "Food in inventory: " << PlayerFoodInventory << endl;
	cout << "=====================\n" << endl;
	if (KillMonsters == 1) {
		if (achievementskill1 == 0) {
			achievementskill1 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: Kill the first monster!");
			cout << "ACHIEVEMENT UNLOCKED: Kill the first monster!" << endl;
		}
	}
	if (KillMonsters == 10) {
		if (achievementskill10 == 0) {
			achievementskill10 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: Kill ten monsters!");
			cout << "ACHIEVEMENT UNLOCKED: Kill ten monsters!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 6;
		}
	}
	if (KillMonsters == 100) {
		if (achievementskill100 == 0) {
			achievementskill100 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: GOD of monsters! (Kill a hundred monsters!)");
			cout << "ACHIEVEMENT UNLOCKED: GOD of monsters!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 60;
		}
	}
	if (FoundFood == 1) {
		if (achievementsfood1 == 0) {
			achievementsfood1 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: Find the first food!");
			cout << "ACHIEVEMENT UNLOCKED: Find the first food!" << endl;
		}
	}
	if (FoundFood == 10) {
		if (achievementsfood10 == 0) {
			achievementsfood10 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: Find ten cans of food!");
			cout << "ACHIEVEMENT UNLOCKED: Find ten cans of food!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 4;
		}
	}
	if (FoundFood == 100) {
		if (achievementsfood100 == 0) {
			achievementsfood100 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: GOD of meat and canned goods! (Find 100 cans of food!)");
			cout << "ACHIEVEMENT UNLOCKED: GOD of meat and canned goods!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 65;
		}
	}
	if (Distancecovered == 10 * 2) {
		if (achievementsdist10 == 0) {
			achievementsdist10 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: The first 10 kilometers!");
			cout << "ACHIEVEMENT UNLOCKED: The first 10 kilometers!" << endl;
		}
	}
	if (Distancecovered == 100 * 2) {
		if (achievementsdist100 == 0) {
			achievementsdist100 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: The first 100 kilometers!");
			cout << "ACHIEVEMENT UNLOCKED: The first 100 kilometers!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 6;
		}
	}
	if (Distancecovered == 1000 * 2) {
		if (achievementsdist1000 == 0) {
			achievementsdist1000 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: The first 1000 kilometers!");
			cout << "ACHIEVEMENT UNLOCKED: The first 1000 kilometers!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 30;
		}
	}
	if (Distancecovered == 10000 * 2) {
		if (achievementsdist10000 == 0) {
			achievementsdist10000 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: The first 10000 kilometers!");
			cout << "ACHIEVEMENT UNLOCKED: The first 10000 kilometers!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 100;
		}
	}
	if (Distancecovered == 40075 * 2) {
		if (achievementsdist40075 == 0) {
			achievementsdist40075 = 1;
			cout << endl;
			Filesystem::Achievements("UNLOCKED: A trip around the world! (Cover 40,075 km)");
			cout << "ACHIEVEMENT UNLOCKED: A trip around the world!" << endl;
			PlayerFoodInventory = 200000;
		}
	}


	if (GlobalEvent == 1) {
		cout << "You found canned food! (+1 to inventory)" << endl;
		FoundFood++;
		GlobalEvent = 0;
		SLgame::saveGame();
	}
	else if (GlobalEvent == 2) {
		cout << "A fierce monster is in front of you! It blocks the way!" << endl;
		cout << "[G] Fight!" << endl;
	}
	else {
		cout << "Silence all around. Where do you want to go?" << endl;
	}

	cout << "\nControls:" << endl;
	cout << "[W] Forward" << endl;
	if (PlayerZ > 0) cout << "[S] Backward" << endl;
	else cout << "[S] Backward (LOCKED)" << endl;
	cout << "[A] Left" << endl;
	cout << "[D] Right" << endl;

	if (PlayerFoodInventory > 0 && PlayerFood < 40) cout << "[H] Have a snack" << endl;
	else cout << "[H] Have a snack (LOCKED)" << endl;
	if (GlobalEvent != 2) cout << "[G] Fight! (LOCKED)" << endl;

	char choice = _getch();
	choice = tolower(choice);
	cout << endl;

	switch (choice) {
	case 'w': {
		if (GlobalEvent == 2) {
			cout << "The monster won't let you forward! Defeat it first!" << endl;
			IO::pause();
			break;
		}
		if (PlayerFood == 0) {
			cout << "You are hungry! Eat some food!" << endl;
			IO::pause();
			break;
		}
		PlayerZ++;
		PlayerFood--;
		Distancecovered++;

		int eventRoll = System::getrand(1, 8);
		if (eventRoll < 2) {
			PlayerFoodInventory++;
			GlobalEvent = 1;
		}
		else if (eventRoll == 4) {
			GlobalEvent = 2;
		}
		break;
	}
	case 'a': {
		if (GlobalEvent == 2) { cout << "You cannot escape from the monster to the left!" << endl; IO::pause(); break; }
		if (PlayerFood == 0) { cout << "You are hungry!" << endl; IO::pause(); break; }
		PlayerX--;
		Distancecovered++;
		PlayerFood--;

		int eventRoll = System::getrand(1, 8);
		if (eventRoll < 2) {
			PlayerFoodInventory++;
			GlobalEvent = 1;
		}
		else if (eventRoll == 4) {
			GlobalEvent = 2;
		}
		break;
	}
	case 'd': {
		if (GlobalEvent == 2) { cout << "You cannot escape from the monster to the right!" << endl; IO::pause(); break; }
		if (PlayerFood == 0) { cout << "You are hungry!" << endl; IO::pause(); break; }
		PlayerX++;
		Distancecovered++;
		PlayerFood--;

		int eventRoll = System::getrand(1, 8);
		if (eventRoll < 2) {
			PlayerFoodInventory++;
			GlobalEvent = 1;
		}
		else if (eventRoll == 4) {
			GlobalEvent = 2;
		}
		break;
	}
	case 's': {
		if (GlobalEvent == 2) {
			GlobalEvent = 0;
			PlayerZ--;
			cout << "You cowardly ran away from the monster backward!" << endl;
			SLgame::saveGame();
			IO::pause();
			break;
		}
		if (PlayerZ > 0) {
			if (PlayerFood == 0) { cout << "You are hungry!" << endl; IO::pause(); break; }
			PlayerZ--;
			Distancecovered++;
			PlayerFood--;
		}
		break;
	}
	case 'h': {
		if (PlayerFoodInventory > 0 && PlayerFood < 40) {
			PlayerFood++;
			PlayerFoodInventory--;
			cout << "You had a great snack!" << endl;
			SLgame::saveGame();
		}
		else {
			cout << "You cannot eat right now!" << endl;
		}
		IO::pause();
		break;
	}
	case 'g': {
		if (GlobalEvent != 2) {
			cout << "There is no one to fight here!" << endl;
			IO::pause();
			break;
		}
		if (PlayerFood < 2) {
			PlayerFood = 0;
			cout << "You are too weak from hunger to fight! You lost..." << endl;
			GlobalEvent = 0;
			IO::pause();
			break;
		}

		int win = System::getrand(1, 7);
		if (win == 2 || win == 4) {
			PlayerFood--;
			cout << "VICTORY! You defeated the monster! It dropped meat (+5 to inventory)" << endl;
			KillMonsters++;
			PlayerFoodInventory += 5;
			GlobalEvent = 0;
			SLgame::saveGame();
		}
		else {
			PlayerFood -= 2;
			cout << "Defeat! The monster mauled you pretty bad! Your hunger increased." << endl;
		}
		IO::pause();
		break;
	}
	}
}

int main() {
	timeBeginPeriod(1);
	srand(static_cast<unsigned int>(time(0)));

	try {
		if (Filesystem::fileExists("save.txt")) {
			cout << "Old save file detected! Continue the game? [Y/N]" << endl;
			char loadChoice = _getch();
			loadChoice = tolower(loadChoice);
			cout << endl;

			if (loadChoice == 'y') {
				if (SLgame::loadGame()) {
					cout << "Game successfully loaded! Welcome back, " << PlayerName << "!" << endl;
					Sleep(2000);
				}
				else {
					cout << "ERROR: load game error :(" << endl;
					IO::pause();
					cout << "Exit..." << endl;
					Sleep(2000);
					exit(0);
				}

				while (true) {
					IO::clearScreen();
					game();
				}
				return 0;
			}
		}

		cout << "Hello, player, enter your name to begin" << endl;
		getline(cin, PlayerName);
		PlayerName = System::trim(PlayerName);
		cout << "\nGreat, " << PlayerName << ", start the game?" << endl;
		char choice = _getch();
		choice = tolower(choice);
		switch (choice) {
		case 'y': {
			IO::clearScreen();
			cout << "Starting!" << endl;
			Sleep(2000);
			break;
		}
		case 'n': {
			exit(0);
		}
		default: {
			IO::clearScreen();
			cout << "Starting!" << endl;
			Sleep(2000);
			break;
		}
		}
		while (true) {
			IO::clearScreen();
			game();
		}
	}
	catch (...) {
		Vector3D::returnBuffer();
		DWORD error = GetLastError();
		stringstream ss;
		ss << "\n========================================================================" << endl;
		ss << "                                HAR BETA V6.3" << endl;
		ss << "The game was terminated unexpectedly due to an issue: " << strerror(errno) << endl;
		ss << "Windows error: " << error << endl;
		ss << "If you see \"0\" and \"No error\" instead of an error message," << endl;
		ss << "you may have gone too far," << endl;
		ss << "causing Windows to throw an ACCESS_VIOLATION exception!" << endl;
		ss << "Coordinates: X=" << PlayerX << ", Z=" << PlayerZ << endl;
		ss << "You literally walked into unallocated memory!" << endl;
		ss << "========================================================================\n" << endl;
		IO::ErrorGame(ss.str(), "Armsoup", 1);
	}
	return 0;
}