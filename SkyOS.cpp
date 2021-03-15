/*
+----------------+
|     SkyOS      |
+----------------+

Code written by Jackie Gamby-Haycock 2021
*/

// =====[ INCLUDES ]=====
#include <iostream>
#include <string>
#include <stdlib.h>
#include <locale>
#include <iterator>
#include <sstream>
#include <windows.h>
#include <direct.h>
#include <vector>
#include <cmath>
#include <experimental/filesystem>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <mmsystem.h>
#include <mciapi.h>

#pragma comment(lib, "Winmm.lib");

// =====[ DEFINES ]=====
#define mathPi 3.141

#include <direct.h>
#define GetCurrentDir _getcwd

// =====[ NAMESPACES ]=====
using namespace std;
using namespace std::experimental::filesystem::v1;

string pass = "Bung";

// =====[ CLASSES ]=====
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
/// Runs a combat encounter with 
/// </summary>
/// <param name="combatants">ENEMY character to use in combat encounter</param>
/// <returns>Outcome of the combat encounter. 0 > Player died, 1 > Player killed the enemies, 2 > Player fled combat</returns>
int CombatEncounter(Character combatants)
{
	system("CLS");

	int combatResult = 0;
	bool allEnemiesDead = false;

	cout << combatants.characterName << " confronts you!\n\n";

	// Keeps combat running while the enemy's still alive
	while (allEnemiesDead == false)
	{
		// Player Turn
		string plrAction = "";

		cout << "You have " << player.health << " HP\n";
		cout << "attack (" << (player.attack + player.weaponBonus) << " damage) | potion (" << player.potions << " left) | flee\n> ";
		cin >> plrAction;

		if (plrAction == "flee") // When the player chooses to flee, the game generates a random number between 0 and 4 for both the target chance, and the players roll. If they're equal, combat ends with result of player fleed.
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
		else if (plrAction == "potion") // When the player chooses to drink a potion, the code takes one potion away, adds 5 to their health and ensures that their health isn't above 10
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
		else if (plrAction == "attack") // When the player attacks, they deal damage based off of their base attack and their weapons bonus.
		{
			combatants.health -= player.attack;
			cout << "You swing your weapon at the " << combatants.characterName << ", dealing " << (player.attack + player.weaponBonus) << " damage!\n";
		}
		// Enemy Turn

		if (combatants.health > 0)
		{
			// AI always attacks
			cout << combatants.characterName << " attacks (" << combatants.attack << " DP)!\n";

			if (combatants.attack >= player.armourBonus)
			{
				int attackRand = rand() % 3 - 1; // Random amount to add to enemy's damage.
				int finalAttack = combatants.attack + attackRand;

				if (finalAttack <= 0) // If attackRand is less than or equal to 0, the AI's attack is counted as a miss.
				{
					cout << "But they miss!\n";
				}
				else
				{ // Otherise, they deal attackRand of damage to the player.
					player.health -= finalAttack;
					cout << combatants.characterName << " deals " << finalAttack << " damage!\n";
				}
			}
			else
			{ // If the players armour bonus is more than the enemy's attack, enemy will automatically miss.
				cout << "But they miss!\n";
			}
		}
		else
		{
			combatResult = 1; // When the player kills all enemy's return combatResult of 1.
			break;
		}

		if (player.health <= 0)
		{
			combatResult = 0; // When the player dies, return combatResult of 0
			break;
		}

		system("pause"); // Wait for any key to be pressed before continuing combat.
		system("CLS"); // Clear screen to make the view remain nice and clean
	}

	// Below are just the different messages for the various combat results. This is done inside the combat function
	// so that it's easier to update them if I think they need changing.
	if (combatResult == 0)
	{
		// Player dies
		cout << player.characterName << " was defeated!\n\n";
	}
	else if (combatResult == 1)
	{
		// Player kills the enemy
		cout << player.characterName << " has defeated the" << combatants.characterName << "!\n\n";
	}
	else if (combatResult == 2)
	{
		// Player flees combat
		cout << player.characterName << " successfully fled combat!\n\n";
	}

	// Return combatResult for use outside of combat.
	return combatResult;
}

/// <summary>
/// Used for players choice input. Returns a number based off the users choice.
/// </summary>
/// <param name="branches"></param>
/// <param name="choiceCount"></param>
/// <param name="clear"></param>
/// <returns></returns>
int DoChoice(string* branches, bool clear = false)
{
	int choice = 0;

	cout << "\n\n===[ USE NUMBERS TO CHOOSE ]===\n\nCurrent HP: " << player.health << "/10\nCurrent Potions: " << player.potions << "\n\n";

	// Create a loop that won't end unless the 'break' keyword is used
	while (true)
	{
		// This for loop goes through the branches list and displays their text and index.
		for (int i = 0; i < sizeof(branches); i++)
		{
			if (i > 0)
			{
				cout << "\n";
			}

			cout << " [" << i << "] " << branches[i];
		}
		cout << "\n [" << sizeof(branches) << "] Consume Potion (" << player.potions << ")"; // Add this option so the player can drink potions outside of combat
		cout << "\n\n> ";
		cin >> choice;

		if (choice == sizeof(branches)) // The player has chosen to drink a potion, execute the code below
		{
			system("CLS");

			if (player.potions > 0) // If the player has potions in their inventory, remove one from inventory, add 5 to the HP and make sure their HP is capped at 10
			{
				cout << "You drink one of your potions, gaining 5 HP!\n";
				player.potions--;
				player.health += 5;

				if (player.health > 10)
					player.health = 10;
			}
			else
			{
				// Display a special message for when the player has no potions
				cout << "You rummage for a potion, but there don't appear to be any left!";
			}

			// Wait for user input.
			system("pause");
			break;
		}
		else if (choice < sizeof(branches)) // This makes sure the player's choice is valid.
		{
			break;
		}
		else
		{
			// When the players choice is outside of the range, show this error message
			cout << "Choice number [" << choice << "] doesn't exist! Please input a number between 0 and " << (sizeof(branches) - 1) << "!";
		}
	}

	if (clear)
	{
		system("CLS"); // If the clear flag is checked, clear the screen before continuing.
	}

	return choice; // Return the player's choice for use outside the function
}

void GameOver()
{
	cout << "\nAs your vision fades to black, you drop your weapon and fall to the ground...\nLooks like this is the end of your adventure!";
}

/// <summary>
/// Base command for the adventure, I am NOT going to comment all of this.
/// </summary>
/// <returns></returns>
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
			choice = DoChoice(choices);
		}
		else
		{
			cout << "EXTINGUISHED FIRE. What do you do?";

			string choices[] = { "Examine EXTINGUISHED FIRE", "Look for an EXIT" };
			choice = DoChoice(choices);
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
	choice = DoChoice(options);

	int combatStatus = 0;

	if (choice == 0) // Boldly
	{
		cout << "As you thunder down the cave passage, you hear the sounds of a small group of goblins being alerted of your presence...\n";
		system("pause");
		combatStatus = CombatEncounter(enemies[1]);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1]);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1]);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1]);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1]);

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

		combatStatus = CombatEncounter(enemies[1]);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1]);

		if (combatStatus == 0)
		{
			GameOver();
			goto endAdventure;
		}

		combatStatus = CombatEncounter(enemies[1]);

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

		combatStatus = CombatEncounter(enemies[1]);

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
				combatStatus = CombatEncounter(enemies[1]);

				if (combatStatus == 0)
				{
					GameOver();
					goto endAdventure;
				}

				combatStatus = CombatEncounter(enemies[1]);

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

/// <summary>
/// Starts the text adventure. Everything in here should be pretty self explanatory
/// </summary>
/// <returns></returns>
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

/// <summary>
/// This function doesn't work yet. Still trying to figure it out :l
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
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
	ifstream colourSav("colour.pref");

	if (colourSav.is_open())
	{
		string l;

		while (getline(colourSav, l))
		{
			string colour = l;

			if (colour == "default")
			{
				system("Color 0F"); // Default is a black background with white text
			}
			else if (colour == "amber")
			{
				system("Color 06"); // Amber is a black background with yellow text
			}
			else if (colour == "sulfer")
			{
				system("Color 02"); // Sulfer is a black background with green text
			}
			else if (colour == "bsod")
			{
				system("Color 17"); // BSOD is a blue background with grey text
			}
			else 
			{
				system(colour.c_str());
			}

			break;
		}
		colourSav.close();
	}

	SetConsoleTitle(TEXT("SkyOS - Startup"));

	bool fullscreenMode = true;

	// The three lines below make sure the window starts in fullscreen. Can be disabled, but really gives the old DOS feeling

	if (fullscreenMode) 
	{
		system("mode con COLS=700");
		ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
		SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	}

	bool skipLogin = true; // Skips the password field. Just leaving set to "true" for convenience sake

	string name = ""; // Initialize the variable "name"
	string passcode = "";

	// If skipLogin is false, fetch user password and store it as passcode
	if (skipLogin == false)
	{
		cout << "Enter Password: ";
		cin >> passcode;
	}

	system("CLS");

	if (passcode == pass || skipLogin) // If the password's correct or skipLogin is true, start the shell
	{
		cout << "What's your name? "; // Ask user for their name
		getline(cin >> ws, name);
		// cin >> name; // Store the users input into the variable "name"

		system("CLS"); // Clear console window

		int nameLen = name.length(); // Get the length of the users name so that the asterisk box can be scaled appropriately

		// cout << "Your name is " << nameLen << " characters long";

		// Old code used to display a properly sized box around the users name
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

		// Displays a neat banner when the program is started
		cout << "\t+-----------------------+\n";
		cout << "\t|                       |\n";
		cout << "\t|  SkyOS version 0.1.0  |\n";
		cout << "\t|                       |\n";
		cout << "\t+-----------------------+\n\n";

		// This boolean keeps track of whether the program's running. If it's set to false the program will terminate
		bool running = true;
		cout << "\tCurrent User: " << name << endl << endl;

		// Main program loop
		while (running == true)
		{

			SetConsoleTitle(TEXT("SkyOS - Command Line"));

			// This boolean is used mostly as a method of checking when the screen is refreshed. When it's true, the program will reload the banner shown on lines 655 > 659
			bool skipNewLine = false;

			// The three lines below get the users desired command and store it to the local variable "command"
			string command = "";
			cout << "> ";
			cin >> command;

			if (command == "shutdown") // When the user types "shutdown", set running to false. Setting running to false of course causes the program to terminate
			{
				running = false;
			}
			else if (command == "cls") // When the user types "cls", clear the console screen and ensure the banner is re-drawn
			{
				skipNewLine = true;
				system("CLS");
			}
			else if (command == "help") // Help command. Displays all functions I've finished.
			{
				system("CLS");
				SetConsoleTitle(TEXT("SkyOS - Help"));
				cout << "=====[ HELP ]=====\n\n";
				cout << "====[ COMMANDS ]====\n> shutdown - closes the program\n> cls - clears the consolescreen\n> exec - execute a program\n\t> calc - simple calculator\n\t> adventure - a small text adventure\n\t> console - starts the built-in command line environment\n\n";
				system("pause");
				skipNewLine = true;
				system("CLS");
			}
			else if (command == "exec") // Exec command. Allows users to access the main calculator program as well as the text adventure.
			{
				system("CLS");

				// Exec is effectively its own shell, start a loop to stop user from being booted back into the main shell.
				while (true)
				{
					SetConsoleTitle(TEXT("SkyOS - Execute Program"));

					cout << "=====[ EXECUTE PROGRAM ]=====\n\n"; // Sub-banner to show the programs title.

					// The variable "exec" is used to determine which program the user wants to run.
					string exec = "";
					cout << "Programs:\n\n> calc\n> adventure\n> optest\n> console\n\nProgram Name (type cancel to return to main command line): ";
					cin >> exec;

					system("CLS");

					if (exec == "cancel") // If the user types "cancel", break out of the exec function and return to main shell
					{
						break;
					}
					else if (exec == "calc") // Startup the calculator function when the user types "calc"
					{
						// Give calculator its own shell so it doesn't boot them after every function
						while (true)
						{
							SetConsoleTitle(TEXT("Calculator"));

							// The six lines below display the FUNCTIONS header, what functions there are and then get which one the user wants to do
							string func = "";
							cout << "===[ FUNCTIONS ]===\n";
							cout << "add | sub | div | mult | tri | crcl | evenodd | prime | getprimes | biggest | count | pow | sqrt | quadrat | exit\n";
							cout << "Desired Function: ";

							cin >> func;

							if (func == "add") // Basic add function. Gets the users first and second number and adds them together
							{
								SetConsoleTitle(TEXT("Calculator - ADD"));

								float numberA = 0;
								float numberB = 0;

								cout << "First Number: ";
								cin >> numberA;

								cout << "Second Number: ";
								cin >> numberB;

								float finalNum = numberA + numberB; // The actual math part of the function. I could do this in the cout statement but that looks messy

								cout << numberA << "+" << numberB << "=" << finalNum << "\n";

								system("pause");
								system("CLS");
							}
							else if (func == "biggest") 
							{
								vector<float> numbers = {};

								int numCount = 1;

								cout << "How many numbers do you want to compare? ";
								cin >> numCount;

								for (int i = 0; i < numCount; i++) 
								{
									cout << "Number [" << i << "]: ";
									float number = 0;
									cin >> number;

									numbers.push_back(number);
								}

								float largestNumber = -9999;
								int largestNumberIndex = 0;

								for (int i = 0; i < numbers.size(); i++) 
								{
									if (numbers[i] > largestNumber) 
									{
										largestNumberIndex = i;
										largestNumber = numbers[i];
									}
								}

								cout << "The largest number in your list is " << largestNumber << "! (numbers[" << largestNumberIndex << "])\n";
								system("pause");
								system("CLS");
							}
							else if (func == "sub") // Basic subtraction function. Gets the users first and second number and subtracts number 2 from number 1
							{
								SetConsoleTitle(TEXT("Calculator - SUBTRACT"));

								float numberA = 0;
								float numberB = 0;

								cout << "First Number: ";
								cin >> numberA;

								cout << "Second Number: ";
								cin >> numberB;

								float finalNum = numberA - numberB; // The actual math part of the function. I could do this in the cout statement but that looks messy

								cout << numberA << "-" << numberB << "=" << finalNum << "\n";

								system("pause");
								system("CLS");
							}
							else if (func == "div") 
							{
								SetConsoleTitle(TEXT("Calculator - DIVIDE"));

								float numberA = 0;
								float numberB = 0;

								cout << "First Number: ";
								cin >> numberA;

								cout << "Second Number: ";
								cin >> numberB;

								float finalNum = numberA / numberB;

								cout << numberA << "/" << numberB << "=" << finalNum << "\n";
								
								system("pause");
								system("CLS");
							}
							else if (func == "mult") 
							{
								SetConsoleTitle(TEXT("Calculator - MULTIPLY"));

								float numberA = 0;
								float numberB = 0;

								cout << "First Number: ";
								cin >> numberA;

								cout << "Second Number: ";
								cin >> numberB;

								float finalNum = numberA * numberB;

								cout << numberA << "*" << numberB << "=" << finalNum << "\n";

								system("pause");
								system("CLS");
							}
							else if (func == "tri") // Function that gets the area of a triangle. Two inputs for side one and two (starts with a square so only works with right angle triangles)
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

								float result = (sideA * sideB) / 2; // The actual math part of the function. I could do this in the cout statement but that looks messy
								cout << "(" << sideA << unit << "*" << sideB << unit << ")/2=" << result << unit;
								cout << "\n\nArea of Triangle is " << result << unit << "^2\n";

								system("pause");
								system("CLS");
							}
							else if (func == "crcl") // Basic function gets area of circle using pi*r^2 (written out in this program as pi*(r*r)
							{
								SetConsoleTitle(TEXT("Calculator - CALCULATE AREA OF CIRCLE"));

								string unit = "";
								float radius = 0;

								cout << "Target Units: ";
								cin >> unit;

								cout << "\nCircle Radius: ";
								cin >> radius;

								cout << "(pi*(" << radius << unit << "*" << radius << unit << ")=" << ((mathPi * radius) * radius) << unit; // Did the calculations in the cout statement to save time.
								cout << "\n\nArea of Circle is " << ((mathPi) * (radius * radius)) << unit << "\n";							// And again. VERY inefficient function >:L

								system("pause");
								system("CLS");
							}
							else if (func == "evenodd") // Simple function to get whether a number is odd or even
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
							else if (func == "prime") // Basic function to get whether a number is prime or not.
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
							else if (func == "getprimes") 
							{
								int startnum = 0;
								cout << "Start Number: ";
								cin >> startnum;

								getend:

								int endnum = 0;
								cout << "End Number: ";
								cin >> endnum;

								string filterPrimes;
								bool filter;

								cout << "Do you want to only see the prime numbers? Y/N: ";
								cin >> filterPrimes;

								if (filterPrimes == "y" || filterPrimes == "Y")
								{
									filter = true;
								}					
								else 
								{
									filter = false;
								}

								if (endnum <= startnum) 
								{
									cout << "You can't have an end number that is less or equal to the start number!";
									system("pause");
									system("CLS");
								}

								for (int num = startnum; num < endnum; num++) 
								{
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
										cout << "Number (" << num << ") is prime.\n";
									}
									else
									{
										if (filter == false)
											cout << "Number (" << num << ") is not prime.\n";
									}
								}

								system("pause");
								system("CLS");
							}
							else if (func == "pow") 
							{
								float a, b;
								cout << "Value A: ";
								cin >> a;

								cout << "Value B: ";
								cin >> b;

								cout << a << "^" << b << "=" << pow(a, b) << "\n";

								system("pause");
								system("CLS");
							}
							else if (func == "sqrt") 
							{
								float input = 0;
								cout << "Input Number: ";
								cin >> input;

								float result = sqrt(input);

								cout << "\nThe square root of " << input << " is " << result << endl;
								system("pause");
								system("CLS");
							}	
							else if (func == "quadrat") 
							{
								float a, b, c, x1, x2, discriminant, realPart, falsePart;
								cout << "Enter a, b, c\t";
								cin >> a >> b >> c;
								discriminant = b * b - 4 * a * c;

								if (discriminant > 0) // If the discriminant is more than zero, roots are real and different!
								{
									x1 = (-b + sqrt(discriminant)) / (2 * a); // Take negative value of b and add the square root of discriminant. Divide the result by 2*a
									x2 = (-b - sqrt(discriminant)) / (2 * a); // Take negative value of b and subtract the square root of discriminant. Divide the result by 2*a
									cout << "Roots are real and unequal.\n";
									cout << "x1 = " << x1 << ";\tx2 = " << x2 << endl; // Report the result of our calculations
								}
								else if (discriminant == 0) // If the discriminant is equal to zero, roots are real and same!
								{
									cout << "Roots are real and equal.\n";
									x1 = -b / (2 * a); // Take the negative value of be and divide it by the result of 2*a. I only need to do this for one because they're both the same!
									cout << "x1 = " << x1 << ";\tx2 =" << x1 << endl; // Report the answer to the user
								}
								else // If the discriminant is anything less than zero, roots are complex and different
								{
									realPart = -b/(2 * a); // Set the real part of the equasion to -b divided by the result of 2*a
									falsePart = sqrt(-discriminant) / (2 * a); // Set the "imaginary" or false part of the equasion to the square root of negative discriminant divided by the result of 2*a

									cout << "Roots are imaginary.\n";
									cout << "x1 = nan;\tx2 = nan\n"; // Report that x1 is equal to realPart + falsePart
								}
								system("pause");
								system("CLS");
							}
							else if (func == "count") 
							{
								system("CLS");

								float base = 0;
								float increment = 0;
								float max = 0;

								cout << "Base Number: ";
								cin >> base;

								redoinc:

								cout << "Increment Number: ";
								cin >> increment;

								if (increment <= 0) 
								{
									goto redoinc;
								}

								redomax:

								cout << "Max Number: ";
								cin >> max;

								if (max <= base + increment) 
								{
									cout << "Max number must be above " << base + increment;
								}

								for (int i = base; i < max; i += increment) 
								{
									cout << i << endl;
								}
							}
							else if (func == "exit")
							{
								break;
							}
						}
					}
					else if (exec == "adventure") // User clearly wants to play my AMAZING text adventure
					{
						SetConsoleTitle(TEXT("Text Adventure"));

						// *Note: Text adventure isn't amazing yet :l
						StartAdventure();
					}
					else if (exec == "159") 
					{
						cout << "What's the highest number you want to go to?";
						int highest = 0;
						cin >> highest;
						for (int i = 1; i < highest; i += 4) 
						{
							cout << i << ",\n";
						}
						cout << "\n";
					}
					else if (exec == "console") 
					{
						bool go = true;
						cout << "> ";
						system("CLS");
						SetConsoleTitle(TEXT("SkyOS - Command Line"));
						struct stat info;
					
						string bat = "";

						while (go == true)
						{
							cout << "> ";
							getline(cin >> ws, bat);

							char* arr;
							arr = &bat[0];

							if (bat == "shutdown") 
							{
								go = false;							
							}
							else if (bat == "writecode") 
							{
								string fullBat = "";
								vector<string> file{};
								string line = "";

								bool doCode = true;

								while (doCode == true) 
								{
									system("CLS");

									cout << "Commands: save | open | insertline | editline | removeline | exit\n\n";

									cout << fullBat;

									cout << file.size() << " > ";

									getline(cin >> ws, line);

									if (line == "save")
									{
										system("CLS");
										cout << "=====[ SAVE FILE ]=====\n";
										fullBat = "";

										for (int i = 0; i < file.size(); i++) 
										{
											fullBat.append(file[i]).append("\n");
										}

										cout << "\nFile Name: ";

										string nam = "";
										cin >> nam;

										ofstream batFile(nam + ".bat");
										batFile << fullBat;
										batFile.close();

										doCode = false;
									}
									else if (line == "help") 
									{
										system("CLS");
										cout << "=====[ HELP ]=====\n\n";
										system("help");
										cout << "\n";
										system("pause");
										system("CLS");
									}
									else if (line == "open") 
									{
										system("CLS");
										cout << "=====[ OPEN FILE ]=====\n";
										cout << "\nFile Name: ";
										string f = "";
										cin >> f;

										ifstream instream(f);
										string l;

										file.clear();

										if (instream.is_open()) 
										{
											while (getline(instream,l)) 
											{
												file.push_back(l);
											}
											instream.close();
										}

										fullBat = "";

										int cap = file.size();

										for (int i = 0; i < cap; i++)
										{
											fullBat.append(to_string(i)).append(" > ").append(file[i]).append("\n");
										}
									}
									else if (line == "insertline") 
									{
										system("CLS");
										cout << "=====[ INSERT LINE ]=====\n";
										cout << "\nWhere are you inserting a line? ";

										int line = 0;
										cin >> line;

										if (line >= file.size()) 
										{
											cout << "Line number must be less than " << (file.size() - 1) << "!\n";
										}

										cout << endl;
										string newLine = "";
										getline(cin >> ws, newLine);

										auto pos = file.begin() + line;

										file.insert(pos, newLine);

										fullBat = "";

										int cap = file.size();

										for (int i = 0; i < cap; i++)
										{
											fullBat.append(to_string(i)).append(" > ").append(file[i]).append("\n");
										}
									}
									else if (line == "removeline") 
									{
										system("CLS");
										cout << "=====[ REMOVE LINE ]=====\n";

										cout << "\nWhich line do you want to remove? ";
										int line = 0;
										cin >> line;

										if (line >= file.size())
										{
											cout << "Line number must be less than " << (file.size() - 1) << "!\n";
										}

										auto pos = file.begin() + line;

										file.erase(pos);

										fullBat = "";

										int cap = file.size();

										for (int i = 0; i < cap; i++)
										{
											fullBat.append(to_string(i)).append(" > ").append(file[i]).append("\n");
										}
									}
									else if (line == "editline") 
									{
										system("CLS");
										cout << "=====[ EDIT LINE ]=====\n";

									lineSelect:

										cout << "\nWhich line are you editing? ";
																				
										int line = 0;
										cin >> line;

										if (line >= file.size()) 
										{
											cout << "Line number must be less than " << (file.size() - 1) << "!\n";
											goto lineSelect;
										}

										cout << "Old Line:\n" << file[line] << endl;
										cout << "New Line:\n";
										
										string newLine = "";
										getline(cin >> ws, newLine);

										file[line] = newLine;

										fullBat = "";

										int cap = file.size();

										for (int i = 0; i < cap; i++)
										{
											fullBat.append(to_string(i)).append(" > ").append(file[i]).append("\n");
										}
									}
									else if (line == "exit") 
									{
										system("CLS");
										cout << "=====[ EXIT PROGRAM ]=====\n";
										cout << "\nDo you want to save your code? Y/N\n";
										string save = "";
										cin >> save;

										if (save == "Y") 
										{
											fullBat = "";

											for (int i = 0; i < file.size(); i++)
											{
												fullBat.append(file[i]).append("\n");
											}

											cout << "\nFile Name: ";

											string nam = "";
											cin >> nam;

											ofstream batFile(nam + ".bat");
											batFile << fullBat;
											batFile.close();
										}

										doCode = false;
									}
									else 
									{
										file.push_back(line);
										fullBat = "";

										int cap = file.size();

										for (int i = 0; i < cap; i++) 
										{
											fullBat.append(to_string(i)).append(" > ").append(file[i]).append("\n");
										}
									}
								}
							}
							else if (bat == "help") 
							{
								cout << "\n=====[ Base Commands ]=====\n\n";
								system("help");
								cout << "\n=====[ SkyOS Commands ]=====\n\nSHUTDOWN\tTerminates the Command Line\nWRITECODE\tOpens a code editor for .bat files\n\n";
							}
							else
							{
								system(arr);
							}
						}
					}
					else
					{
						cout << "Unknown Program \"" << exec << "\"\n"; // When the user types an unknown program, tell them they messed up and try again.
						system("pause");
						system("CLS");
					}
				}

				// These lines are only called after the exec while(true) loop is done
				cout << "Returning to command line...\n\n";
				system("pause");
				skipNewLine = true; // Re-draws the banner.
				system("CLS");
			}
			else if (command == "crt") // Hidden function designed to allow users to replicate the feeling of using a monochrome CRT monitor
			{
			colourScreen :
				cout << "Which colour style would you like to use? (Type 'default' or 'amber' or 'sulfer' or 'bsod')\n> ";
				string colour = "default";
				cin >> colour;

				if (colour == "default") 
				{
					system("Color 0F"); // Default is a black background with white text
				}
				else if (colour == "amber") 
				{
					system("Color 06"); // Amber is a black background with yellow text
				}
				else if (colour == "sulfer") 
				{
					system("Color 02"); // Sulfer is a black background with green text
				}
				else if (colour == "bsod") 
				{
					system("Color 17");
				}
				else if (colour == "custom") 
				{
					cout << "Pick two colours (use the letters that appear before the colour name): \n";
					cout << "[1] Blue\n";
					cout << "[2] Green\n";
					cout << "[3] Aqua\n";
					cout << "[4] Red\n";
					cout << "[5] Magenta\n";
					cout << "[6] Yellow\n";
					cout << "[7] White\n";
					cout << "[8] Grey\n";
					cout << "[9] Light Blue\n";
					cout << "[0] Black\n";
					cout << "[A] Light Green\n";
					cout << "[B] Light Aqua\n";
					cout << "[C] Light Red\n";
					cout << "[D] Light Magenta\n";
					cout << "[E] Light Yellow\n";
					cout << "[F] Bright White\n";
					char c[2] = {'E','E'};
					cout << "Colour (BackgroundForeground): ";
					cin >> c;

					string f = "color ";

					f.append(c);

					system(f.c_str());

					colour = f;
				}
				else 
				{
					cout << "Unrecognized colour scheme \"" << colour << "\"";
					goto colourScreen;
				}

				ofstream colourSav("colour.pref");

				if (colourSav.is_open()) 
				{
					colourSav << colour;
					colourSav.close();
				}
			}
			else if (command == "func") // This is my experimentation area. Just like crt, this is hidden from the help text
			{
				cout << "Enter function:\n";
				string command = "";
				cin >> command;

				if (command == "echo") // Echo (as the name suggests), reads the whole line of text a user inputs, and repeats it.
				{
					string value;
					cout << "echo[0] = ";
					getline(cin >> ws, value);
					cout << "\n\n" << value << "\n\n";
				}
			}
			else if (command == "bob") 
			{
				cout << "Jews in the gas chambers:   Hitler:\n\n";
				cout << "       ###  ####         ||         ###  ####      \n";
				cout << "      #'#'#    ###       ||        #.#.#    ###    \n";
				cout << "      ##^##      ###     ||  Bye > ##u##      ###  \n";
				cout << "       ###   #######     ||         ###   #######  \n";
				cout << "     #########           ||       #########        \n";
				cout << "    #########            ||      #########         \n";
				cout << "    # ########           ||      # ########        \n";
				cout << " # #   #######           ||     #   #######        \n";
				cout << " ###   #######           ||     #   #######        \n";
				cout << "        ######           ||     #    ######        \n";
				cout << "        ########         ||     #    ########      \n";
				cout << "       #########         ||         #########      \n";
				cout << "       ###    ###        ||         ###    ###     \n";
				cout << "       ##      ##        ||         ##      ##     \n";
			}
			else if (command == "bitch") 
			{
			cout << "\n";
			cout << "                               ..,**////*,...             \n";
			cout << "                          ..,*/#%&@@@&&&&&&%(,....        \n";
			cout << "                        .*/(%&&@@&&&@@@@@@@@@&%#/,......    \n";
			cout << "                      ..*#%&&&&@&&&&&%###%&&@&&&@@&%#/*...  \n";
			cout << "                      .*(&&&@@@&%%#(((////(#%&&@@@@&&&#/*,...\n";
			cout << "                      .*#&&&&@@&#((//////////(#%&&@@@@&&#(***,..\n";
			cout << "                   .,.*(%%%&@&%%#((///***/(#%%%%%%%&&&&&&%#(*,,..\n";
			cout << "                   ./(%%%%&&@&&&%%%%#/*/%#((///*,,(%@@@&&&&%(/*,...\n";
			cout << "                  .*(%&@@@&&&%#(((///(#(%#%%#(((((/(%@@@&@@&%(/,... \n";
			cout << "                  .,(%&@@&@&%(((##((#%%((((##((#(**/#&@&@@@@@&#/,.  \n";
			cout << "                 ..*(%&%%&@%(/((//(%&#//***/((/*,**//#&@@@@&&%%#*.    \n";
			cout << "                 ./#&&&&&@@&#,,,*/**/###(/////(((***//#&@&&##(//,.        \n";
			cout << "                 ,(#%%%&@@@&#*,**/((%%%%%####%##(/***(%&&&&#((/*,.          \n";
			cout << "                .*(%%%%&@@@&#,**///(%#/(((/***/((////#&@@@@@&%#/..         \n";
			cout << "                 .,**(#%%%&&#***/((#(///(((/////////(%@@@@@@&%#*           \n";
			cout << "                 .*#%&&&&&&&&#/(//(((///((//////////#&@@@&&&/,..             \n";
			cout << "                   .*(#%&&&&@&(//((((((((((/////////#&@@@&&#*,..             \n";
			cout << "                   .*/////(%&&&%##(((((((((///****/(#%&@&&&&%/,.             \n";
			cout << "                .,,,*/(###(///(#%%####(///********/((###%&&&%(///*,.         \n";
			cout << "             .,,,***//*********/((###(////********//((%##&&(/**/***///*,.     \n";
			cout << "          ..,,,******************///((((/*********///(/////**************,.   \n";
			cout << "       .....,***//******,,,**********//(//*******////**********************..   \n";
			cout << "     .....,,,**//****************************////**/************************,.  \n";
			cout << "   ......,***//*****/***/******************/(//////**************************,. \n";
			cout << "  .....,,****//*****/*///**************,*******///****************************, \n";
			cout << "  ....,,**/////*****(/(#************************/*****/************************.\n";
			cout << " .,,,.***///((*****//(#(**************************,*/**************************.\n";
			cout << " **,,,*((///(#/,***/%%(******//*******************************************//*/*,\n";
			cout << ".**//,*/(///##/**/(&#//////**********************************************/(/***,\n";
			cout << ",**(#**/////##///#%(((((///////*****************************************/(/*****\n";
			cout << "***/#(**/(//##//(#%%%%#(////**********************,***********************///***\n";
			}
			else if (command == "choochoo") 
			{
				cout << "			                      *.&&%%										\n";
				cout << "                *&,*###########(/#/((%(@&*&&/(									\n";
				cout << "             &&&&&&&&%&#&&&&&&&&%&%%****(**///((#@&%/%/@@@@@@@@@@              \n";
				cout << "           @@@@@@@@@%%&&@@&&@&&@@&@/***/**/*///#/%((@@*/*//#@@@@@@%@&(#((/     \n";
				cout << "          @@@@@@@@@@@@@@@@@@%@@@@@//*/**/******@(/(/(@@#//**/@@@@@(#/**(*((.   \n";
				cout << "         @@@@@@@@@@@@@@@@@@@@@@@@(///////*///**@#(#(##@****,//@@@@%%##/(//(/   \n";
				cout << "         @@@@@@@@@@@@@@@@@@@@@@@@*(((#(///*//**#&%##*@@@#(@*#((@@@@#(@/////(   \n";
				cout << "         /@%(@@@@@@(@@@@@@@@@@@@@@@@@@@#@@@@@%#@/(@#%/#(*(&%***@@@&%(@((((#(   \n";
				cout << "         *@@@@@@@@@%@@@@@@@@@@&@#/#@@@/#((@#@@@%%%%&&@@&%&@&&%#&%&&%%#@@#(&@&  \n";
				cout << "     @@@@@@@@@@@@@@@@@@@@@%@@@@@####@@%%%@,#,@@@@@/%//*///*///&#(*/(@@@@@*@@/@ \n";
				cout << "     @@@@@@@@@@@@@@@@@@@@@@@@@@&#(@##@##@(%@@%*&@@%@%%@@@@@@%/#@&@/*%@(%@(@%,  \n";
				cout << "     @@@@@@@@@@@@@@@@@@@%&@@@@&%%##&&(@&(/@&&@(@@@@&/@@@@@@(@*#@#%#@&@#@. *    \n";
				cout << "           @@@@@@@@ @@@@*%&#&%&%@&@(/ %** @&.@   @.                            \n";
				cout << "           *@@@       .@.";
			}
			else if (command == "321fuck") 
			{
				PlaySound(TEXT("321fuck.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			else if (command == "321fuckCOLOR") 
			{
				PlaySound(TEXT("321fuck.wav"), NULL, SND_FILENAME |  SND_ASYNC);

				int counter = 0;

				int c = false;

				while (true) 
				{
					counter++;

					if (c == 0) 
					{
						system("color 9D");
						c++;
					}
					else  if (c == 1)
					{
						system("color 73");
						c++;
					}
					else if (c == 2) 
					{
						system("color A0");
						c++;
					}
					else if (c == 3) 
					{
						system("color 86");
						c++;
					}
					else if (c == 4) 
					{
						system("color 5C");
						c++;
					}
					else if (c == 5) 
					{
						system("color F2");
						c = 0;
					}

					if (counter > 260000) 
					{
						break;
					}
				}
			}
			else if (command == "ourlives") 
			{
				PlaySound(TEXT("ourlives.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			else if (command == "soundstop") 
			{
				PlaySound(NULL, 0, 0);
			}
			else
			{
				// When a user puts in an unknown command, tell them off and tell them to use the help command for a list of commands
				cout << "Unknown command \"" << command << "\". Type \"help\" for a list of all available commands!";
			}

			if (!skipNewLine) // When skipNewLine is false, add a new line and ask for another command
				cout << "\n";
			else 
			{	// When skipNewLine is true, draw the banner and ask for another command
				cout << "\t+-----------------------+\n";
				cout << "\t|                       |\n";
				cout << "\t|  SkyOS version 0.1.0  |\n";
				cout << "\t|                       |\n";
				cout << "\t+-----------------------+\n\n";
				cout << "\tCurrent User: " << name << endl << endl;
			}
		}
	}
	else
	{
		cout << "Invalid Password! Terminating Program...";
	}
}