// Copyright © Armsoup 2026
#include "ArmsoupHAREngine.cpp"
#include <cmath>

using namespace std;
using namespace HEngine;

string PlayerName = "Player";
int PlayerFood = 40;
int PlayerFoodInventory = 0;
float PlayerX = 1.5f;
float PlayerY = 0.0f;
float PlayerZ = 1.5f;
int GlobalEvent = 0;
int KillMonsters = 0;
int FoundFood = 0;
int Distancecovered = 0;
bool IsAutoPhysics = true;

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
	Vector3D::returnBuffer();

	IO::clearScreen();
	IO::setColor("4f");
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
					IO::pause();
					break;
				}

				int win = System::getrand(1, 7);
				if (win == 2 || win == 4) {
					PlayerFood--;
					KillMonsters++;
					cout << "VICTORY! You defeated the monster! It dropped meat (+5 to inventory)" << endl;
					PlayerFoodInventory += 5;
					GameMode = 0;
					SLgame::saveGame();
				}
				else {
					PlayerFood -= 2;
					cout << "Defeat! The monster mauled you pretty bad! Your hunger increased." << endl;
				}
				IO::pause();
				break;
			}
			else if (combatChoice == 's') {
				PlayerZ -= 1.0f;
				cout << "You cowardly ran away from the monster backward!" << endl;
				GameMode = 0;
				SLgame::saveGame();
				IO::pause();
				break;
			}
			else {
				startCombat();
				break;
			}
		}
		Sleep(10);
	}
	IO::setColor("1f");
	Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16);
}

void game() {
	while (true) {
		if (PlayerFood == 0 && PlayerFoodInventory == 0) {
			Vector3D::returnBuffer();

			IO::setColor("4f");
			IO::clearScreen();
			cout << "=========================================" << endl;
			cout << "  YOU DIED OF HUNGER IN THE FOREST...   " << endl;
			cout << "=========================================" << endl;
			cout << "Your final score: " << Distancecovered << " steps." << endl;
			cout << "Monsters killed: " << KillMonsters << "." << endl;
			cout << "Food found: " << FoundFood << "." << endl;
			cout << "Kilometers covered: " << Distancecovered * 0.5 << " km." << endl;
			IO::pause();
			exit(0);
		}


		if (GameMode == 1) {
			startCombat();
			continue;
		}

		float oldX = PlayerX;
		float oldZ = PlayerZ;

		if (IO::isKeyPressed('A')) { PlayerAngle3D -= 0.05f; Sleep(15); }
		if (IO::isKeyPressed('D')) { PlayerAngle3D += 0.05f; Sleep(15); }

		if (IO::isKeyPressed('W')) {
			int hungry = 0;
			if (PlayerFood == 0) { Vector3D::returnBuffer(); cout << "You are hungry!" << endl; IO::pause(); hungry = 1; Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
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
			if (PlayerFood == 0) { Vector3D::returnBuffer(); cout << "You are hungry!" << endl; IO::pause(); hungry = 1; Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
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

		if (IO::isKeyPressed('H')) {
			if (PlayerFoodInventory > 0 && PlayerFood < 40) {
				PlayerFood++;
				PlayerFoodInventory--;
				SLgame::saveGame();
				Sleep(300);
			}
		}

		if (IO::isKeyPressed(VK_ESCAPE)) {
			SLgame::saveGame();
			break;
		}

		if ((int)floorf(oldX) != (int)floorf(PlayerX) || (int)floorf(oldZ) != (int)floorf(PlayerZ)) {
			Distancecovered++;
			PlayerFood--;

			int eventRoll = System::getrand(1, 8);
			if (eventRoll < 2) {
				PlayerFoodInventory++;
				FoundFood++;
				SLgame::saveGame();
			}
			else if (eventRoll == 4) {
				GameMode = 1;
			}


			if (Distancecovered == 10 * 2) { Vector3D::returnBuffer(); Filesystem::Achievements("UNLOCKED: The first 10 kilometers!"); Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
			if (Distancecovered == 100 * 2) { Vector3D::returnBuffer(); Filesystem::Achievements("UNLOCKED: The first 100 kilometers!"); PlayerFoodInventory += 6; Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
			if (Distancecovered == 1000 * 2) { Vector3D::returnBuffer(); Filesystem::Achievements("UNLOCKED: The first 1000 kilometers!"); PlayerFoodInventory += 30; Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
			if (Distancecovered == 40075 * 2) { Vector3D::returnBuffer(); Filesystem::Achievements("UNLOCKED: A trip around the world!"); PlayerFoodInventory = 200000; Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16); }
		}
		Vector3D::render3D_Ultimate(PlayerAngle3D, ForestMap3D, 16, 16);
		Sleep(1);
	}
}

int main() {
	timeBeginPeriod(1);
	srand(static_cast<unsigned int>(time(0)));

	try {
		IO::setColor("1f");

		if (Filesystem::fileExists("save.txt")) {
			cout << "Old save file detected! Continue the game? [Y/N]" << endl;
			char loadChoice = _getch();
			loadChoice = tolower(loadChoice);
			if (loadChoice == 'y') {
				if (SLgame::loadGame()) {
					cout << "Game successfully loaded! Welcome back, " << PlayerName << "!" << endl;
					Sleep(1500);
				}
				else {
					cout << "ERROR: load game error :(" << endl;
					IO::pause();
					cout << "Exit..." << endl;
					Sleep(2000);
					exit(0);
				}
			}
			else {
				cout << "Enter your name to begin a new journey:" << endl;
				getline(cin, PlayerName);
				PlayerName = System::trim(PlayerName);
			}
		}
		else {
			cout << "Hello, player, enter your name to begin:" << endl;
			getline(cin, PlayerName);
			PlayerName = System::trim(PlayerName);
		}

		if (Distancecovered == 0) {
			IO::clearScreen();
			IO::print_slow("-Hi! I found something...", 75);
			Sleep(2500);
			IO::print_slow("-Hi, so what strange thing did you find?", 75);
			Sleep(2500);
			IO::print_slow("-It... looks like some kind of creature!", 75);
			Sleep(2500);
			IO::print_slow("-Hahaha, I don't believe you, so I'm heading into that forest myself!", 75);
			IO::pause();
		}
		game();
	}
	catch (...) {
		Vector3D::returnBuffer();
		DWORD error = GetLastError();
		stringstream ss;
		ss << "\n========================================================================" << endl;
		ss << "                                HAR3D BETA V6.3" << endl;
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