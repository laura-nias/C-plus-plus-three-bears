//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c ? group assignment
//Last updated: 24 February 2017
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <thread>
using namespace std;

//include our own libraries
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int  SIZEX(16);    	//horizontal dimension
const int  SIZEY(11);		//vertical dimension
							//defining symbols used for display of the grid and content
const char BEAR('@');   	//bear
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char BOMB('B');		//bomb
const char DETONATOR('T');  //detonator
const char KEY('K');
const char LOCK('L');
const char EXIT('X');
//defining the command letters to move the bear on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
							//defining the other command letters
const char QUIT('Q');		//to end the game
const char CHEAT('C');

struct Item {
	int x, y;
	char symbol;
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	void playerSetup(string&);
	void mainMenu();
	void endProgram();

	//local variable declarations 
	string playerName;

	//action...
	playerSetup(playerName);
	mainMenu();

	endProgram();						//display final message
	return 0;
}

void runProgram(int levelNo)
{
	//function declarations (prototypes)
	void playerSetup(string&);
	void mainMenu();
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], vector<Item>& bears, vector<Item>& bombs, int levelNo);
	void paintGame(const char g[][SIZEX], string mess, int moves, int bearsSaved, string);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int key);
	bool wantsToCheat(const int key);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], vector<Item>& bears, vector<Item>& bombs, Item detonator, Item lockKey, Item lock, const int key, string& mess, int& moves, bool& gameOver, int& bearsSaved, bool& bombsActive, bool& hasKey, string, int& levelNo);
	void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const vector<Item>& bears, const vector<Item>& bombs);
	void enableCheatMode(int& moves, string& mess, bool& bombsActive, bool& cheatActive, vector<Item>& bombs);
	void timer();
	void endProgram();

	//local variable declarations 
	string playerName;
	char grid[SIZEY][SIZEX];	//grid for display
	char maze[SIZEY][SIZEX];	//structure of the maze

	Item bear = { 0, 0, BEAR };
	vector<Item> bears = { bear, bear, bear };

	Item bomb = { 0, 0, BOMB };
	vector<Item> bombs = { bomb, bomb, bomb, bomb, bomb };

	Item detonator = { 0, 0, DETONATOR };

	Item lockKey = { 0, 0, KEY };

	Item lock = { 0, 0, LOCK };

	string message("LET'S START...");	//current message to player
	int moves(0); //count each move taken
	int bearsSaved(0);
	bool gameOver(false);
	bool validMovement(false);
	bool bombsActive = true;
	bool cheatActive = false;
	bool hasKey = false;

	initialiseGame(grid, maze, bears, bombs, levelNo);	//initialise grid (incl. walls & bear)
	paintGame(grid, message, moves, bearsSaved, playerName);			//display game info, modified grid & messages
	// thread t1(timer);
	int key(getKeyPress()); 			//read in  selected key: arrow or letter command
	while ((!wantsToQuit(key)) && (!gameOver))		//while user does not want to quit
	{
		if (isArrowKey(key))
		{
			updateGameData(grid, bears, bombs, detonator, lockKey, lock, key, message, moves, gameOver, bearsSaved, bombsActive, hasKey, playerName, levelNo);		//move bear in that direction
			updateGrid(grid, maze, bears, bombs);									//update grid information
		}
		else if (wantsToCheat(key))
		{
			enableCheatMode(moves, message, bombsActive, cheatActive, bombs);
			moves = 500;
		}
		else
		{
			message = "INVALID KEY!";				//set 'Invalid key' message
		}

		paintGame(grid, message, moves, bearsSaved, playerName);			//display game info, modified grid & messages

		if (!gameOver)
		{
			key = getKeyPress(); 			//display menu & read in next option
		}
	}

	if (wantsToQuit(key))
	{
		message = "GAME STOPPED BY PLAYER";
		paintGame(grid, message, moves, bearsSaved, playerName);
	}
}

void mainMenu()
{
	void runProgram(int levelNo);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void menuInput(char& choice);
	void endProgram();

	int levelNo(1);
	char choice(' ');
	bool correctChoice(false);

	showMessage(clGrey, clYellow, 20, 4, "                      The Three Bears                        ");
	showMessage(clGrey, clYellow, 20, 5, "                         Main Menu                           ");
	showMessage(clGrey, clYellow, 20, 6, "                       A: Game Start                         ");
	showMessage(clGrey, clYellow, 20, 7, "                       B: High Scores                        ");
	showMessage(clGrey, clYellow, 20, 8, "                       C: Quit Game                          ");
	showMessage(clBlack, clWhite, 20, 9, "                              ");

	menuInput(choice);

	while (!correctChoice)
	{
		switch (choice)
		{
		case 'A':
		{
			correctChoice = true;
			Clrscr();
			runProgram(levelNo);
		} break;
		case 'B':
		{
			correctChoice = true;
			Clrscr();
		} break;
		case 'C':
		{
			correctChoice = true;
			Clrscr();
		} break;
		default:
		{
			showMessage(clBlack, clWhite, 20, 11, "               Please enter a correct option. ");
			showMessage(clBlack, clWhite, 20, 9, "                              ");
			menuInput(choice);
		} break;
		}
	}
}

void menuInput(char& choice)
{
	cin >> choice;
	choice = toupper(choice);

	if (choice == !'A' || choice == !'B' || choice == !'C')
	{
		cin >> choice;
		choice = toupper(choice);
	}
}

void playerSetup(string& playerName)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	showMessage(clGrey, clYellow, 20, 4, "                        The Three Bears                      ");
	showMessage(clGrey, clYellow, 20, 5, "  Group 1XD - Joshua Edwards, Laura Nias and Alexander Elgy  ");
	showMessage(clGrey, clYellow, 20, 6, "           BSci Game Software Development - 2016-2017        ");
	showMessage(clBlack, clWhite, 20, 8, "Enter your name: ");
	cin >> playerName;
	Clrscr();
}

void playerWin(string playerName, int moves)
{
	ofstream fout;
	ifstream fin(playerName + ".txt");
	if (fin.fail())
	{
		fout.open(playerName + ".txt", ios::out);
		fout << moves;
		fout.close();
	}
	else
	{
		fin.close();
	}
	fin.open(playerName + ".txt");
	int oldScore;
	fin >> oldScore;
	fin.close();

	if (moves < oldScore)
	{
		fout.open(playerName + ".txt", ios::out);
		fout << moves;
		fout.close();
	}
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], vector<Item>& bears, vector<Item>& bombs, int levelNo)
{ //initialise grid & place bear in middle
	void setInitialMazeStructure(char maze[][SIZEX], int levelNo);
	void setInitialDataFromMaze(char maze[][SIZEX], vector<Item>& bears, vector<Item>& bombs);
	void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const vector<Item>& bears, const vector<Item>& bombs);

	setInitialMazeStructure(maze, levelNo);		//initialise maze
	setInitialDataFromMaze(maze, bears, bombs);	//initialise bear's position
	updateGrid(grid, maze, bears, bombs);		//prepare grid
}

void setInitialMazeStructure(char maze[][SIZEX], int levelNo)
{
	ifstream level;
	string line;
	level.open("Levels\\level" + to_string(levelNo) + ".txt");
	if (level.is_open())
	{
		for (int row(0); row < SIZEY; ++row)
		{
			getline(level, line);
			for (int col(0); col < SIZEX; ++col)
			{
				int current = line[col] - '0';
				switch (current)
				{
				case 0: maze[row][col] = TUNNEL; break;
				case 1: maze[row][col] = WALL; break;
				case 2: maze[row][col] = BEAR; break;
				case 3: maze[row][col] = BOMB; break;
				case 4: maze[row][col] = DETONATOR; break;
				case 5: maze[row][col] = KEY; break;
				case 6: maze[row][col] = LOCK; break;
				case 7: maze[row][col] = EXIT; break;
				}
			}
		}
	}
}


void setInitialDataFromMaze(char maze[][SIZEX], vector<Item>& bears, vector<Item>& bombs)
{ //extract bear's coordinates from initial maze info
	int bearNo(0);
	int bombNo(0);
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (maze[row][col])
			{
			case BEAR:
			{
				bears[bearNo].x = col;
				bears[bearNo].y = row;
				maze[row][col] = TUNNEL;
				bearNo++;
			}
			break;
			case BOMB:
			{
				bombs[bombNo].x = col;
				bombs[bombNo].y = row;
				maze[row][col] = TUNNEL;
				++bombNo;
			}
			break;
			}
}

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const vector<Item>& bears, const vector<Item>& bombs)
{ //update grid configuration after each move
	void setMaze(char g[][SIZEX], const char m[][SIZEX]);
	void placeBear(char g[][SIZEX], const vector<Item>& bears);
	void placeBomb(char g[][SIZEX], const vector<Item>& bombs);

	setMaze(grid, maze);	//reset the empty maze configuration into grid
	placeBomb(grid, bombs);
	placeBear(grid, bears);	//set bear in grid
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeBear(char g[][SIZEX], const vector<Item>& bears)
{
	for (int bearNo(0); bearNo < bears.size(); ++bearNo)
	{
		g[bears[bearNo].y][bears[bearNo].x] = bears[bearNo].symbol;
	}
}

void placeBomb(char g[][SIZEX], const vector<Item>& bombs)
{
	for (int bombNo(0); bombNo < bombs.size(); ++bombNo)
	{
		g[bombs[bombNo].y][bombs[bombNo].x] = bombs[bombNo].symbol;
	}
}

//---------------------------------------------------------------------------
//----- move the bear
//---------------------------------------------------------------------------
void updateGameData(const char g[][SIZEX], vector<Item>& bears, vector<Item>& bombs, Item detonator, Item lockKey, Item lock, const int key, string& mess, int& moves, bool& gameOver, int& bearsSaved, bool& bombsActive, bool& hasKey, string playerName, int& levelNo)
{
	//move bear in required direction
	bool isArrowKey(const int k);
	void mainMenu();
	void runProgram(int levelNo);
	void playerWin(string, int);
	void setLevelTwo(char maze[][SIZEX]);
	void setKeyDirection(int k, int& dx, int& dy);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	assert(isArrowKey(key));
	bool validMovement(false);
	string messErase = "                                         ";
	//reset message to blank
	mess = messErase;		//reset message to blank

							//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);
	int countBear = 0;
	//check new target position in grid and update game data (incl. bear coordinates) if move is possible
	for (size_t bearNo(0); bearNo < bears.size(); ++bearNo)
	{
		switch (g[bears[bearNo].y + dy][bears[bearNo].x + dx])
		{			//...depending on what's on the target position in grid...
		case TUNNEL:		//can move
			bears[bearNo].y += dy;	//go in that Y direction
			bears[bearNo].x += dx;	//go in that X direction
			if (!validMovement)
			{
				moves++;
				validMovement = true;
			}
			break;
		case WALL:  		//hit a wall and stay there
			countBear++;
			if (countBear == bears.size())
			{
				cout << '\a';	//beep the alarm
				mess = messErase;
				mess = "CANNOT GO THERE!";
				countBear = 0;
			}
			break;
		case BOMB:
			bears[bearNo].y += dy;
			bears[bearNo].x += dx;
			if (bombsActive)
			{
				cout << '\a';
				moves++;
				mess = messErase;
				mess = "BOOM!";
				bears.clear();
				showMessage(clWhite, clBlack, 40, 16, "GAME OVER!!     ");
				gameOver = true;
			}
			break;
		case DETONATOR:
			bears[bearNo].y += dy;
			bears[bearNo].x += dx;
			if (bombsActive)
			{
				bombs.clear();
				mess = messErase;
				mess = "BOMBS DEFUSED!      ";
			}
			break;
		case KEY:
			bears[bearNo].y += dy;
			bears[bearNo].x += dx;
			mess = "THE LOCK IS NOW OPEN!";
			hasKey = true;
			break;
		case LOCK:
			if (hasKey == false)
			{
				mess = "YOU NEED TO FLICK THE SWITCH!";
			}
			else
			{
				bears[bearNo].y += dy;
				bears[bearNo].x += dx;
			}
			break;
		case EXIT:
			mess = messErase;
			mess = "BEAR HAS ESCAPED!";
			bears.erase(bears.begin() + bearNo);
			bearsSaved++;
			bearNo--;
			if (bearsSaved == 3)
			{
				mess = messErase;
				mess = "GAME WON!\n ";
				playerWin(playerName, moves);
				Clrscr();
				if (levelNo == 2)
				{
					mainMenu();
				}
				else 
				{
					levelNo++;
					runProgram(levelNo);
				}
			}
			break;
		case BEAR:
			switch (g[bears[bearNo].y + (dy * 2)][bears[bearNo].x + (dx * 2)])
			{
			case WALL:
				break;
			case BEAR:
				switch (g[bears[bearNo].y + (dy * 3)][bears[bearNo].x + (dx * 3)])
				{
				case WALL:
					break;
				default:
					bears[bearNo].y += dy;
					bears[bearNo].x += dx;
					if (!validMovement)
					{
						moves++;
						validMovement = true;
					}
					break;
				}
				break;
			default:
				bears[bearNo].y += dy;
				bears[bearNo].x += dx;
				if (!validMovement)
				{
					moves++;
					validMovement = true;
				}
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		break;
	case UP: 		//when UP arrow pressed...
		dx = 0;
		dy = -1;	//decrease the Y coordinate
		break;
	case DOWN: 		//when DOWN arrow pressed...
		dx = 0;
		dy = +1;	//increase the Y coordinate
		break;
	}
}

int getKeyPress()
{ //get key or command (in uppercase) selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return toupper(keyPressed);		//return it in uppercase 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}

bool wantsToCheat(const int key)
{
	return toupper(key) == CHEAT;
}

bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')
	return toupper(key) == QUIT;
}

//---------------------------------------------------------------------------
//----- Cheat mode
//---------------------------------------------------------------------------
void enableCheatMode(int& moves, string& mess, bool& bombsActive, bool& cheatActive, vector<Item>& bombs)
{
	cheatActive = !cheatActive;
	if (cheatActive)
	{
		bombsActive = false;
		cout << "\nCHEAT MODE ON!\n";
		Beep(1000, 500); //1000 hertz for half a second
		Beep(1000, 500);
		Beep(1000, 500);
	}
	else
	{
		bombsActive = true;
		cout << "\nCHEAT MODE OFF\n";
	}
}

//---------------------------------------------------------------------------
//----- Timer
//---------------------------------------------------------------------------
void timer()
{
 int seconds = 60;

 for (int sec = 59; sec >= 0; sec--)
 {
	 if (sec == 0)
	 {
		 seconds = 59;
	 }

	 Sleep(1000);

	 cout << "\rTime remaining: " << sec;
 }
 Sleep(1000);
 cout << "TIME OVER, YOU LOST" << endl;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------
string tostring(char x) {
	std::ostringstream os;
	os << x;
	return os.str();
}

string tostring(int x) {
	std::ostringstream os;
	os << x;
	return os.str();
}

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message)
{
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}

void paintGame(const char g[][SIZEX], string mess, int moves, int bearsSaved, string playerName)
{ //display game title, messages, maze, bear and other items on screen
	string tostring(int x);
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);

	void paintGrid(const char g[][SIZEX]);

	//display game title
	showMessage(clGrey, clYellow, 0, 0, "Three Bears Game");
	showMessage(clBlack, clYellow, 0, 1, "RESCUED: ");
	showMessage(clBlack, clYellow, 40, 0, "FoP Task 1c: February 2017");

	//display menu options available
	showMessage(clBlack, clYellow, 40, 2, "PLAYER: " + playerName);
	showMessage(clBlack, clYellow, 40, 3, "NUMBER OF MOVES: " + tostring(moves));
	showMessage(clBlack, clYellow, 40, 4, "NUMBER OF BEARS RESCUED: " + tostring(bearsSaved));
	showMessage(clBlack, clYellow, 40, 5, "");
	showMessage(clBlack, clYellow, 40, 6, "GAME RULES: ");
	showMessage(clBlack, clYellow, 40, 7, "Get the three bears to the exit");
	showMessage(clBlack, clYellow, 40, 8, "by diffusing the bombs.");
	showMessage(clBlack, clYellow, 40, 9, "To diffuse bombs reach the detonator 'T'");
	showMessage(clBlack, clYellow, 40, 10, "");
	showMessage(clBlack, clYellow, 40, 11, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clBlack, clYellow, 40, 12, "TO QUIT ENTER 'Q'           ");
	showMessage(clBlack, clYellow, 40, 13, "TO ENTER CHEAT MODE ENTER 'C' ");

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 15, mess);	//display current message

													// display grid contents
	paintGrid(g);

	showMessage(clGrey, clWhite, 0, 15, "BEAR      " + tostring(BEAR));
	showMessage(clGrey, clWhite, 0, 16, "BOMB      " + tostring(BOMB));
	showMessage(clGrey, clWhite, 0, 17, "DETONATOR " + tostring(DETONATOR));
	showMessage(clGrey, clWhite, 0, 18, "EXIT      " + tostring(EXIT));
	showMessage(clBlack, clWhite, 0, 19, "\n");
}

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
			cout << g[row][col];	//output cell content
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	showMessage(clBlack, clWhite, 40, 20, "");	//hold output screen until a keyboard key is hit
	system("pause");
}