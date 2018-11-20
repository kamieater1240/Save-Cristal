#define _CRT_SECURE_NO_WARNINGS
#define objectnum(obj) (sizeof(obj)/sizeof(obj[0]))
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string>
#include <iostream>
#include "Display.h"
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

//入力する鍵を判断する、それに今選択する選択肢の行列を戻る
int getinput(int *row, int rowNum, int * column, int columnNum, int listNum) {

	int get;
	get = _getch();
	if (get == UP) {
		if (*row > 0)
			*row -= 1;
		else {
			if (((rowNum - 1) * columnNum + *column) > listNum - 1)
				*row = rowNum - 2;
			else
				*row = rowNum - 1;
		}
	}
	else if (get == DOWN) {
		if (*row < rowNum - 1) {
			if (((*row + 1) * columnNum + *column) > listNum - 1)
				*row = 0;
			else
				*row += 1;
		}
		else {
			*row = 0;
		}
	}
	else if (get == LEFT) {
		if (*column > 0)
			*column -= 1;
		else if (*column == 0)
			if ((*row * columnNum + columnNum - 1) > listNum - 1)
				*column = *column + (listNum - 1) - (*row * columnNum);
			else
				*column = columnNum - 1;
	}
	else if (get == RIGHT) {
		if (*column < columnNum - 1 && (*row * columnNum + *column) != listNum - 1)
			*column += 1;
		else
			*column = 0;
	}
	else if (get == ENTER)
		return ENTER;

	return 0;
}

void ClearScreen(HANDLE hWindow, COORD pos, int height, int width) {

	pos = {0, 0};
	SetConsoleCursorPosition(hWindow, pos);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf(" ");
		}
	}
	pos = { 0, 0 };
	SetConsoleCursorPosition(hWindow, pos);
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
	for (int i = 0; i < listNum; i++) {
		//strlength = strlen(choice[i - 1]);
		//pos.X = Moto_pos.X - strlength / 2;
		if (i == index) {
			SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		}
		else {
			SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		SetConsoleCursorPosition(hWindow, pos);
		pos.Y++;
		printf("%s", choice[i]);
	}
	return;
}

//既存キャラクターを画面に表示する
void drawchoices_forLoad(HANDLE hWindow, COORD pos, STATUS *loadList, int listNum, int indexsize, int columnsize, int index, int column) {


	pos = { 2, 4 };
	SetConsoleCursorPosition(hWindow, pos);

	for (int i = 0; i < indexsize; i++) {
		pos.X = 2;
		pos.Y = 4 + i * 2;
		SetConsoleCursorPosition(hWindow, pos);
		for (int j = 0; j < columnsize; j++) {
			pos.X = 2 + j * 10;
			SetConsoleCursorPosition(hWindow, pos);

			if (i == index && j == column) {
				SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
			}
			else {
				SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			printf("%s", (loadList + i * 3 + j)->name);

			if ((i * 3 + j) == listNum - 1)
				break;
		}
	}
}

//開始画面を描く
int DrawStartMenu(HANDLE hWindow, COORD pos) {
	int strlength, press, row = 0, column = 0;
	char choices[2][100] = {
	"新規生成\n",
	"既存ロード"
	};

	SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | 0x0);
	system("cls");

	Sleep(400);
	pos = { 25, 6 };
	DrawRectangle(hWindow, pos, 50, 8, '*', ' ');

	for (int i = 0; i < objectnum(startText); i++) {
		strlength = strlen(startText[i]);
		pos.X = 50 - strlength / 2;
		pos.Y = 9 + i;
		SetConsoleCursorPosition(hWindow, pos);
		Sleep(400);
		printf("%s", startText[i]);
		Sleep(400);
	}

	pos = { 41, 29 };
	SetConsoleCursorPosition(hWindow, pos);
	printf("Copyright (c) 2018 Josh, All rights reserved.");

	//選択を描く
	pos.X = 40; pos.Y = 15;
	DrawRectangle(hWindow, pos, 20, 4, '*', ' ');
	while (1) {
		pos = { 41, 16 };
		drawchoices(hWindow, pos, choices, 2, row);
		press = getinput(&row, 2, &column, 1, 2);
		if (press == ENTER) {
			SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | 0x0);
			system("cls");
			return row;
		}
	}
}

//既存キャラクターをロードする
int LoadCharacter(HANDLE hWindow, COORD pos, STATUS *loadList, int listNum) {
	int press, row = 0, column = 0;

	while (1) {
		pos = { 0, 0 };
		SetConsoleCursorPosition(hWindow, pos);
		SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		DrawRectangle(hWindow, pos, 100, 29, '*', ' ');

		pos = { 2, 2 };
		SetConsoleCursorPosition(hWindow, pos);
		printf("どのキャラクターを選びますか？");
		drawchoices_forLoad(hWindow, pos, loadList, listNum, ((listNum - 1) / 3) + 1, 3, row, column);
		press = getinput(&row, ((listNum - 1) / 3) + 1, &column, 3, listNum);
		if (press == ENTER) {
			return row * 3 + column;
		}
	}

	return 0;
}