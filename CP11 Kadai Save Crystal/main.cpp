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

//���͂����L�����N�^�[���O�����݂��Ă��邩�ǂ����m�F����
bool FindName(STATUS *input, STATUS *loadList, int loadNum);

void PrintPlayerStatus(HANDLE hWindow, COORD pos, STATUS *input);

//Cursor�̍��W
COORD pos = { 0, 0 };

void main() {

	//window handle��͂�
	HANDLE hWindow;
	hWindow = GetStdHandle(STD_OUTPUT_HANDLE);

	//Cursor�������
	CONSOLE_CURSOR_INFO CCI;

	//window��cursor���B��
	CCI.dwSize = 1;
	CCI.bVisible = false;
	SetConsoleCursorInfo(hWindow, &CCI);

	//window title��ݒ肷��
	SetConsoleTitleA("�Z�[�u�@�N���X�^��");

	//--------------------------------�f�[�^���[�h------------------------------------------------
	//�t�@�C�����I�[�v������
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
	//�L�����N�^�[�ϐ���錾����
	STATUS input_status;
	STATUS *in_st;
	in_st = &input_status;
	//=========================================//

	//�J�n��ʂ�\������-------------------------------------------
	int CreateorLoad;
	bool flag_finished = true;
	while (flag_finished) {
		//=========================================//
		CreateorLoad = DrawStartMenu(hWindow, pos);
		//�V�K�L�����N�^�[�𐶐�����
		if (CreateorLoad == 0) {

			srand((unsigned)time(NULL));

			printf("�L�����N�^�[�̖��O����͂��Ă�������:");
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
				printf("���������L�����N�^�[\n");
				PrintPlayerStatus(hWindow, pos, in_st);
			}
			else {
				pos = { 32, 7 };
				SetConsoleCursorPosition(hWindow, pos);
				printf("�L�����N�^�[�͂��łɑ��݂��Ă��܂��B\n");
				PrintPlayerStatus(hWindow, pos, in_st);
			}
		}
		//=========================================//

		//=========================================//
		//�����L�����N�^�[�����[�h����
		else {
			if (characterNum == 0) {
				SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | 0x0);
				system("cls");
				printf("�܂��L�����N�^�[�����Ȃ�����A�V�K�������Ă�������...");
				Sleep(2000);
				continue;
			}

			int getNum;
			getNum = LoadCharacter(hWindow, pos, saved_Char_P, characterNum);
			SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | 0x0);
			system("cls");

			pos = { 32, 7 };
			SetConsoleCursorPosition(hWindow, pos);
			printf("%d�ԖڃL�����N�^�[���Q�b�g���܂�!!!!!\n", getNum + 1);

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
	//�o�g��������ʂ�\������
	ClearScreen(hWindow, pos, 30, 100);
	LoadingBattle(hWindow, pos);
	
	rewind(stdin);
	_getch();
	system("cls");

	pos = { 35, 11 };
	SetConsoleCursorPosition(hWindow, pos);
	SetConsoleTextAttribute(hWindow, FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("�͂��A�o�g���`�I���I�Id(`��ͥ)b");
	pos = { 25, 12 };
	SetConsoleCursorPosition(hWindow, pos);
	printf("�i�o�g���̕����͂܂��ł��Ă��Ȃ��A���߂�Ȃ����B�j");

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

		//���Ƃ����͂��ꂽ���O�̃A���t�@�x�b�g�̑召������Ă��������O�𔻒f�ł���
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
	printf("�L�����N�^�[\n");
	
	for (int i = 0; i < 5; i++) {
		pos.Y = 10 + i;
		SetConsoleCursorPosition(hWindow, pos);
		if (i == 0)
			printf("���O�F%s", input->name);
		else if (i == 1)
			printf("HP�F%d", input->hp);
		else if (i == 2)
			printf("ATK�F%d", input->atk);
		else if (i == 3)
			printf("DEF�F%d", input->def);
		else {
			if(input->attribute==0)
				printf("ATTRIBUTE�FPhysic", input->attribute);
			else
				printf("ATTRIBUTE�FMagic", input->attribute);
		}
	}
}