#include "windows.h"
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <ios>
#include <cstdlib>

// Define ACII code for arrows and colors
#define KEY_LEFT 75
#define KEY_RIGHT 77 
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

// Defining and initializing conditions for the race track
int Xlngt = 12;
int XlngtCrd = 57;

//Class for changing Colors
class TextAttr {
	friend std::ostream& operator<<(std::ostream& out, TextAttr attr)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr.value);
		return out;
	}
public:
	explicit TextAttr(WORD attributes) : value(attributes) {}
private:
	WORD value;
};

// Mechanics of changing the dificulty level
int Difi=1;
int DifiLevl;
char Dificlty;

// Player Name
char ChrPlyr[]="Player 1";

// Declaring Functions
DWORD WINAPI ThreadFn(LPVOID vpParam);
void gotoxy(int x, int y);
void drawp(int x, int y, int Cr);
void drawCar(int x, int y, int Cr);
void resetb();
int getCar();
void Intro();
int Exit();
void LogHighScore(char name[], int score);
void readScore();
void VarIniz();
char chrGetKey;

//Declaring Array for race track
int ArryDraw[12][26][2];
int Incremt = 0, BolGamestatue = 1, XcordDrw = 0, XcordComptrCar, XcordComptrCar1, XcordComptrCar2;
int score = 1, level = 1, delay = 85;
using namespace std;
void VarIniz() {
	score = 1; level = 1;
	Incremt = 0; BolGamestatue = 1; XcordDrw = 0;
	
}

int main() {
StructOutr:system("cls");

//Konfigurowanie Œrodowiska Gry Setting up the game environment
Intro();
VarIniz();
system("cls");
	
DWORD qThreadID;
HANDLE hThread = CreateThread(0, 0, ThreadFn, 0, 0, &qThreadID); 

//Welcome Screen
	gotoxy(44, 2);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE) << "C++ Racing Game";
	gotoxy(44, 4);
	cout << TextAttr FOREGROUND_WHITE << "Control";
	gotoxy(44, 5);
	cout << "========";
	gotoxy(44, 6);
	cout << "[A]=Left  or Arrow Left  [<-]";
	gotoxy(44, 7);
	cout << "[D]=Right or Arrow Right [->]";
	
//Displaying the timer before game started
for (int i = 5,j=0; i >= 0; i--,j++) {  

	gotoxy(44, 8);
	cout << i;
	gotoxy(45+j, 8);
	cout << "*";
	gotoxy(51, 15);
	Sleep(1000);

	}
		system("cls");
		
//Game started
while (BolGamestatue) {
	
	gotoxy(59, 4);
	cout << "Score:" << score;
	gotoxy(59, 5);
	cout << "Level:" << level;

	resetb();

//Drawing template of Computer Controlled Cars
	drawCar(XcordComptrCar, Incremt, 0);

	if (Difi==1) {
		drawCar(XcordComptrCar1, Incremt, 0);
	}else if (Difi == 2) {
		drawCar(XcordComptrCar1, Incremt, 0);
		drawCar(XcordComptrCar2, Incremt, 0);
	}

//Drawing template of Player Control Car
	drawCar(XcordDrw, 21, 1);
	for (int j = 0; j<26; j++) {

//Displaying Left side of a race track
	gotoxy(44, j + 1);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << "S";
	cout << TextAttr(FOREGROUND_WHITE);
	gotoxy(45, j + 1);

//Print the movement of the computer control cars
	for (int i = 0; i<Xlngt; i++) {

	if (ArryDraw[i][j][0] == 1) {
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << "0";
	cout << TextAttr(FOREGROUND_WHITE);
	}
	else if (ArryDraw[i][j][1] == 1) {
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE) << "0";
	cout << TextAttr(FOREGROUND_WHITE);
	}
	else {
			cout << " ";
		}
	}

//Displaying Right side of the race track
gotoxy(XlngtCrd, j + 1);
cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << "S";
cout << TextAttr(FOREGROUND_WHITE);

//Cars Collision Mechanic
if (Incremt >= 21 && (XcordDrw == XcordComptrCar || XcordDrw == XcordComptrCar1 || XcordDrw == XcordComptrCar1)) {

	BolGamestatue = 0;

		}
}
Incremt++;

//Scoring and designating tracks for computer controlled Cars
if (Incremt == 25) {
	Incremt = -4;
	XcordComptrCar = getCar();
	int RndCord = XcordComptrCar;
	while (XcordComptrCar == RndCord) {
		RndCord = rand() % 4;
		RndCord = RndCord * 3;
	}
	XcordComptrCar1 = RndCord;
	int RndCord1 = XcordComptrCar1;
	while(XcordComptrCar1 == RndCord1) {
		RndCord1 = rand() % 4;
		RndCord1 = RndCord1 * 3;
	}
	XcordComptrCar2 = RndCord1;
	score++;
}

//Mechanics changing levels
if (score % 5 == 0 && (Incremt == 25 || Incremt == -4)) {
	level++;

	//Mechanics changing speed of computer controlling cars
	if (DifiLevl==1) {
		delay = delay - 10;
	}
	else if (DifiLevl == 2) {
		delay = delay - 5;
	}
	else if (DifiLevl == 3) {
		delay = delay - 5;
	}
			
}

//Displaying the win ccreen and final high score
if (delay<0) {
	gotoxy(59, 4);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << "YOU WIN" << endl;
	gotoxy(59, 5);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	cout << "Score:" << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << score;
	gotoxy(59, 6);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE) << "Level:" << "12";
	BolGamestatue = 0;
	int intExt = Exit();
	if (intExt == 0) {
		goto StructOutr;
		}
		else {
			return 1;
		}

}
Sleep(delay);
if (score == 5) {   
	Difi = 2;
	}


}

//Save score in TXT file
LogHighScore(ChrPlyr,score);

//Displaying "Game Over" screen and score
if (delay>0) {
	gotoxy(59, 4);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << "GAME OVER" << endl;
	gotoxy(59, 5);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	cout << "High Score:" << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << score;
	gotoxy(59, 6);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE) << "Level:" << level;
	int intExt = Exit();
	if (intExt == 0) {
		goto StructOutr;
	}
	else {
		return 1;
	}
}

CloseHandle(hThread);
_getch();
return 0;
}

//Player Car Control
DWORD WINAPI ThreadFn(LPVOID vpParam)
{

	int leftCr = 0;
	int rigtCr = 3;
	while (1) { 
	
	chrGetKey = _getch();
		if (Xlngt == 12) {
			if (chrGetKey == 'a') {
				if (rigtCr == 0 && leftCr == 3) {
					XcordDrw = 6;
					rigtCr++;
					leftCr--;
				}
				else if (rigtCr == 1 && leftCr == 2) {
					XcordDrw = 3;
					rigtCr++;
					leftCr--; 
				}
				else if (rigtCr == 2 && leftCr == 1) {
					XcordDrw = 0;
					rigtCr++;
					leftCr--; 
				}
		}
		else if (chrGetKey == 'd') {
			if (rigtCr == 3 && leftCr == 0) {
				XcordDrw = 3;
				rigtCr--;
				leftCr++;
			}
		else if (rigtCr == 2 && leftCr == 1) {
			XcordDrw = 6;
			rigtCr--;
			leftCr++; 
			}
		else if (rigtCr == 1 && leftCr == 2) {
			XcordDrw = 9;
			rigtCr--;
			leftCr++; 
			}
		}
		else if (chrGetKey == KEY_LEFT) {
			if (rigtCr == 0 && leftCr == 3) {
				XcordDrw = 6;
				rigtCr++;
				leftCr--;
			}
			else if (rigtCr == 1 && leftCr == 2) {
				XcordDrw = 3;
				rigtCr++;
				leftCr--; 
			}
			else if (rigtCr == 2 && leftCr == 1) {
				XcordDrw = 0;
				rigtCr++;
				leftCr--; 
			}

		}
		else if (chrGetKey == KEY_RIGHT) {
			if (rigtCr == 3 && leftCr == 0) {
				XcordDrw = 3;
				rigtCr--;
				leftCr++;
			}
			else if (rigtCr == 2 && leftCr == 1) {
				XcordDrw = 6;
				rigtCr--;
				leftCr++; 
			}
			else if (rigtCr == 1 && leftCr == 2) {
				XcordDrw = 9;
				rigtCr--;
				leftCr++; 
			}

		}


	}
	if (Xlngt == 6) {
		if (chrGetKey == 'a') {
			XcordDrw = 0;
		}
		else if (chrGetKey == 'd') {
			XcordDrw = 3;
		}


	}


}
return 0;
}

// Set console cursor position on given cordinations

void gotoxy(int x, int y) 
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Assign car coordinates to an array
void resetb() 
{
	for (int j = 0; j<26; j++) {

		for (int i = 0; i<Xlngt; i++) {

			ArryDraw[i][j][0] = 0;
			ArryDraw[i][j][1] = 0;


		}
	}
}

void drawp(int x, int y, int Cr) { 

	if (x >= 0 && x<Xlngt && y >= 0 && y<26) {

		ArryDraw[x][y][Cr] = 1;

	}

}

// Drawing Car Template
void drawCar(int x, int y, int Cr)  
{
	drawp(1 + x, 0 + y, Cr);
	drawp(0 + x, 1 + y, Cr);
	drawp(1 + x, 1 + y, Cr);
	drawp(2 + x, 1 + y, Cr);
	drawp(1 + x, 2 + y, Cr);
	drawp(0 + x, 3 + y, Cr);
	drawp(1 + x, 3 + y, Cr);
	drawp(2 + x, 3 + y, Cr);

}

// Random Display Opponents
int getCar() 
{
	int n = 0;

	int Modr = 2;

	if (Xlngt == 12) {
		Modr = 4;
	}
	else if (Xlngt == 9) {
		Modr = 3;
	}
	else if (Xlngt == 6) {
		Modr = 2;
	}

	int random = rand();
	
	if (random % Modr == 0) {
		n = 0;
	}
	else if (random % Modr == 1) {
		n = 3;
	}
	else if (random % Modr == 2) {
		n = 6;
	}
	else if (random % Modr == 3) {
		n = 9;
	}

	return n;

}

// Print Main Menu
void Intro() {

	system("cls");
	gotoxy(44, 3);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE) << "C++ Racing Game";
	gotoxy(44, 4);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	cout << "Player Name: ";
	cin >> ChrPlyr;
	gotoxy(44, 6);
	cout << "____________________________________________ ";
	VallidLbel1:gotoxy(44, 7);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE)<<ChrPlyr<< TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << " Select Difficulty Level ";
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	
		
VallidLbel11:gotoxy(44, 8);

cout << "_____________________________________________ " ;
	gotoxy(44, 9);
		cout << "       Easy        (1) ";
	gotoxy(44, 10);
	    cout << "       Medium     (2)";
	gotoxy(44,11);
	     cout<< "       Hard       (3) ";
	gotoxy(44, 12);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	cout << "___________________________________________ ";
	gotoxy(44, 14);
	VallidLbel12:int intInput;
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE)<<"Confirm your choise :";
	cin >> intInput;
	DifiLevl = intInput;
	
		if (intInput==1) {
		delay = 85;
		Dificlty = 'a';
	}
	else if (intInput == 2) {
		delay = 68;
		Dificlty = 'a';

	}
	else if (intInput == 3) {
		delay = 37;
		Dificlty = 'h';
	}
	else  {
		gotoxy(44, 15);
		cout << " <!> 	Invalid Input  <!>   Default Difficulty activatedc(Easy)";
		system("cls");
		Dificlty = 'a';
	}

}

//Print High Score Table and Shutdown Game Mechanic
int Exit() {
	system("cls");
	gotoxy(44, 3);
	readScore();

	system("cls");

	gotoxy(44, 3);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << "Do you want quit the game? (y/n) :"<<TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);

	char Chrchoice;
	cin >> Chrchoice;
	
	if (Chrchoice=='y') {
		return 0;
	}
	else if (Chrchoice=='n') {
	
		return 1;
	}
	else{
		return 1 ;
	}

}

//Save Score to TXT file
void LogHighScore(char name[],int scoret) {
	try {
		std::ofstream log("High Score.txt", std::ios_base::app | std::ios_base::out);
		if (Dificlty == 'h') {
			log << "Player " << name << "  High Score :" << scoret << "   Level :  | Hard |\n";
		}
		std::ofstream log1("High Score.txt", std::ios_base::app | std::ios_base::out);
		log1 << name << "  High Score :" << scoret << "   Level :  | ---------- |\n";
	
	}
	catch (exception& e)
	{
		cout << e.what() << '\n';
	}


}

//Load and Display data in TXT file of High Score
void readScore() {

	gotoxy(44, 3);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED)<< "------------------Score Board--------------------";
	gotoxy(44, 4);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE) << "-----Display only <Hard> Difficulty Level-------";
	gotoxy(44, 5);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE) << "_______________________________________________";
	string STRING;
	ifstream infile;
	int lineY=6;
	infile.open("High Score.txt");
	while (!(infile.eof()))
	{
		getline(infile, STRING); 
		gotoxy(44, lineY);
		cout << STRING<<endl; 
		lineY++;
	}
	infile.close();
	gotoxy(44 , lineY + 1);
	for (int i = 0; i <8; i++) {  
		cout << "##";
		Sleep(1000);
		gotoxy(44 + i, lineY + 1);
	}
	system("cls");
}

	
