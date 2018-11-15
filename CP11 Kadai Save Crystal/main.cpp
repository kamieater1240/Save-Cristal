#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#define objectnum(obj) (sizeof(obj)sizeof(obj[0]))
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include "DblBuffer.h"
using namespace std;

//Attribute = 0 physic, 1 magic
typedef struct {
	char name[100];
	int hp;
	int atk;
	int def;
	int attribute;
}STATUS;

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

//入力したキャラクター名前が存在しているかどうか確認する
bool FindName(STATUS *, STATUS *, int);

//Cursorの座標
COORD pos = { 0, 0 };

void main() {

	//window handleを掴む
	HANDLE hWindow;
	hWindow = GetStdHandle(STD_OUTPUT_HANDLE);

	//Cursor情報を取る
	CONSOLE_CURSOR_INFO CCI;

	//window titleを設定する
	SetConsoleTitleA("セーブ　クリスタル");

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
	char loadName[100];
	int loadHp, loadAtk, loadDef, loadAttribute;
	int characterNum = 0;
	saved_Char_P = &saved_Characters[0];

	while (fread(&load_Chracter, sizeof(load_Chracter), 1, fp) == 1) {
		strcpy((saved_Char_P + characterNum)->name, load_Chracter.name);
		(saved_Char_P + characterNum)->hp = load_Chracter.hp;
		(saved_Char_P + characterNum)->atk = load_Chracter.atk;
		(saved_Char_P + characterNum)->def = load_Chracter.def;
		(saved_Char_P + characterNum)->attribute = load_Chracter.attribute;
		characterNum++;
	}
	if (characterNum != 0)
		printf("Have %d Data!\n", characterNum);

	getchar();

	//新規キャラクターを生成する
	STATUS input_status;
	STATUS *in_st;
	in_st = &input_status;

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
		if (in_st->attribute == 0)
			printf("自動生成キャラクター\n名前: %s\nHP: %d\nATK: %d\nDEF: %d\nAttribute: Physic\n", in_st->name, in_st->hp, in_st->atk, in_st->def);
		else
			printf("自動生成キャラクター\n名前: %s\nHP: %d\nATK: %d\nDEF: %d\nAttribute: Magic\n", in_st->name, in_st->hp, in_st->atk, in_st->def);
	}
	else {
		printf("キャラクターはすでに存在しています。\n");
		if (in_st->attribute == 0)
			printf("キャラクター\n名前: %s\nHP: %d\nATK: %d\nDEF: %d\nAttribute: Physic\n", in_st->name, in_st->hp, in_st->atk, in_st->def);
		else
			printf("キャラクター\n名前: %s\nHP: %d\nATK: %d\nDEF: %d\nAttribute: Magic\n", in_st->name, in_st->hp, in_st->atk, in_st->def);
	}

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