// Copyright © Armsoup 2026
#include "ArmsoupHAREngine.hpp"
#include <cmath>

using namespace std;
using namespace HEngine;

string PlayerName = "Player";
int PlayerFood = 40;
int PlayerFoodInventory = 0;
float PlayerX = 1.5f;
float PlayerY = 1.5f;
int GlobalEvent = 0;
int KillMonsters = 0;
int FoundFood = 0;
int Distancecovered = 0;

float PlayerAngle3D = 0.0f;
int GameMode = 0;

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

void startCombat() {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleActiveScreenBuffer(hStdOut);

	clearScreen();
	setColor("4f");
	cout << "=========================================" << endl;
	cout << "A fierce monster is in front of you! It blocks the way!" << endl;
	cout << "=========================================" << endl;
	cout << "Your hunger: " << PlayerFood << "/40" << endl;
	cout << "[G] Fight!" << endl;
	cout << "[S] Run cowardly backward!" << endl;

	while (GameMode == 1) {
		if (_kbhit()) {
			char combatChoice = _getch();
			combatChoice = tolower(combatChoice);

			if (combatChoice == 'g') {
				if (PlayerFood < 2) {
					PlayerFood = 0;
					cout << "You are too weak from hunger to fight! You lost..." << endl;
					GameMode = 0;
					pause();
					break;
				}

				int win = rand() % 8;
				if (win == 2 || win == 4) {
					PlayerFood--;
					KillMonsters++;
					cout << "VICTORY! You defeated the monster! It dropped meat (+5 to inventory)" << endl;
					PlayerFoodInventory += 5;
					GameMode = 0;
					saveGame();
				}
				else {
					PlayerFood -= 2;
					cout << "Defeat! The monster mauled you pretty bad! Your hunger increased." << endl;
				}
				pause();
				break;
			}
			else if (combatChoice == 's') {
				PlayerY -= 1.0f;
				cout << "You cowardly ran away from the monster backward!" << endl;
				GameMode = 0;
				saveGame();
				pause();
				break;
			}
		}
		Sleep(10);
	}
	setColor("1f");
	render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16);
}

int main() {
	srand(static_cast<unsigned int>(time(0)));

	setColor("1f");

	if (fileExists("save.txt")) {
		cout << "Old save file detected! Continue the game? [Y/N]" << endl;
		char loadChoice = _getch();
		loadChoice = tolower(loadChoice);
		if (loadChoice == 'y') {
			loadGame();
			cout << "Game successfully loaded! Welcome back, " << PlayerName << "!" << endl;
			Sleep(1500);
		}
		else {
			cout << "Enter your name to begin a new journey:" << endl;
			getline(cin, PlayerName);
			PlayerName = trim(PlayerName);
		}
	}
	else {
		cout << "Hello, player, enter your name to begin:" << endl;
		getline(cin, PlayerName);
		PlayerName = trim(PlayerName);
	}

	if (Distancecovered == 0) {
		clearScreen();
		print_slow("-Hi! I found something...", 75);
		Sleep(2500);
		print_slow("-Hi, so what strange thing did you find?", 75);
		Sleep(2500);
		print_slow("-It... looks like some kind of creature!", 75);
		Sleep(2500);
		print_slow("-Hahaha, I don't believe you, so I'm heading into that forest myself!", 75);
		pause();
	}

	while (true) {
		if (PlayerFood == 0 && PlayerFoodInventory == 0) {
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleActiveScreenBuffer(hStdOut);

			setColor("4f");
			clearScreen();
			cout << "=========================================" << endl;
			cout << "  YOU DIED OF HUNGER IN THE FOREST...   " << endl;
			cout << "=========================================" << endl;
			cout << "Your final score: " << Distancecovered << " steps." << endl;
			cout << "Monsters killed: " << KillMonsters << "." << endl;
			cout << "Food found: " << FoundFood << "." << endl;
			cout << "Kilometers covered: " << Distancecovered * 0.5 << " km." << endl;
			pause();
			exit(0);
		}


		if (GameMode == 1) {
			startCombat();
			continue;
		}

		float oldX = PlayerX;
		float oldY = PlayerY;

		if (isKeyPressed('A')) PlayerAngle3D -= 0.05f;
		if (isKeyPressed('D')) PlayerAngle3D += 0.05f;

		if (isKeyPressed('W')) {
			int hungry = 0;
			if (PlayerFood == 0) { HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleActiveScreenBuffer(hStdOut); cout << "You are hungry!" << endl; pause(); hungry = 1; render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
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
			if (PlayerFood == 0) { HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleActiveScreenBuffer(hStdOut); cout << "You are hungry!" << endl; pause(); hungry = 1; render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
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

		if (isKeyPressed('H')) {
			if (PlayerFoodInventory > 0 && PlayerFood < 40) {
				PlayerFood++;
				PlayerFoodInventory--;
				saveGame();
				Sleep(300);
			}
		}

		if (isKeyPressed(VK_ESCAPE)) {
			saveGame();
			break;
		}

		if ((int)floorf(oldX) != (int)floorf(PlayerX) || (int)floorf(oldY) != (int)floorf(PlayerY)) {
			Distancecovered++;
			PlayerFood--;

			int eventRoll = rand() % 9;
			if (eventRoll < 2) {
				PlayerFoodInventory++;
				FoundFood++;
				saveGame();
			}
			else if (eventRoll == 4) {
				GameMode = 1;
			}


			if (Distancecovered == 10 * 2) { HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleActiveScreenBuffer(hStdOut); Achievements("UNLOCKED: The first 10 kilometers!"); render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
			if (Distancecovered == 100 * 2) { HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleActiveScreenBuffer(hStdOut); Achievements("UNLOCKED: The first 100 kilometers!"); PlayerFoodInventory += 6; render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
			if (Distancecovered == 1000 * 2) { HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleActiveScreenBuffer(hStdOut); Achievements("UNLOCKED: The first 1000 kilometers!"); PlayerFoodInventory += 30; render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
			if (Distancecovered == 40075 * 2) { HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleActiveScreenBuffer(hStdOut); Achievements("UNLOCKED: A trip around the world!"); PlayerFoodInventory = 200000; render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
		}

		render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16);
		Sleep(15);
	}
	return 0;
}
