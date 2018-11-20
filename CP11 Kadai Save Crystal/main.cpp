#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#define objectnum(obj) (sizeof(obj)/sizeof(obj[0]))
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include "Display.h"
#include "Battle.h"
using namespace std;

//入力したキャラクター名前が存在しているかどうか確認する
bool FindName(STATUS *input, STATUS *loadList, int loadNum);

void PrintPlayerStatus(HANDLE hWindow, COORD pos, STATUS *input);

//Cursorの座標
COORD pos = { 0, 0 };

void main() {

	//window handleを掴む
	HANDLE hWindow;
	hWindow = GetStdHandle(STD_OUTPUT_HANDLE);

	//Cursor情報を取る
	CONSOLE_CURSOR_INFO CCI;

	//windowのcursorを隠す
	CCI.dwSize = 1;
	CCI.bVisible = false;
	SetConsoleCursorInfo(hWindow, &CCI);

	//window titleを設定する
	SetConsoleTitleA("セーブ　クリスタル");

	//--------------------------------データロード------------------------------------------------
	//ファイルをオープンする
	FILE *fp;
	fp = fopen("status.dat", "ab+");
	if (!fp) {
		printf("Fail to open file...\n");
		exit(1);
	}

	//Load saved character
	STATUS saved_Characters[100], *saved_Char_P;
	STATUS load_Chracter;
	int characterNum = 0;
	saved_Char_P = &saved_Characters[0];

	while (fread(&load_Chracter, sizeof(load_Chracter), 1, fp) == 1) {
		*(saved_Char_P + characterNum) = load_Chracter;
		characterNum++;
	}
	//--------------------------------------------------------------------------------------------

	//=========================================//
	//キャラクター変数を宣言する
	STATUS input_status;
	STATUS *in_st;
	in_st = &input_status;
	//=========================================//

	//開始画面を表示する-------------------------------------------
	int CreateorLoad;
	bool flag_finished = true;
	while (flag_finished) {
		//=========================================//
		CreateorLoad = DrawStartMenu(hWindow, pos);
		//新規キャラクターを生成する
		if (CreateorLoad == 0) {

			srand((unsigned)time(NULL));

			printf("キャラクターの名前を入力してください:");
			scanf("%s", in_st->name);

			bool HaveorNot = false;
			if (characterNum > 0) {
				HaveorNot = FindName(in_st, saved_Char_P, characterNum);
				if (HaveorNot == false) {
					for (int i = 0; i < 4; i++) {
						if (i == 0)
							in_st->hp = (rand() % (500 - 100 + 1)) + 100;
						else if (i == 1)
							in_st->atk = (rand() % (100 - 10 + 1)) + 10;
						else if (i == 2)
							in_st->def = (rand() % (100 - 10 + 1)) + 10;
						else if (i == 3)
							in_st->attribute = (rand() % (1 - 0 + 1)) + 0;
					}
					fwrite(in_st, sizeof(*in_st), 1, fp);
				}
			}
			else {
				for (int i = 0; i < 4; i++) {
					if (i == 0)
						in_st->hp = (rand() % (500 - 100 + 1)) + 100;
					else if (i == 1)
						in_st->atk = (rand() % (100 - 10 + 1)) + 10;
					else if (i == 2)
						in_st->def = (rand() % (100 - 10 + 1)) + 10;
					else if (i == 3)
						in_st->attribute = (rand() % (1 - 0 + 1)) + 0;
				}
				fwrite(in_st, sizeof(*in_st), 1, fp);
			}

			fclose(fp);

			if (HaveorNot == false) {
				pos = { 40, 7 };
				SetConsoleCursorPosition(hWindow, pos);
				printf("自動生成キャラクター\n");
				PrintPlayerStatus(hWindow, pos, in_st);
			}
			else {
				pos = { 32, 7 };
				SetConsoleCursorPosition(hWindow, pos);
				printf("キャラクターはすでに存在しています。\n");
				PrintPlayerStatus(hWindow, pos, in_st);
			}
		}
		//=========================================//

		//=========================================//
		//既存キャラクターをロードする
		else {
			if (characterNum == 0) {
				SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | 0x0);
				system("cls");
				printf("まだキャラクターがいないから、新規生成してください...");
				Sleep(2000);
				continue;
			}

			int getNum;
			getNum = LoadCharacter(hWindow, pos, saved_Char_P, characterNum);
			SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | 0x0);
			system("cls");

			pos = { 32, 7 };
			SetConsoleCursorPosition(hWindow, pos);
			printf("%d番目キャラクターをゲットします!!!!!\n", getNum + 1);

			*in_st = *(saved_Char_P + getNum);
			PrintPlayerStatus(hWindow, pos, in_st);
		}
		flag_finished = false;
		rewind(stdin);
		getchar();
		rewind(stdin);
		_getch();
	}

	//START BATTLE !!!!!!!!!!!!!!!!!!!!!!!!======================================================//
	//バトル準備画面を表示する
	ClearScreen(hWindow, pos, 30, 100);
	LoadingBattle(hWindow, pos);
	
	rewind(stdin);
	_getch();
	system("cls");

	pos = { 35, 11 };
	SetConsoleCursorPosition(hWindow, pos);
	SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("はい、バトル～終了！！d(`･∀･)b");
	pos = { 25, 12 };
	SetConsoleCursorPosition(hWindow, pos);
	printf("（バトルの部分はまだできていない、ごめんなさい。）");

	//===========================================================================================//

	rewind(stdin);
	getchar();
	return;
}

bool FindName(STATUS *input, STATUS *loadList, int loadNum) {

	char check1[100], check2[100];

	for (int i = 0; i < strlen(input->name); i++)
		check1[i] = tolower(input->name[i]);
	check1[strlen(input->name)] = '\0';

	for (int i = 0; i < loadNum; i++) {

		for (int j = 0; j < strlen((loadList + i)->name); j++)
			check2[j] = tolower((loadList + i)->name[j]);
		check2[strlen((loadList + i)->name)] = '\0';

		//たとえ入力された名前のアルファベットの大小が違っても同じ名前を判断できる
		if (!strcmp(check1, check2)) {
			input->hp = (loadList + i)->hp;
			input->atk = (loadList + i)->atk;
			input->def = (loadList + i)->def;
			input->attribute = (loadList + i)->attribute;
			return true;
		}
	}
	return false;
}

void PrintPlayerStatus(HANDLE hWindow, COORD pos, STATUS *input) {
	
	pos = { 42, 8 };
	SetConsoleCursorPosition(hWindow, pos);
	printf("キャラクター\n");
	
	for (int i = 0; i < 5; i++) {
		pos.Y = 10 + i;
		SetConsoleCursorPosition(hWindow, pos);
		if (i == 0)
			printf("名前：%s", input->name);
		else if (i == 1)
			printf("HP：%d", input->hp);
		else if (i == 2)
			printf("ATK：%d", input->atk);
		else if (i == 3)
			printf("DEF：%d", input->def);
		else {
			if(input->attribute==0)
				printf("ATTRIBUTE：Physic", input->attribute);
			else
				printf("ATTRIBUTE：Magic", input->attribute);
		}
	}
}