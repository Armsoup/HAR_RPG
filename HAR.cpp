// Copyright © Armsoup 2026
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
using namespace HEngine;

namespace fs = filesystem;

// GLOBAL VARIABLES 
string PlayerName;
int PlayerFood = 40;
int PlayerFoodInventory = 0;
float PlayerX = 0; // Position left/right
float PlayerY = 0; // Position Forward/backward
int GlobalEvent = 0; // 0 - Quietly, 1 - Have you found food, 2 - We met a monster
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
	setColor("1f");
	if (Distancecovered == 0) {
		// Backstory...
		print_slow("-Hi! I found something...", 75);
		Sleep(3500);
		print_slow("-Hi, so what strange thing did you find?", 75);
		Sleep(3500);
		print_slow("-It... looks like some kind of creature!", 75);
		Sleep(3500);
		print_slow("-Hahaha, I don't believe you, so I'm heading into that forest myself!", 75);
		pause();
		clearScreen();
	}
	if (PlayerFood > 40) { cout << "Cheater, huh?" << endl; pause(); exit(0); }
	if (PlayerFood == 0 && PlayerFoodInventory == 0) {
		setColor("4f");
		cout << "=========================================" << endl;
		cout << "  YOU DIED OF HUNGER IN THE FOREST...   " << endl;
		cout << "=========================================" << endl;
		cout << "Your final score: " << Distancecovered << " steps." << endl;
		cout << "Monsters killed: " << KillMonsters << "." << endl;
		cout << "Food found: " << FoundFood << "." << endl;
		cout << "Kilometers covered: " << Distancecovered * 0.5 << endl;
		pause();
		string ForestMap3D =
			"################"
			"#..............#"
			"#..###....###..#"
			"#..#........#..#"
			"#..#........#..#"
			"#......##......#"
			"#......##......#"
			"#..............#"
			"#..............#"
			"#..............#"
			"#..............#"
			"#..............#"
			"#..#........#..#"
			"#..............#"
			"#..............#"
			"################";
		float PlayerAngle3D = 0.0f;
		PlayerX = 2.0f;
		PlayerY = 2.0f;
		PlayerFood = 0;
		while (true) { // Endless 3D walking mode
			if (isKeyPressed('W')) {
				int hungry = 0;
				if (hungry == 0) {
					float nextX = PlayerX + sinf(PlayerAngle3D) * 0.08f;
					float nextY = PlayerY + cosf(PlayerAngle3D) * 0.08f;
					if (ForestMap3D[(int)floorf(nextY) * 16 + (int)floorf(nextX)] != '#') {
						PlayerX = nextX;
						PlayerY = nextY;
					}
					Sleep(40);
				}
			}

			if (isKeyPressed('S')) {
				int hungry = 0;
				if (hungry == 0) {
					float nextX = PlayerX - sinf(PlayerAngle3D) * 0.08f;
					float nextY = PlayerY - cosf(PlayerAngle3D) * 0.08f;
					if (ForestMap3D[(int)floorf(nextY) * 16 + (int)floorf(nextX)] != '#') {
						PlayerX = nextX;
						PlayerY = nextY;
					}
					Sleep(40);
				}
			}
			if (isKeyPressed('A')) PlayerAngle3D -= 0.05f;
			if (isKeyPressed('D')) PlayerAngle3D += 0.05f;
			if (isKeyPressed('P')) exit(0);
			render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16);
			Sleep(15);
		}
	}
	cout << "=== PLAYER STATUS ===" << endl;
	cout << "Coordinates: X: " << PlayerX << ", Y: " << PlayerY << endl;
	cout << "Your hunger: " << PlayerFood << "/40" << endl;
	cout << "Food in inventory: " << PlayerFoodInventory << endl;
	cout << "=====================\n" << endl;
	if (KillMonsters == 1) {
		if (achievementskill1 == 0) {
			achievementskill1 = 1;
			cout << endl;
			Achievements("UNLOCKED: Kill the first monster!");
			cout << "ACHIEVEMENT UNLOCKED: Kill the first monster!" << endl;
		}
	}
	if (KillMonsters == 10) {
		if (achievementskill10 == 0) {
			achievementskill10 = 1;
			cout << endl;
			Achievements("UNLOCKED: Kill ten monsters!");
			cout << "ACHIEVEMENT UNLOCKED: Kill ten monsters!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 6;
		}
	}
	if (KillMonsters == 100) {
		if (achievementskill100 == 0) {
			achievementskill100 = 1;
			cout << endl;
			Achievements("UNLOCKED: GOD of monsters! (Kill a hundred monsters!)");
			cout << "ACHIEVEMENT UNLOCKED: GOD of monsters!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 60;
		}
	}
	if (FoundFood == 1) {
		if (achievementsfood1 == 0) {
			achievementsfood1 = 1;
			cout << endl;
			Achievements("UNLOCKED: Find the first food!");
			cout << "ACHIEVEMENT UNLOCKED: Find the first food!" << endl;
		}
	}
	if (FoundFood == 10) {
		if (achievementsfood10 == 0) {
			achievementsfood10 = 1;
			cout << endl;
			Achievements("UNLOCKED: Find ten cans of food!");
			cout << "ACHIEVEMENT UNLOCKED: Find ten cans of food!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 4;
		}
	}
	if (FoundFood == 100) {
		if (achievementsfood100 == 0) {
			achievementsfood100 = 1;
			cout << endl;
			Achievements("UNLOCKED: GOD of meat and canned goods! (Find 100 cans of food!)");
			cout << "ACHIEVEMENT UNLOCKED: GOD of meat and canned goods!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 65;
		}
	}
	if (Distancecovered == 10 * 2) {
		if (achievementsdist10 == 0) {
			achievementsdist10 = 1;
			cout << endl;
			Achievements("UNLOCKED: The first 10 kilometers!");
			cout << "ACHIEVEMENT UNLOCKED: The first 10 kilometers!" << endl;
		}
	}
	if (Distancecovered == 100 * 2) {
		if (achievementsdist100 == 0) {
			achievementsdist100 = 1;
			cout << endl;
			Achievements("UNLOCKED: The first 100 kilometers!");
			cout << "ACHIEVEMENT UNLOCKED: The first 100 kilometers!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 6;
		}
	}
	if (Distancecovered == 1000 * 2) {
		if (achievementsdist1000 == 0) {
			achievementsdist1000 = 1;
			cout << endl;
			Achievements("UNLOCKED: The first 1000 kilometers!");
			cout << "ACHIEVEMENT UNLOCKED: The first 1000 kilometers!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 30;
		}
	}
	if (Distancecovered == 10000 * 2) {
		if (achievementsdist10000 == 0) {
			achievementsdist10000 = 1;
			cout << endl;
			Achievements("UNLOCKED: The first 10000 kilometers!");
			cout << "ACHIEVEMENT UNLOCKED: The first 10000 kilometers!" << endl;
			PlayerFoodInventory = PlayerFoodInventory + 100;
		}
	}
	if (Distancecovered == 40075 * 2) {
		if (achievementsdist40075 == 0) {
			achievementsdist40075 = 1;
			cout << endl;
			Achievements("UNLOCKED: A trip around the world! (Cover 40,075 km)");
			cout << "ACHIEVEMENT UNLOCKED: A trip around the world!" << endl;
			PlayerFoodInventory = 200000;
		}
	}
	

	if (GlobalEvent == 1) {
		cout << "You found canned food! (+1 to inventory)" << endl;
		FoundFood++;
		GlobalEvent = 0;
		saveGame();
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
	if (PlayerY > 0) cout << "[S] Backward" << endl;
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
			pause();
			break;
		}
		if (PlayerFood == 0) {
			cout << "You are hungry! Eat some food!" << endl;
			pause();
			break;
		}
		PlayerY++;
		PlayerFood--;
		Distancecovered++;

		int eventRoll = rand() % 9;
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
		if (GlobalEvent == 2) { cout << "You cannot escape from the monster to the left!" << endl; pause(); break; }
		if (PlayerFood == 0) { cout << "You are hungry!" << endl; pause(); break; }
		PlayerX--;
		Distancecovered++;
		PlayerFood--;

		int eventRoll = rand() % 9;
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
		if (GlobalEvent == 2) { cout << "You cannot escape from the monster to the right!" << endl; pause(); break; }
		if (PlayerFood == 0) { cout << "You are hungry!" << endl; pause(); break; }
		PlayerX++;
		Distancecovered++;
		PlayerFood--;

		int eventRoll = rand() % 9;
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
			PlayerY--;
			cout << "You cowardly ran away from the monster backward!" << endl;
			saveGame();
			pause();
			break;
		}
		if (PlayerY > 0) {
			if (PlayerFood == 0) { cout << "You are hungry!" << endl; pause(); break; }
			PlayerY--;
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
			saveGame();
		}
		else {
			cout << "You cannot eat right now!" << endl;
		}
		pause();
		break;
	}
	case 'g': {
		if (GlobalEvent != 2) {
			cout << "There is no one to fight here!" << endl;
			pause();
			break;
		}
		if (PlayerFood < 2) {
			PlayerFood = 0;
			cout << "You are too weak from hunger to fight! You lost..." << endl;
			GlobalEvent = 0;
			pause();
			break;
		}

		int win = rand() % 8;
		if (win == 2 || win == 4) {
			PlayerFood--;
			cout << "VICTORY! You defeated the monster! It dropped meat (+5 to inventory)" << endl;
			KillMonsters++;
			PlayerFoodInventory += 5;
			GlobalEvent = 0;
			saveGame();
		}
		else {
			PlayerFood -= 2;
			cout << "Defeat! The monster mauled you pretty bad! Your hunger increased." << endl;
		}
		pause();
		break;
	}
	}
}

int main() {
	srand(static_cast<unsigned int>(time(0)));

	if (fileExists("save.txt")) {
		cout << "Old save file detected! Continue the game? [Y/N]" << endl;
		char loadChoice = _getch();
		loadChoice = tolower(loadChoice);
		cout << endl;

		if (loadChoice == 'y') {
			loadGame();
			cout << "Game successfully loaded! Welcome back, " << PlayerName << "!" << endl;
			Sleep(2000);

			while (true) {
				clearScreen();
				game();
			}
			return 0;
		}
	}

	cout << "Hello, player, enter your name to begin" << endl;
	getline(cin, PlayerName);
	PlayerName = trim(PlayerName);
	cout << "\nGreat, " << PlayerName << ", start the game?" << endl;
	char choice = _getch();
	choice = tolower(choice);
	switch (choice) {
	case 'y': {
		clearScreen();
		cout << "Starting!" << endl;
		Sleep(2000);
		break;
	}
	case 'n': {
		exit(0);
	}
	default: {
		clearScreen();
		cout << "Starting!" << endl;
		Sleep(2000);
		break;
	}
	}
	while (true) {
		clearScreen();
		game();
	}
	return 0;
}