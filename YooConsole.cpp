/*
+----------------+
|   SkyOS   |
+----------------+

Code written by Jackie Gamby-Haycock 2021
*/
#include <iostream>
#include <string>
#include <stdlib.h>
#include <locale>
#include <iterator>
#include <sstream>
#include <windows.h>
#include <direct.h>
#include <vector>

#define _CRT_SECURE_NO_WARNINGS 1
#define mathPi 3.141

using namespace std;

string pass = "Bung";

class Character
{
public:
	Character(string, int, int);
	string characterName = "";
	int health = 10;
	int attack = 1;
	int potions = 5;
	int weaponBonus = 0;
	int armourBonus = 0;
};

Character::Character(string name, int hp, int ap)
{
	characterName = name;
	health = hp;
	attack = ap;
}

Character player = Character("Player", 10, 1);

string GetCurrentPath();

/// <summary>
/// 
/// </summary>
/// <param name="combatants">List of all ENEMY characters in combat encounter</param>
/// <returns></returns>
int CombatEncounter(Character combatants, int enemyCount = 0)
{
	system("CLS");

	int combatResult = 0;
	bool allEnemiesDead = false;

	cout << combatants.characterName << " confronts you!\n\n";

	while (allEnemiesDead == false)
	{
		// Player Turn
		string plrAction = "";

		cout << "You have " << player.health << " HP\n";
		cout << "attack (" << (player.attack + player.weaponBonus) << " damage) | potion (" << player.potions << " left) | flee\n> ";
		cin >> plrAction;

		if (plrAction == "flee")
		{
			int targetChance = rand() % 5;
			int playerValue = rand() % 5;

			if (targetChance == playerValue)
			{
				combatResult = 2;
				break;
			}
			else
			{
				cout << "You weren't quick enough!\n";
			}
		}
		else if (plrAction == "potion")
		{
			if (player.potions > 0)
			{
				player.potions--;
				player.health += 5;

				if (player.health > 10)
				{
					player.health = 10;
				}

				cout << "You consume a health potion, and regain 5 HP.\nYou now have " << player.health << " HP!\n";
			}
			else
			{
				cout << "You've run out of potions!\n";
			}
		}
		else if (plrAction == "attack")
		{
			combatants.health -= player.attack;
			cout << "You swing your weapon at the " << combatants.characterName << ", dealing " << (player.attack + player.weaponBonus) << " damage!\n";
		}
		// Enemy Turn

		if (combatants.health > 0)
		{
			cout << combatants.characterName << " attacks (" << combatants.attack << " DP)!\n";

			if (combatants.attack >= player.armourBonus)
			{
				int attackRand = rand() % 3 - 1;
				int finalAttack = combatants.attack + attackRand;

				if (finalAttack < 0)
				{
					cout << "But they miss!\n";
				}
				else
				{
					player.health -= finalAttack;
					cout << combatants.characterName << " deals " << finalAttack << " damage!\n";
				}
			}
			else
			{
				cout << "But they miss!\n";
			}
		}
		else
		{
			combatResult = 1;
			break;
		}

		if (player.health <= 0)
		{
			combatResult = 0;
			break;
		}

		system("pause");
		system("CLS");
	}

	if (combatResult == 0)
	{
		cout << player.characterName << " was defeated!\n\n";
	}
	else if (combatResult == 1)
	{
		cout << player.characterName << " has defeated all enemies!\n\n";
	}
	else if (combatResult == 2)
	{
		cout << player.characterName << " successfully fled combat!\n\n";
	}

	return combatResult;
}

int DoChoice(string branches[], int choiceCount, bool clear = false)
{
	int choice = 0;

	cout << "\n\n===[ USE NUMBERS TO CHOOSE ]===\n\nCurrent HP: " << player.health << "/10\nCurrent Potions: " << player.potions << "\n\n";

	while (true)
	{
		for (int i = 0; i < choiceCount; i++)
		{
			if (i > 0)
			{
				cout << "\n";
			}

			cout << " [" << i << "] " << branches[i];
		}
		cout << "\n [" << choiceCount << "] Consume Potion (" << player.potions << ")";
		cout << "\n\n> ";
		cin >> choice;

		if (choice == choiceCount)
		{
			system("CLS");

			if (player.potions > 0)
			{
				cout << "You drink one of your potions, gaining 5 HP!\n";
				player.potions--;
				player.health += 5;

				if (player.health > 10)
					player.health = 10;
			}
			else
			{
				cout << "You rummage for a potion, but there don't appear to be any left!";
			}

			system("pause");
			break;
		}
		else if (choice < choiceCount)
		{
			break;
		}
		else
		{
			cout << "Choice number [" << choice << "] doesn't exist! Please input a number between 0 and " << (choiceCount - 1) << "!";
		}
	}

	if (clear)
	{
		system("CLS");
	}

	return choice;
}

void GameOver()
{
	cout << "\nAs your vision fades to black, you drop your weapon and fall to the ground...\nLooks like this is the end of your adventure!";
}

int RunAdventure()
{
	Character enemies[] =
	{
		Character("Rat", 5, 1),
		Character("Goblin", 10, 1),
		Character("Hobbe", 7, 2),
		Character("Skeleton", 10, 2),
		Character("Zombie", 12, 3)
	};

	system("CLS");

	string choices[] = { "" };
	int choice = 0;

	player.armourBonus = 0;

	while (true)
	{
		cout << "What is your name? ";
		getline(cin >> ws, player.characterName);
		// cin >> player.characterName;

		string nameVerif = "";

		cout << "\nAre you sure your name is " << player.characterName << " (y/n)? ";

	nameVerifStart:
		cin >> nameVerif;


		if (nameVerif == "y") // Yes
		{
			break;
		}
		else if (nameVerif == "n")
		{

		}
		else
		{
			goto nameVerifStart;
		}
	}

	system("CLS");

	cout << "You wake up in a dark cave. Your friends have left, leaving you alone in the cave with nothing more than an\n";

	bool takenAxe = false;

	while (true)
	{
		if (takenAxe == false)
		{
			cout << "EXTINGUISHED FIRE, and a SMALL AXE. What do you do?";

			string choices[] = { "Examine EXTINGUISHED FIRE", "Take SMALL AXE", "Look for an EXIT" };
			choice = DoChoice(choices, 3);
		}
		else
		{
			cout << "EXTINGUISHED FIRE. What do you do?";

			string choices[] = { "Examine EXTINGUISHED FIRE", "Look for an EXIT" };
			choice = DoChoice(choices, 2);
		}

		system("CLS");

		bool skipPause = false;

		if (choice == 0) // Examine fire
		{
			cout << "The campfire is completely burnt out, and your friends didn't even have the courtesy to leave flint, steel and tinder.\n";
		}
		else if (choice == 1 && takenAxe == false) // Take axe
		{
			cout << "You pickup the small axe. It's got a decent weight to it, perfect for chopping wood.\n";
			player.weaponBonus = 1;
			takenAxe = true;
		}
		else if ((takenAxe == false && choice == 2) || (takenAxe == true && choice == 1)) // Exit
		{
			int ran = rand() % 5;

			if (ran > 2)
			{
				string blank = "";
				cout << "After stumbling around the cave for a while, you eventually find an exit.\n";
				system("pause");
				break;
			}
			else
			{
				cout << "In the dim light, you failed to find the exit to the cave.\n";

				if (ran == 2)
				{
					cout << "You did however, find a potion!\n\n";
					player.potions++;
				}
				else
				{
					cout << "\n";
				}
			}
		}
		else
		{
			skipPause = true;
		}

		if (!skipPause)
			system("pause");

		system("CLS");
	}

	system("CLS");

	cout << "You begin questioning how you and your friends got down this far into the cave, you've been walking for what\n";
	cout << "seems like hours. Slowly however, a dim orange glow begins to flow from down the passage...\n";

	cout << "How do you approach the light?";
	string options[] = { "Boldly", "Normally", "Cautiously" };
	choice = DoChoice(options, 3);

	int combatStatus = 0;

	if (choice == 0) // Boldly
	{
		cout << "As you thunder down the cave passage, you hear the sounds of a small group of goblins being alerted of your presence...\n";
		system("pause");
		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

	}
	else if (choice == 1) // Normally
	{
		cout << "You walk down the passage as you would the town streets. A tiny group of goblins stumble upon you...\n";
		system("pause");

		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

	}
	else if (choice == 2) // Cautiously
	{
		cout << "You creep down the cave passage as quietly as you can... A single goblin bumps into you...\n";
		system("pause");

		combatStatus = CombatEncounter(enemies[1], 1);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}
	}

	system("pause");
	system("CLS");

	if (choice == 2)
	{
		cout << "After defeating the goblin, you sneak closer toward the light. As it draws nearer, you realize what it is: A goblin campfire!\n";
		system("pause");
		int stealthRoll = rand() % 20 + 1; // Generates a number between 1 and 20

		if (stealthRoll == 20)
		{
			cout << "You sneak past the goblin camp without making a single sound, not even the brushing of your clothing!";
		}
		else if (stealthRoll == 1)
		{
			cout << "You make a right racket, alerting EVERY goblin in the cave of your exact location!\nSuddenly, hoardes of goblins flood in, slashing you with daggers and\nbeating you with maces.";
			GameOver();
			goto endAdventure;
		}
		else
		{
			if (stealthRoll > 10)
			{
				cout << "You accidentally step on the tail of a goblin, causing him to yelp and wake his partner...\n";
				system("pause");
				combatStatus = CombatEncounter(enemies[1], 1);

				if (combatStatus == 0)
				{
					GameOver();
					goto endAdventure;
				}

				combatStatus = CombatEncounter(enemies[1], 1);

				if (combatStatus == 0)
				{
					GameOver();
					goto endAdventure;
				}
			}
		}
	}
	cout << "\nThis is the end of your adventure! Check back for more, as the story forever grows!";

	goto playerNeverDied;

endAdventure:
	system("pause");
	return 1; // exit code of 1 means player died

playerNeverDied:
	return 0;
}

int StartAdventure()
{
	while (true)
	{
		system("CLS");
		cout << "=====[ BASIC TEXT ADVENTURE ]=====\n\n";

		cout << "begin | exit\n> ";
		string menuoption = "";
		cin >> menuoption;

		if (menuoption == "begin")
		{
			int adv = 0;
			adv = RunAdventure();

			if (adv == 0)
				break;
		}
		else if (menuoption == "exit")
		{
			break;
		}
		else
		{
			cout << "Invalid option: \"" << menuoption << "\"!";
			system("pause");
		}
	}

	return 0;
}

string* SplitString(string input) 
{
	istringstream iss(input);

	vector<string> results((istream_iterator<string>(iss)), istream_iterator<string>());

	return results.data();
}

/// <summary>
/// Main function for running program. Executes automatically at startup
/// </summary>
/// <returns></returns>
int main()
{

	SetConsoleTitle(TEXT("SkyOS - Startup"));

	system("mode con COLS=700");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);

	bool skipLogin = true; // Only enable when running on home computer please :)

	string name = ""; // Initialize the variable "name"
	string passcode = "";

	if (skipLogin == false)
	{
		cout << "Enter Password: ";
		cin >> passcode;
	}

	system("CLS");

	if (passcode == pass || skipLogin)
	{
		cout << "What's your name? "; // Ask user for their name
		getline(cin >> ws, name);
		// cin >> name; // Store the users input into the variable "name"

		system("CLS"); // Clear console window

		int nameLen = name.length(); // Get the length of the users name so that the asterisk box can be scaled appropriately

		// cout << "Your name is " << nameLen << " characters long";
/*
		cout << "\n";
		cout << "\t******";

		for (int i = 0; i < nameLen; i++)
		{
			cout << "*";
		}

		cout << "\n\n\tHello " << name << "\n\n";

		cout << "\t******";

		for (int i = 0; i < nameLen; i++)
		{
			cout << "*";
		}

		cout << "\n\n";
		*/

		cout << "\t+-----------------------+\n";
		cout << "\t|                       |\n";
		cout << "\t|  SkyOS version 0.1.0  |\n";
		cout << "\t|                       |\n";
		cout << "\t+-----------------------+\n\n";

		bool running = true;

		while (running == true)
		{
			SetConsoleTitle(TEXT("SkyOS - Command Line"));

			bool skipNewLine = false;

			string command = "";
			cout << "> ";
			cin >> command;

			if (command == "shutdown")
			{
				running = false;
			}
			else if (command == "cls")
			{
				skipNewLine = true;
				system("CLS");
			}
			else if (command == "help")
			{
				system("CLS");
				SetConsoleTitle(TEXT("SkyOS - Help"));
				cout << "=====[ HELP ]=====\n\n";
				cout << "====[ COMMANDS ]====\n> shutdown - closes the program\n> cls - clears the consolescreen\n> exec - execute a program\n\t> calc - simple calculator\n\t> adventure - a small text adventure\n\n";
				system("pause");
				skipNewLine = true;
				system("CLS");
			}
			else if (command == "exec")
			{
				system("CLS");

				while (true)
				{
					SetConsoleTitle(TEXT("SkyOS - Execute Program"));

					cout << "=====[ EXECUTE PROGRAM ]=====\n\n";

					string exec = "";
					cout << "Programs:\n\n> calc\n> adventure\n\nProgram Name (type cancel to return to main command line): ";
					cin >> exec;

					system("CLS");

					if (exec == "cancel")
					{
						break;
					}
					else if (exec == "calc")
					{
						while (true)
						{
							SetConsoleTitle(TEXT("Calculator"));
							string func = "";
							cout << "===[ FUNCTIONS ]===\n";
							cout << "add | subtract | tri | crcl | odd | prime | exit\n";
							cout << "Desired Function: ";

							cin >> func;

							if (func == "add")
							{
								SetConsoleTitle(TEXT("Calculator - ADD"));

								float numberA = 0;
								float numberB = 0;

								try
								{
									cout << "First Number: ";
									cin >> numberA;

									cout << "Second Number: ";
									cin >> numberB;

									float finalNum = numberA + numberB;

									cout << numberA << "+" << numberB << "=" << finalNum << "\n";

									system("pause");
									system("CLS");
								}
								catch (exception)
								{
									cout << "Error has occurred! Booting back to main program";
								}
							}
							else if (func == "subtract")
							{
								SetConsoleTitle(TEXT("Calculator - SUBTRACT"));

								float numberA = 0;
								float numberB = 0;

								try
								{
									cout << "First Number: ";
									cin >> numberA;

									cout << "Second Number: ";
									cin >> numberB;

									float finalNum = numberA - numberB;

									cout << numberA << "-" << numberB << "=" << finalNum << "\n";

									system("pause");
									system("CLS");
								}
								catch (exception)
								{
									cout << "Error has occurred! Booting back to main program";
								}
							}
							else if (func == "tri")
							{
								SetConsoleTitle(TEXT("Calculator - CALCULATE AREA OF TRIANGLE"));

								string unit = "";
								float sideA = 0;
								float sideB = 0;

								cout << "Target Units: ";
								cin >> unit;

								cout << "\nBase Length: ";
								cin >> sideA;

								cout << "\nHeight Length: ";
								cin >> sideB;

								float result = (sideA * sideB) / 2;
								cout << "(" << sideA << unit << "*" << sideB << unit << ")/2=" << result << unit;
								cout << "\n\nArea of Triangle is " << result << unit << "^2\n";

								system("pause");
								system("CLS");
							}
							else if (func == "crcl")
							{
								SetConsoleTitle(TEXT("Calculator - CALCULATE AREA OF CIRCLE"));

								string unit = "";
								float radius = 0;

								cout << "Target Units: ";
								cin >> unit;

								cout << "\nCircle Radius: ";
								cin >> radius;

								cout << "(pi*(" << radius << unit << "*" << radius << unit << ")=" << ((mathPi * radius) * radius) << unit;
								cout << "\n\nArea of Circle is " << ((mathPi) * (radius * radius)) << unit << "\n";

								system("pause");
								system("CLS");
							}
							else if (func == "odd") 
							{
								int num = 0;
								cout << "Number: ";
								cin >> num;

								if (num % 2 == 0) 
								{
									cout << "\nNumber (" << num << ") is even.\n";
								}
								else 
								{
									cout << "\nNumber (" << num << ") is odd.\n";
								}

								system("pause");
								system("CLS");
							}
							else if (func == "prime") 
							{
								int num = 0;
								cout << "Number: ";
								cin >> num;

								bool isPrime = true;

								if (num == 0 || num == 1) 
								{
									isPrime == false;
								}
								else 
								{
									for (int i = 2; i <= num / 2; i++) 
									{
										if (num % i == 0) 
										{
											isPrime = false;
										}
									}
								}

								if (isPrime) 
								{
									cout << "\nNumber (" << num << ") is prime.\n";
								}
								else 
								{
									cout << "\nNumber (" << num << ") is not prime.\n";
								}

								system("pause");
								system("CLS");
							}
							else if (func == "exit")
							{
								break;
							}
						}
					}
					else if (exec == "adventure")
					{
						SetConsoleTitle(TEXT("Text Adventure"));
						StartAdventure();
					}
					else
					{
						cout << "Unknown Program \"" << exec << "\"\n";
						system("pause");
						system("CLS");
					}
				}

				cout << "Returning to command line...\n\n";
				system("pause");
				skipNewLine = true;
				system("CLS");
			}
			else if (command == "retro")
			{
			colourScreen:
				cout << "Which colour style would you like to use? (Type 'default' or 'amber' or 'sulfer')\n> ";
				string colour = "default";
				cin >> colour;

				if (colour == "default") 
				{
					system("Color 0F");
				}
				else if (colour == "amber") 
				{
					system("Color 06");
				}
				else if (colour == "sulfer") 
				{
					system("Color 02");
				}
				else 
				{
					system("CLS");
					goto colourScreen;
				}
			}
			else if (command == "func") 
			{
				cout << "Enter function:\n";
				string command = "";
				cin >> command;

				if (command == "echo") 
				{
					string value;
					cout << "echo[0] = ";
					getline(cin >> ws, value);
					cout << "\n\n" << value << "\n\n";
				}
			}
			else
			{
				cout << "Unknown command \"" << command << "\". Type \"help\" for a list of all available commands!";
			}

			if (!skipNewLine)
				cout << "\n";
			else
			{
				cout << "\t+-----------------------+\n";
				cout << "\t|                       |\n";
				cout << "\t|  SkyOS version 0.1.0  |\n";
				cout << "\t|                       |\n";
				cout << "\t+-----------------------+\n\n";
			}
		}
	}
	else
	{
		cout << "Invalid Password! Terminating Program...";
	}
}

string GetCurrentPath()
{
	size_t size;
	char* path = NULL;
	return path;
}