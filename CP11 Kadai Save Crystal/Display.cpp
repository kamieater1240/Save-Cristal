#define _CRT_SECURE_NO_WARNINGS
#define objectnum(obj) (sizeof(obj)/sizeof(obj[0]))
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

//キーボードの入力コードのENUM
enum INPUTCOMMAND
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27,
};

const char *startText[] =
{
	"セーブクリスタルへようこそ！！\n",
	"よろしくお願いします。\n"
};

int getinput(int *row, int rowNum) {

	int get;
	get = _getch();
	if (get == UP) {
		if (*row > 1) {
			*row -= 1;
		}
		else {
			*row = rowNum;
		}
	}
	else if (get == DOWN) {
		if (*row < rowNum) {
			*row += 1;
		}
		else {
			*row = 1;
		}
	}
	else if (get == ENTER)
		return ENTER;

	return 0;
}

//ボーダーラインを描く
void DrawRectangle(HANDLE hWindow, COORD pos, int width, int height, char drawChar, char emptyChar) {

	COORD Moto_pos = pos;
	string firstAndLastRow(width, drawChar);

	SetConsoleCursorPosition(hWindow, pos);

	//first row
	cout << firstAndLastRow << endl;

	//rows between first and last
	for (int row = 0; row < height - 2; row++) {
		pos.X = Moto_pos.X;
		pos.Y++;
		SetConsoleCursorPosition(hWindow, pos);
		string betweenRow(width, emptyChar);
		betweenRow[0] = drawChar;
		betweenRow[width - 1] = drawChar;
		//here we add the posX, so adding posX spaces in front of each line
		cout << betweenRow << endl;
	}

	pos.X = Moto_pos.X; pos.Y++;
	SetConsoleCursorPosition(hWindow, pos);
	//last row
	cout << firstAndLastRow << endl;
}

//選択肢を描く
void drawchoices(HANDLE hWindow, COORD pos, char(*choice)[100], int listNum, int index) {

	COORD Moto_pos = pos;
	int strlength;

	SetConsoleCursorPosition(hWindow, pos);
	for (int i = 1; i <= listNum; i++) {
		strlength = strlen(choice[i - 1]);
		pos.X = Moto_pos.X - strlength / 2;
		if (i == index) {
			SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
			SetConsoleCursorPosition(hWindow, pos);
			pos.Y++;
			printf("%s", choice[i - 1]);
		}
		else {
			SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			SetConsoleCursorPosition(hWindow, pos);
			pos.Y++;
			printf("%s", choice[i - 1]);
		}
	}
	return;
}

//開始画面を描く
int DrawStartMenu(HANDLE hWindow, COORD pos) {
	int strlength, press, row = 1;

	pos.X = 25; pos.Y = 6;
	DrawRectangle(hWindow, pos, 50, 8, '*', ' ');

	for (int i = 0; i < objectnum(startText); i++) {
		strlength = strlen(startText[i]);
		pos.X = 50 - strlength / 2;
		pos.Y = 9 + i;
		SetConsoleCursorPosition(hWindow, pos);
		Sleep(400);
		printf("%s", startText[i]);
	}

	char choices[2][100] = {
		"新規生成\n",
		"既存ロード"
	};

	//選択を描く
	pos.X = 40; pos.Y = 15;
	DrawRectangle(hWindow, pos, 20, 4, '*', ' ');
	while (1) {
		pos.X = 50; pos.Y = 16;
		drawchoices(hWindow, pos, choices, 2, row);
		press = getinput(&row, 2);
		if (press == ENTER) {
			system("cls");
			return row;
			break;
		}
	}
}

int LoadCharacter() {


}