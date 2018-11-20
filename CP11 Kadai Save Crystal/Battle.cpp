#define _CRT_SECURE_NO_WARNINGS
#define objectnum(obj) (sizeof(obj)/sizeof(obj[0]))
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string>
#include <iostream>
#include "Battle.h"
using namespace std;

void LoadingBattle(HANDLE hWindow, COORD pos) {

	while (1) {
		pos = { 34, 11 };
		SetConsoleCursorPosition(hWindow, pos);
		SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("魔王とのバトル、準備はいいか！！");
		pos = { 37, 18 };
		SetConsoleCursorPosition(hWindow, pos);
		SetConsoleTextAttribute(hWindow, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("Press any key to start....");
		if (_kbhit())
			break;
		Sleep(600);

		pos = { 34, 11 };
		SetConsoleCursorPosition(hWindow, pos);
		SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("魔王とのバトル、準備はいいか！！");
		pos = { 37, 18 };
		SetConsoleCursorPosition(hWindow, pos);
		SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("Press any key to start....");
		if (_kbhit())
			break;
		Sleep(600);
	}
}

