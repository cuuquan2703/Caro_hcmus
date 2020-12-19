#pragma once
#include <string>
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")
#define BOARD_SIZE 16
#define LEFT 3
#define TOP 1
#define Black 0
#define DeepCyan 3
#define BLUE 9
#define Cyan 11
#define RED 12
#define Pink 13
#define Yellow 14
#define White 15
#define Green 10
using namespace std;

void gotoxy(int x, int y);
void resizeConsole(int width, int height);
void textcolor(int x);
void DisableCtrButton(bool Close, bool Min, bool Max);
void Draw();
void SetScreenBufferSize(SHORT width, SHORT height);
void XoaManHinh();
void FixConsoleWindow();
void SetConsoleWindow(int w, int h);
void Reset();
void starGame();
void  Checkboard();
bool KT_doc(int n, int m);
bool KT_cheo_chinh(int n, int m);
bool KT_ngang(int n, int m);
bool KT_cheo_phu(int n, int m);
void MoveRight();
void MoveLeft();
void MoveDown();
void MoveUp();
int START();
bool KT();
void processCheckBoard();
int PlaySoundEffect(int sound);
void P1();
void P2();
void _cout(int x, int y, string c);
void PrintCaro();
void about_us();
void _Setcolor(int c);
int _SelectCmd();
void Menu();
void Instruction();
int save_Game();
int _continue();
void Load_Game();
