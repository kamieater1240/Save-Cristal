#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

//Attribute = 0 physic, 1 magic
typedef struct {
	char name[100];
	int hp;
	int atk;
	int def;
	int attribute;
}STATUS;

void DrawRectangle(HANDLE hWindow, COORD pos, int width, int height, char drawChar, char emptyChar);

void drawchoices(HANDLE hWindow, COORD pos, char(*choice)[100], int listNum, int index);

void drawchoices_forLoad(HANDLE hWindow, COORD pos, STATUS *loadList, int listNum, int indexsize, int columnsize, int index, int column);

int DrawStartMenu(HANDLE hWindow, COORD pos);

int LoadCharacter(HANDLE hWindow, COORD pos, STATUS *loadList, int listNum);

#endif // !DISPLAY_H