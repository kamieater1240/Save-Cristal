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
using namespace std;

typedef struct {
	char name[30];
	int hp;
	int atk;
	int def;
}STATUS;

void main() {

	FILE *fp, *fr;
	fp = fopen("status.txt", "w");

	STATUS status[4];
	STATUS *st;
	st = status;

	printf("type character name:");
	scanf("%s", st->name);

	int len;
	len = strlen(st->name);
	printf("mojiretsu nagasa  %d\n", len);

	if (len = 4) {
		for (int i = 0; i < 3; i++) {
			fprintf(fp, "%d\n", rand() % 100);
		}
	}

	fclose(fp);

	fr = fopen("status.txt", "r");
	if (!fr)
		printf("wtf\n");
	fscanf(fr, "%d%d%d", &st->hp, &st->atk, &st->def);
	printf("%d   %d   %d\n", st->hp, st->atk, st->def);

	fclose(fr);

	rewind(stdin);
	getchar();
	return;
}