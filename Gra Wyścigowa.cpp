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

//Definiowanie Kodu ASCII dla klawiszy strza³ek i kolorów
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

// Definiowanie i inicjalizacja warunków dla toru wyœcigowego
int Xlngt = 12;
int XlngtCrd = 57;

//Zmiana Kolorystyki
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


// Mechanika Poziomu Trudnoœci
int Difi=1;
int DifiLevl;
char Dificlty;

// Nazwa Gracza
char ChrPlyr[]="Gracz1";

// Deklaracja Funkcji
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

//Deklaracja Tablicy dla Toru Wyœcigowego
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
	
	//Konfigurowanie Œrodowiska Gry
	Intro();
	VarIniz();
	system("cls");
	
	DWORD qThreadID;
	HANDLE hThread = CreateThread(0, 0, ThreadFn, 0, 0, &qThreadID);  

	//Ekran Powitalny
		gotoxy(44, 2);

	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE) << "Gra Wyscigowa w C++";
	gotoxy(44, 4);
	cout << TextAttr FOREGROUND_WHITE << "Sterowanie";
	gotoxy(44, 5);
	cout << "========";
	gotoxy(44, 6);
	cout << "[A]=Lewo  or Strza³ka w Lewo  [<-]";
	gotoxy(44, 7);
	cout << "[D]=Prawo or Strza³ka w Prawo [->]";

// Wyœwietlanie Zegara Przed Rozpoczêciem Gry
for (int i = 5,j=0; i >= 0; i--,j++) {  

		gotoxy(44, 8);
		cout << i;
		gotoxy(45+j, 8);
		cout << "*";
		gotoxy(51, 15);
		Sleep(1000);

	}
		system("cls");
	
//Uruchomienie gry
	while (BolGamestatue) {

		gotoxy(59, 4);
		cout << "WYNIK:" << score;
		gotoxy(59, 5);
		cout << "POZIOM:" << level;

		resetb();
	
//Szablon Kolorystyki Samochodów Sterowanych przez Komputer
	drawCar(XcordComptrCar, Incremt, 0);

	if (Difi==1) {
		drawCar(XcordComptrCar1, Incremt, 0);
	}else if (Difi == 2) {
		drawCar(XcordComptrCar1, Incremt, 0);
		drawCar(XcordComptrCar2, Incremt, 0);
	}

//Szablon Samochodu Sterowanego przez Gracza
	drawCar(XcordDrw, 21, 1);
	for (int j = 0; j<26; j++) {
	
//Wyœwietlanie Lewej Linii Toru Wyœcigowego
	gotoxy(44, j + 1);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << "S";
	cout << TextAttr(FOREGROUND_WHITE);
	gotoxy(45, j + 1);
		
//Wyœwitlanie Ruchu Samochodów Sterowanych przez Komputer
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

//Wyœwietlanie Prawej Linni Toru Wyœcigowego
gotoxy(XlngtCrd, j + 1);
cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << "S";
cout << TextAttr(FOREGROUND_WHITE);
	
//Mechanika Kolizji Samochodu
if (Incremt >= 21 && (XcordDrw == XcordComptrCar || XcordDrw == XcordComptrCar1 || XcordDrw == XcordComptrCar1)) {

	BolGamestatue = 0;

		}
}
Incremt++;
		
// Punktacja i Wyznaczenie Trasy Dla Samochodów Sterowanych Przez Komputer
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

// Zmiana Poziomu
if (score % 5 == 0 && (Incremt == 25 || Incremt == -4)) {
	level++;

	// Zmiana prêdkoœci Samochodów Sterowanych Przez Komputer
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

// Wyœwietlanie Ekranu Wygranej oraz Ostatecznego Wyniku Punktowego
if (delay<0) {
	gotoxy(59, 4);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << "WYGRALES" << endl;
	gotoxy(59, 5);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	cout << "WYNIK:" << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << score;
	gotoxy(59, 6);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE) << "POZIOM:" << "12";
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
	
//Zapisywanie Wyniku w Piku TXT
LogHighScore(ChrPlyr,score);

//Wyœwitlenie Ekranu "KONIEC GRY" oraz Wyniku Punktowego
if (delay>0) {
	gotoxy(59, 4);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << "KONIEC GRY" << endl;
	gotoxy(59, 5);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	cout << "NAJLEPSZY WYNIK:" << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << score;
	gotoxy(59, 6);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE) << "POZIOM:" << level;
	int intExt = Exit();
	if (intExt == 0) {
		goto StructOutr;
	}
	else {
		return 1;
	}
}
	
//Zamkniêcie W¹tku
CloseHandle(hThread);
_getch();
return 0;
}


//Sterowanie Samochodem Gracza
DWORD WINAPI ThreadFn(LPVOID vpParam)
{

	int leftCr = 0;
	int rigtCr = 3;
	while (1) {     

//Pobieranie Wartoœci Klawisza
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

// Ustawienie pozycji kursora na podanych wspó³rzêdnych

void gotoxy(int x, int y) 
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Przypisywanie wspó³rzêdnych samochodu do tablicy
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

// Rysowanie Szablonu Samochodu
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

// Losowe wyœwietlanie przeciwników
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

// Wyœiwetlanie Menu G³ównego
void Intro() {

	system("cls");
	gotoxy(44, 3);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE) << "Gra Wyscigowa w C++";
	gotoxy(44, 4);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	cout << "Imie Gracza: ";
	cin >> ChrPlyr;
	gotoxy(44, 6);
	cout << "____________________________________________ ";
	VallidLbel1:gotoxy(44, 7);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE)<<ChrPlyr<< TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << " Wybierz Poziom Trudnosci ";
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	
VallidLbel11:gotoxy(44, 8);

cout << "_____________________________________________ " ;
	gotoxy(44, 9);
		cout << "       Latwy        (1) ";
	gotoxy(44, 10);
	    cout << "       Sredni     (2)";
	gotoxy(44,11);
	     cout<< "       Trudny       (3) ";
	gotoxy(44, 12);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
	cout << "___________________________________________ ";
	gotoxy(44, 14);
	VallidLbel12:int intInput;
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE)<<"Zatwierdz swoj wybor :";
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
		cout << " <!> Nieprawidlowe Dane Wejsciowe  <!>   Wybrano Domyslny Poziom Trudnosci(Latwy)";
		system("cls");
		Dificlty = 'a';
	}

}
	

// Wyœwietlane Tabeli Wyniku oraz Mechanizm Zamykania Programu
int Exit() {
	system("cls");
	gotoxy(44, 3);
	readScore();

	system("cls");

	gotoxy(44, 3);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED) << "Czy Chcesz Opuscic Gre? (t/n) :"<<TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);

	char Chrchoice;
	cin >> Chrchoice;
	
	if (Chrchoice=='t') {
		return 0;
	}
	else if (Chrchoice=='n') {
	
		return 1;
	}
	else{
		return 1 ;
	}

}

// Zapis Wyniku jako plik TXT
void LogHighScore(char name[],int scoret) {
	try {
		std::ofstream log("Tabela Wynikow.txt", std::ios_base::app | std::ios_base::out);
		if (Dificlty == 'h') {
			log << "Gracz " << name << "  Najlepszy Wynik :" << scoret << "   Poziom :  | Trudny |\n";
		}
		std::ofstream log1("Tabela Wynikow.txt", std::ios_base::app | std::ios_base::out);
		log1 << name << "  Najlepszy Wynik :" << scoret << "   Poziom :  | ---------- |\n";
	
	}
	catch (exception& e)
	{
		cout << e.what() << '\n';
	}


}

//Wczytywanie i wyœwietlanie danych z pliku TXT o wynikach
void readScore() {

	gotoxy(44, 3);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED)<< "------------------Tabela Wynikow--------------------";
	gotoxy(44, 4);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE) << "-----Wyniki tylko <Trudnego> Poziomu Trudnosci-------";
	gotoxy(44, 5);
	cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE) << "_______________________________________________";
	string STRING;
	ifstream infile;
	int lineY=6;
	infile.open("Tabela Wynikow.txt");
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












