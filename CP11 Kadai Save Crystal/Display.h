#pragma once



void DrawRectangle(HANDLE hWindow, COORD pos, int width, int height, char drawChar, char emptyChar);

void drawchoices(HANDLE hWindow, COORD pos, char(*choice)[100], int listNum, int index);

int DrawStartMenu(HANDLE hWindow, COORD pos);

int LoadCharacter();