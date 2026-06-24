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

namespace fs = filesystem;

// GLOBAL VARIABLES 
string PlayerName;
int PlayerFood = 20;
int PlayerFoodInventory = 0;
int PlayerX = 0; // Position left/right
int PlayerY = 0; // Position Forward/backward
int GlobalEvent = 0; // 0 - Quietly, 1 - Have you found food, 2 - We met a monster

void game() {
	setColor("1f");
	if (PlayerFood > 20) { cout << "Cheater, huh?" << endl; pause(); exit(0); }
	if (PlayerFood == 0 && PlayerFoodInventory == 0) {
		setColor("4f");
		cout << "=========================================" << endl;
		cout << "  YOU DIED OF HUNGER IN THE FIELD...   " << endl;
		cout << "=========================================" << endl;
		cout << "Your final score: " << PlayerX + PlayerY << " steps." << endl;
		pause();
		exit(0);
	}
	cout << "=== PLAYER STATUS ===" << endl;
	cout << "Coordinates: X: " << PlayerX << ", Y: " << PlayerY << endl;
	cout << "Your hunger: " << PlayerFood << "/20" << endl;
	cout << "Food in inventory: " << PlayerFoodInventory << endl;
	cout << "=====================\n" << endl;

	if (GlobalEvent == 1) {
		cout << "You found canned food! (+1 to inventory)" << endl;
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

	if (PlayerFoodInventory > 0 && PlayerFood < 20) cout << "[H] Have a snack" << endl;
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

		int eventRoll = rand() % 5;
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
		PlayerFood--;

		int eventRoll = rand() % 5;
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
		PlayerFood--;

		int eventRoll = rand() % 5;
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
			PlayerFood--;
		}
		break;
	}
	case 'h': {
		if (PlayerFoodInventory > 0 && PlayerFood < 20) {
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

		int win = rand() % 5;
		if (win == 2 || win == 4) {
			PlayerFood--;
			cout << "VICTORY! You defeated the monster! It dropped meat (+5 to inventory)" << endl;
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