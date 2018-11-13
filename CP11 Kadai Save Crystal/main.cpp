#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#define question_num 10
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

bool FindName(STATUS *, STATUS *, int);

void main() {

	FILE *fp, *fr;
	fp = fopen("status.txt", "at+");

	//Load saved character
	STATUS saved_Characters[100], *saved_Char_P;
	char loadName[100];
	int loadHp, loadAtk, loadDef, loadAttribute;
	int characterNum = 0;
	saved_Char_P = &saved_Characters[0];
	while (fscanf(fp, "%s%d%d%d%d", loadName, &loadHp, &loadAtk, &loadDef, &loadAttribute) != EOF) {
		strcpy((saved_Char_P + characterNum)->name, loadName);
		(saved_Char_P + characterNum)->hp = loadHp;
		(saved_Char_P + characterNum)->atk = loadAtk;
		(saved_Char_P + characterNum)->def = loadDef;
		(saved_Char_P + characterNum)->attribute = loadAttribute;
		characterNum++;
	}

	STATUS input_status;
	STATUS *in_st;
	in_st = &input_status;

	srand((unsigned)time(NULL));

	printf("キャラクターの名前を入力してください:");
	scanf("%s", in_st->name);

	bool HaveorNot;
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
			fprintf(fp, "%s\n%d\n%d\n%d\n%d\n", in_st->name, in_st->hp, in_st->atk, in_st->def, in_st->attribute);
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