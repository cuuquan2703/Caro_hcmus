#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <Windows.h>
#include <MMsystem.h>
#include <fstream>
#include<iostream>
#include "Header.h"
using namespace std;

bool TURN, KTLOADGAME;
char _COMMAND;
int _X, _Y;
bool l = true;
int h = 0, k = 0;
string A[] = { "New Game","Load Game","Instruction","ABout Us","Exit" };
int Score_p1 = 0, Score_p2 = 0;
int dem = 0, demy = 0;// đếm số quân x ,o
int demf = 0;
string _Loadfile[30];

struct _POINT {
	int x, y, c;
} _A[BOARD_SIZE][BOARD_SIZE];
HANDLE hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};
void DisableSelection()
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);
};
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void textcolor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}
//vohieuhoaBOARD_SIZE
void DisableCtrButton(bool Close, bool Min, bool Max)
{
	HWND hWnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, false);

	if (Close == 1)
	{
		DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}
	if (Min == 1)
	{
		DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
	}
	if (Max == 1)
	{
		DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	}
}
//anthanhcuon
void Draw()
{
	textcolor(3);
	if (BOARD_SIZE == 0) return;
	// Dong thuong 
	for (int i = 0; i <= BOARD_SIZE * 2; i = i + 2)
	{
		Sleep(5);
		gotoxy(LEFT, i + TOP + 1);

		for (int j = 1; j < 3 * BOARD_SIZE + 1; j += 3)
		{

			cout << char(197);

			cout << char(196) << char(196);
		}
	}
	for (int i = 1; i <= BOARD_SIZE * 2 + 1; i++)
	{
		// Ve cot phai
		Sleep(5);
		gotoxy(BOARD_SIZE * 3 + LEFT, i + TOP);
		if ((i + 1) % 2 == 1)
			cout << char(21);
		else
			cout << char(23);
	}
	//dong cuoi
	for (int j = 3 * BOARD_SIZE + 1; j > 0; j--)
	{
		Sleep(2);
		gotoxy(j + LEFT - 1, TOP + BOARD_SIZE * 2 + 1);
		if (j % 2 == 0)
			cout << char(29);
		else
			cout << char(3);
	}
	for (int i = BOARD_SIZE * 2 + 1; i > 1; i--)
	{
		// Ve cot trai
		Sleep(5);
		gotoxy(LEFT, i + TOP);
		if ((i + 1) % 2 == 1)
			cout << char(21);
		else
			cout << char(23);
	}

	// Ve dong dau
	//
	for (int j = 0; j < 3 * BOARD_SIZE + 1; j++)
	{
		Sleep(3);
		gotoxy(j + LEFT, TOP + 1);
		if (j % 2 == 0)
			cout << char(5);
		else
			cout << char(127);
	}

	for (int i = 1; i <= BOARD_SIZE * 2; i += 2)
	{
		Sleep(20);
		for (int j = 3; j < 3 * BOARD_SIZE; j += 3)
		{
			gotoxy(j + LEFT, i + TOP + 1);
			cout << char(179);
		}
	}
}
void SetScreenBufferSize(SHORT width, SHORT height)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD NewSize;
	NewSize.X = width;
	NewSize.Y = height;

	SetConsoleScreenBufferSize(hStdout, NewSize);
};
// Hàm xóa màn hình.
void XoaManHinh()
{
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void SetConsoleWindow(int w, int h)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);

	MoveWindow(console, r.left, r.top, w, h, TRUE);
} 
int PlaySoundEffect(int sound) {
	switch (sound) {
	case 0:
	{
		PlaySound(NULL, 0, 0); //tat nhac
		break;
	}
	case 1:
	{
		PlaySound(TEXT("MarioWorld-Title.wav"), NULL, SND_FILENAME | SND_ASYNC); //nhac nen
		break;
	}
	case 2:
	{
		PlaySound(TEXT("menuchange.wav"), NULL, SND_ALIAS); //thao tac menu
		break;
	}
	case 3:
	{
		PlaySound(TEXT("click.wav"), NULL, SND_FILENAME | SND_ASYNC); //di chuyen trong game
		break;
	}
	case 4:
	{
		PlaySound(TEXT("RoundStart.wav"), NULL, SND_FILENAME | SND_ASYNC); //nhac vao game
		break;
	}
	case 5:
	{
		PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_ASYNC); //chien thang
		break;
	}
	}
	return 0;
}
//
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConCurInf;

	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = CursorVisibility;

	SetConsoleCursorInfo(handle, &ConCurInf);
}; 
void CAPNHAT_X()
{
	textcolor(Pink);
	gotoxy(BOARD_SIZE * 2 + 56, BOARD_SIZE * 2 - 10);
	cout << " P2";
	textcolor(White);

	gotoxy(BOARD_SIZE * 2 + 38, BOARD_SIZE * 2 - 10);
	cout << " P1";

	// bôi đen X
	textcolor(Black);
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 5);
	cout << "ooo   ooo" << endl;
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 4);
	cout << " ooo ooo" << endl;
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 3);
	cout << "   ooo" << endl;
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 2);
	cout << " ooo ooo " << endl;
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 1);
	cout << "ooo   ooo" << endl;
	// tô màu O
	textcolor(BLUE);
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 5);
	cout << " oooo" << endl;
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 4);
	cout << "o    o" << endl;
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 3);
	cout << "o    o" << endl;
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 2);
	cout << "o    o" << endl;
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 1);
	cout << " oooo" << endl;

}
void CAPNHAT_O()
{
	textcolor(Pink);
	gotoxy(BOARD_SIZE * 2 + 38, BOARD_SIZE * 2 - 10);
	cout << " P1";

	textcolor(White);
	gotoxy(BOARD_SIZE * 2 + 56, BOARD_SIZE * 2 - 10);
	cout << " P2";
	//bôi màu O
	textcolor(Black);
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 5);
	cout << " oooo" << endl;
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 4);
	cout << "o    o" << endl;
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 3);
	cout << "o    o" << endl;
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 2);
	cout << "o    o" << endl;
	gotoxy(BOARD_SIZE * 2 + 55, BOARD_SIZE * 2 - 1);
	cout << " oooo" << endl;
	// Tô màu X
	textcolor(RED);
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 5);
	cout << "ooo   ooo" << endl;
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 4);
	cout << " ooo ooo" << endl;
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 3);
	cout << "   ooo" << endl;
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 2);
	cout << " ooo ooo " << endl;
	gotoxy(BOARD_SIZE * 2 + 35, BOARD_SIZE * 2 - 1);
	cout << "ooo   ooo" << endl;
}
void  Checkboard()
{
	ShowCur(1);
//	gotoxy(_X, _Y);
	if (_A[h][k].c == 0)
		
			if (TURN == true)
			{
				PlaySoundEffect(2);
				textcolor(RED);
				cout << "X";
				dem++;
				// bôi đen X
				textcolor(RED);
				gotoxy(BOARD_SIZE * 2 + 38, BOARD_SIZE * 2 - 24);
				cout << dem;
				CAPNHAT_X();
				TURN = false;
				_A[h][k].c = 1;
			}
			else {
				PlaySoundEffect(2);
				textcolor(BLUE);
				cout << "O";
				demy++;
				//bôi màu O
				textcolor(BLUE);
				gotoxy(BOARD_SIZE * 2 + 58, BOARD_SIZE * 2 - 24);
				cout << demy;
				CAPNHAT_O();
				TURN = true;
				_A[h][k].c = -1;
			}
	gotoxy(_X, _Y);
}
void processCheckBoard()
{
	// Vẽ cột phải
	for (int i = 1; i <= BOARD_SIZE*2 + 1; i++)
	{
			gotoxy(BOARD_SIZE * 6 + LEFT, i + TOP);
		if ((i + 1) % 2 == 1)
			cout << char(21);
		else
			cout << char(23);
	}
	// Vẽ dòng cuối
	for (int j = 0; j < 3 * BOARD_SIZE  + 1 - 11; j++)
	{
			gotoxy(j + LEFT + BOARD_SIZE*2 + 27, TOP + 1 + BOARD_SIZE*2);
		if (j % 2 == 0)
			cout << char(29);
		else
			cout << char(3);
	}
	// Vẽ cột trái
	for (int i = BOARD_SIZE*2 + 1; i > 1; i--)
	{
		gotoxy(LEFT + BOARD_SIZE*2 + 27, i + TOP);
		if ((i + 1) % 2 == 1)
			cout << char(21);
		else
			cout << char(23);
	}
	// Vẽ dòng đầu 
	for (int j = 0; j < 3 * BOARD_SIZE  + 1 - 11; j++)
	{
			gotoxy(j + LEFT + BOARD_SIZE*2 + 27, TOP + 1);
		if (j % 2 == 0)
			cout << char(5);
		else
			cout << char(127);
	}
	// Vẽ dòng ngang nằm giữa thứ 1
	for (int j = 0; j < 3 * BOARD_SIZE  + 1 - 11; j++)
	{
		gotoxy(j + LEFT + BOARD_SIZE*2 + 27, BOARD_SIZE*2 - 28);
		if (j % 2 == 0)
			cout << char(5);
		else
			cout << char(127);
	}
	// Vẽ dòng ngang nằm giữa thứ 2
	for (int j = 0; j < 3 * BOARD_SIZE + 1 - 11; j++)
	{
			gotoxy(j + LEFT + BOARD_SIZE*2 + 27, BOARD_SIZE*2 - 26);
		if (j % 2 == 0)
			cout << char(5);
		else
			cout << char(127);
	}
	// Vẽ dòng ngang nằm giữa thứ 3
	for (int j = 0; j < 3 * BOARD_SIZE  + 1 - 11; j++)
	{
			gotoxy(j + LEFT + BOARD_SIZE*2 + 27, BOARD_SIZE*2 - 21);
		if (j % 2 == 0)
			cout << char(5);
		else
			cout << char(127);
	}
	// Vẽ cột ngang nằm giữa thứ 4
	for (int j = 0; j < 3 * BOARD_SIZE  + 1 - 11; j++)
	{
			gotoxy(j + LEFT + BOARD_SIZE*2 + 27, BOARD_SIZE*2 - 19);
		if (j % 2 == 0)
			cout << char(5);
		else
			cout << char(127);
	}
	// Vẽ cột ngang nằm giữa thứ 5
	for (int j = 0; j < 3 * BOARD_SIZE  + 1 - 11; j++)
	{
			gotoxy(j + LEFT + BOARD_SIZE*2 + 27, BOARD_SIZE*2 - 12);
		if (j % 2 == 0)
			cout << char(5);
		else
			cout << char(127);
	}
	// Vẽ cột ngang nằm giữa thứ 6
	for (int j = 0; j < 3 * BOARD_SIZE  + 1 - 11; j++)
	{
			gotoxy(j + LEFT + BOARD_SIZE*2 + 27, BOARD_SIZE*2 - 10);
		if (j % 2 == 0)
			cout << char(5);
		else
			cout << char(127);
	}
	// Vẽ cột giữa thứ 1
	for (int i = 1; i <= BOARD_SIZE *2/ 3 - 7; i++)
	{
		gotoxy(BOARD_SIZE*2 + 48, i + BOARD_SIZE*2 - 31);
		if ((i + 1) % 2 == 1)
			cout << char(21);
		else
			cout << char(23);
	}
	// Vẽ cột giữa thứ 2
	for (int i = 1; i <= BOARD_SIZE*2 / 3 - 3; i++)
	{
			gotoxy(BOARD_SIZE*2 + 48, i + BOARD_SIZE*2 - 27);
		if ((i + 1) % 2 == 1)
			cout << char(21);
		else
			cout << char(23);
	}
	// Vẽ cột giữa thứ 3
	for (int i = BOARD_SIZE *2/ 3 - 3; i <= BOARD_SIZE ; i++)
	{
			gotoxy(BOARD_SIZE*2 + 48, i + BOARD_SIZE*2 - 27);
		if ((i + 1) % 2 == 1)
			cout << char(21);
		else
			cout << char(23);
	}
	// Vẽ cột giữa thứ 4
	for (int i = BOARD_SIZE*2 / 3 - 3; i <= BOARD_SIZE +2; i++)
	{
			gotoxy(BOARD_SIZE*2 + 48, i + BOARD_SIZE*2 - 17);
		if ((i + 1) % 2 == 1)
			cout << char(21);
		else
			cout << char(23);
	}
	// Xuất nội dung bảng chức năng 
	textcolor(Yellow);
	gotoxy(BOARD_SIZE*2 + 44, BOARD_SIZE*2 - 27);
	cout << "So luot di";
	textcolor(RED);
	gotoxy(BOARD_SIZE*2 + 38, BOARD_SIZE*2 - 29);
	cout << "X";
	textcolor(BLUE);
	gotoxy(BOARD_SIZE*2 + 58, BOARD_SIZE*2 - 29);
	cout << "Y";
	textcolor(Green);
	gotoxy(BOARD_SIZE*2 + 38, BOARD_SIZE*2 - 11);
	cout << "den luot cua nguoi choi ";
	textcolor(Cyan);
	gotoxy(BOARD_SIZE*2 + 43, BOARD_SIZE*2 - 20);
	cout << "so tran thang ";
	gotoxy(BOARD_SIZE * 2 + 43-4, BOARD_SIZE * 2 - 20 + 4); cout << Score_p1;
	gotoxy(BOARD_SIZE * 2 + 43 + 14, BOARD_SIZE * 2 - 20 + 4); cout<<Score_p2;
	gotoxy(1, 37);
	textcolor(RED);
	cout << "Press Q to save game";
	gotoxy(1, 38);
	cout << "Press ESC to quit game";
	textcolor(White);
}
//
void Reset() {
	for (int i = 0; i < BOARD_SIZE - 1; i++) {
		for (int j = 0; j < BOARD_SIZE - 1; j++) {
			_A[i][j].x = LEFT + 3 + j * 3;
			_A[i][j].y = 2 * i + TOP + 3;
			_A[i][j].c = 0;
		}
	}
	_COMMAND = -1;
	TURN = true;
	_X = _A[0][0].x;
	_Y = _A[0][0].y;
	KTLOADGAME = false;
};


void starGame() 
{
	ShowCur(1);
	system("cls");
	PlaySoundEffect(4);
	Draw();
	processCheckBoard();
	if (KTLOADGAME == true)  //lấy dữ liệu cũ ra, ko reset nếu load game
	{
		for (int i = 0; i < BOARD_SIZE - 1; i++) {
			for (int j = 0; j < BOARD_SIZE - 1; j++) {
				_A[i][j].x = LEFT + 3 + j * 3;
				_A[i][j].y = 2 * i + TOP + 3;
			}
		}
	//	cout << Score_p1 << " " << Score_p2 << " " << dem << " " << demy;
		for (int i = 0; i < BOARD_SIZE - 1; i++)
		{
			for (int j = 0; j < BOARD_SIZE - 1; j++)
			{
				if (_A[i][j].c == 1)
				{
					textcolor(RED);
						gotoxy(_A[i][j].x, _A[i][j].y);
						cout << "X";
				}
				else
					if (_A[i][j].c == -1)
					{
						textcolor(BLUE);
						gotoxy(_A[i][j].x, _A[i][j].y);
						cout << "0";
					}
			}
		}
			_COMMAND = -1;

		_X = _A[0][0].x;
		_Y = _A[0][0].y;


		KTLOADGAME = false;

		if (demy >= dem)
		{
			TURN = true;
			CAPNHAT_O();
		}
		else

		{
			TURN = false; CAPNHAT_X();
		}
	}
	else Reset();
	h = 0; k = 0;
	gotoxy(_X, _Y);
}

//	gotoxy(_X, _Y);
void P1()
{	
	system("cls");
	PlaySoundEffect(5);
	int i = 5, j = 10;
	gotoxy(i, j);     cout << " .----------------.  .----------------.   .----------------.  .----------------.  .-----------------.";
	gotoxy(i, j + 1); cout << "| .--------------. || .--------------. | | .--------------. || .--------------. || .--------------. |";
	textcolor(rand() % 15 + 1);
	gotoxy(i, j + 2); cout << "| |   ______     | || |     __       | | | | _____  _____ | || |     _____    | || | ____  _____  | |";
	gotoxy(i, j + 3); cout << "| |  |_   __ " << char(92) << "   | || |    /  |      | | | ||_   _||_   _|| || |    |_   _|   | || ||_   " << char(92) << "|_   _| | |";
	gotoxy(i, j + 4); cout << "| |    | |__) |  | || |    `| |      | | | |  | | /" << char(92) << " | |  | || |      | |     | || |  |   " << char(92) << " | |   | |";
    gotoxy(i, j + 5); cout << "| |    |  ___/   | || |     | |      | | | |  | |/  " << char(92) << "| |  | || |      | |     | || |  | |" << char(92) << " " << char(92) << "| |   | |";
	gotoxy(i, j + 6); cout << "| |   _| |_      | || |    _| |_     | | | |  |   /" << char(92) << "   |  | || |     _| |_    | || | _| |_" << char(92) << "   |_  | |";
	gotoxy(i, j + 7); cout << "| |  |_____|     | || |   |_____|    | | | |  |__/  " << char(92) << "__|  | || |    |_____|   | || ||_____|" << char(92) << "____| | |";
	gotoxy(i, j + 8); cout << "| |              | || |              | | | |              | || |              | || |              | |";
	textcolor(rand() % 15 + 1);
    gotoxy(i, j + 9); cout << "| '--------------' || '--------------' | | '--------------' || '--------------' || '--------------' |";
	gotoxy(i, j + 10); cout << " '----------------'  '----------------'   '----------------'  '----------------'  '----------------' ";
}
void P2()
{
	system("cls");
	PlaySoundEffect(5);
	int i = 5, j = 10;
	gotoxy(i, j);     cout << " .----------------.  .----------------.   .----------------.  .----------------.  .-----------------.";
	gotoxy(i, j + 1); cout << "| .--------------. || .--------------. | | .--------------. || .--------------. || .--------------. |";
	(rand() % 15 + 1);
	gotoxy(i, j + 2); cout << "| |   ______     | || |    _____     | | | | _____  _____ | || |     _____    | || | ____  _____  | |";
	gotoxy(i, j + 3); cout << "| |  |_   __ " << char(92) << "   | || |   / ___ `.   | | | ||_   _||_   _|| || |    |_   _|   | || ||_   " << char(92) << "|_   _| | |";
	gotoxy(i, j + 4); cout << "| |    | |__) |  | || |  |_/___) |   | | | |  | | /" << char(92) << " | |  | || |      | |     | || |  |   " << char(92) << " | |   | |";
	gotoxy(i, j + 5); cout << "| |    |  ___/   | || |   .'____.'   | | | |  | |/  " << char(92) << "| |  | || |      | |     | || |  | |" << char(92) << " " << char(92) << "| |   | |";
    gotoxy(i, j + 6); cout << "| |   _| |_      | || |  / /____     | | | |  |   /" << char(92) << "   |  | || |     _| |_    | || | _| |_" << char(92) << "   |_  | |";
    gotoxy(i, j + 7); cout << "| |  |_____|     | || |  |_______|   | | | |  |__/  " << char(92) << "__|  | || |    |_____|   | || ||_____|" << char(92) << "____| | |";
	gotoxy(i, j + 8); cout << "| |              | || |              | | | |              | || |              | || |              | |";
	textcolor(rand() % 15 + 1);
	gotoxy(i, j + 9); cout << "| '--------------' || '--------------' | | '--------------' || '--------------' || '--------------' |";
	gotoxy(i, j + 10); cout << " '----------------'  '----------------'   '----------------'  '----------------'  '----------------' ";
}
void HOA()  //hieu ung hoa
{
	PlaySoundEffect(5);
	system("cls");

	gotoxy(35, 10); cout << endl;
	cout << "     HH            HH         OOOOOO            AAAAAAAAAAA   \n";
	cout << "     HH            HH       OO     OOO          AAA      AAA  \n";
	cout << "     HH            HH     OO         OO         AAA       AAA  \n";
	cout << "     HH            HH     OO          OO        AAA        AAA  \n";
	cout << "     HH            HH     OO          OOO       AAAAAAAAAAAAAAA  \n";
	cout << "     HHHHHHHHHHHHHHHH     OO          OOO       AAAAAAAAAAAAAAAA  \n";
	cout << "     HH            HH     OO          OOO       AAA         AAAAA  \n";
	cout << "     HH            HH     OO          OOO       AAA          AAAAA   \n";
	cout << "     HH            HH      OO         OOO       AAA          AAAAAA   \n";
	cout << "     HH            HH       OO      OOO         AAA           AAAAAA   \n";
	cout << "     HH            HH         OOOOOOO           AAA            AAAAAA  \n";


}


bool KT_doc(int n, int m)
{
	int d1 = 0;
	int d2 = 0;
	int i = 0;
	while (1)
	{
		i++;
		if ((m - i) < 0)  break;
		if ((_A[n][m - i].c) == 0) break;

		if ((_A[n][m - i].c) != (_A[n][m].c)) break;
		else d1++;
	}
	i = 0;
	while (2)
	{
		i++;;
		if ((m + i) > (BOARD_SIZE - 2)) break;
		if ((_A[n][m + i].c) == 0) break;
		else
			if ((_A[n][m + i].c) != (_A[n][m].c)) break;
			else d2++;
	}
	if ((d1 + d2) >= 4) return true;
	else return false;
}

bool KT_ngang(int n, int m)
{
	int d1 = 0;
	int d2 = 0;
	int i = 0;
	while (1)
	{
		i++;;
		if ((n - i) < 0) break;
		if ((_A[n - i][m].c) == 0) break;
		if ((_A[n - i][m].c) != (_A[n][m].c)) break;
		else
			d1++;
	}
	i = 0;
	while (2)
	{
		i++;;
		if ((n + i) > (BOARD_SIZE - 2)) break;
		if ((_A[n + i][m].c) == 0) break;
		else
			if ((_A[n + i][m].c) != (_A[n][m].c))	break;
			else
				d2++;
	}
	if ((d1 + d2) >= 4) return true;
	else return false;
}
bool KT_cheo_chinh(int n, int m)
{
	int d1 = 0;
	int d2 = 0;
	int i = 0;
	while (1)
	{
		i++;
		if (((n - i) < 0) || ((m - i) < 0)) break;
		if ((_A[n - i][m - i].c) == 0) break;

		if ((_A[n - i][m - i].c) != (_A[n][m].c)) break;
		else d1++;
	}
	i = 0;
	while (2)
	{
		i++;
		if (((n + i) > BOARD_SIZE - 2) || ((m + i) > BOARD_SIZE - 2)) break;
		if ((_A[n + i][m + i].c) == 0) break;

		if ((_A[n + i][m + i].c) != (_A[n][m].c)) break;
		else d2++;
	}

	if ((d1 + d2) >= 4) return true;
	else return false;
}

bool KT_cheo_phu(int n, int m)
{
	int d1 = 0;
	int d2 = 0;
	int i = 0;
	while (1)
	{
		i++;
		if (((n - i) < 0) || ((m + i) > BOARD_SIZE - 2)) break;
		if ((_A[n - i][m + i].c) == 0) break;

		if ((_A[n - i][m + i].c) != (_A[n][m].c)) break;
		else d1++;
	}
	i = 0;
	while (2)
	{
		i++;
		if (((n + i) > BOARD_SIZE - 2) || ((m - i) < 0)) break;
		if ((_A[n + i][m - i].c) == 0) break;

		if ((_A[n + i][m - i].c) != (_A[n][m].c)) break;
		else d2++;
	}

	if ((d1 + d2) >= 4) return true;
	else return false;
}

void MoveRight() {
	PlaySoundEffect(3);
	if (_X < _A[BOARD_SIZE - 2][BOARD_SIZE - 2].x)
	{
		l = true;
		h++;
		_X += 3;
		gotoxy(_X, _Y);
	}
};

void MoveLeft() {
	PlaySoundEffect(3);
	if (_X > _A[0][0].x)
	{
		l = true;
		h--;
		_X -= 3;
		gotoxy(_X, _Y);
	}
};

void MoveDown() {
	PlaySoundEffect(3);
	if (_Y < _A[BOARD_SIZE - 2][BOARD_SIZE - 2].y)
	{
		k++;

		l = true;
		_Y += 2;
		gotoxy(_X, _Y);
	}
};
void MoveUp() {
	PlaySoundEffect(3);
	if (_Y > _A[0][0].y)
	{
		k--;

		_Y -= 2;
		gotoxy(_X, _Y);
		l = true;
	}
};
bool KT()
{
	if (KT_doc(h, k) == true) return true;
	if (KT_ngang(h, k) == true) return true;
	if (KT_cheo_chinh(h, k) == true) return true;
	if (KT_cheo_phu(h, k) == true) return true;
	return false;
}
int _continue()
{
	int i = 40, j = 25;

	char cmd;
	_cout(i, j, "Ban co muon tiep tuc choi khong (Y/N) : ");
	cin >> cmd;
			if (cmd == 'n') return 0;
			else return 1;
}
int START()
{

	int k = 0, temp, check = 0;
	while (1) {
		_COMMAND = toupper(_getch());
		if (_COMMAND == 27)
		{
			Score_p1 = 0;
			Score_p2 = 0;
			dem = 0;
			demy = 0;
			return 0;
		}
		else if (_COMMAND == 'Q')
		{
			save();
			return 0;
		}
		else {
			if ((_COMMAND == 'A') || (int(_COMMAND) == 75)) MoveLeft();
			else if ((_COMMAND == 'W') || (int(_COMMAND) == 72)) MoveUp();
			else if ((_COMMAND == 'S') || (int(_COMMAND) == 80)) MoveDown();
			else if ((_COMMAND == 'D') || (int(_COMMAND) == 77)) MoveRight();
			else if (_COMMAND == 13)
			{
				textcolor(White);
				Checkboard();
				if (KT() == true)
				{
					if (_A[(_X - 6) / 3][(_Y - 4) / 2].c == 1)
					{
						P1();

						temp = _continue();
						if (temp == 0)
						{
							Score_p2 = 0;
							check = 1;
						}
						else
						{
							Score_p2++;
							starGame();
						}

					}
					else
					{
						P2();

						temp = _continue();
						if (temp == 0)
						{
							Score_p1 = 0;
							check = 1;
						}
						else
						{
							Score_p1++;
							starGame();
						}


					};
				}
				else
					if (dem + demy == (BOARD_SIZE - 1) * (BOARD_SIZE - 1))
					{
						HOA();
						dem = 0;
						demy = 0;
						temp = _continue();
						if (temp == 0)
						{
							Score_p1 = 0;
							Score_p2 = 0;
							check = 1;
						}
						else {
							starGame();
						}
					}
				if (check == 1) break;

			}
		}
	};
	return 0;
}

void _cout(int x, int y, string c)
{
	gotoxy(x, y);
	cout << c;
};
void PrintCaro()
{
	SetConsoleTextAttribute(hConsoleColor, 12);
	int x = 25, y = 10;
	gotoxy(x, y);				   cout << "ooooooooooooo";
	gotoxy(x - 3, y + 1);			cout << "oooooooooooooooo";
	gotoxy(x - 5, y + 2);    cout << "oooooooooooooooooo";
	gotoxy(x - 6, y + 3);   cout << "ooooooooooooooooooo";

	gotoxy(x - 7, y + 4);  cout << "ooooooo";
	gotoxy(x - 8, y + 5); cout << "ooooooo";
	gotoxy(x - 8, y + 6); cout << "ooooooo";
	gotoxy(x - 8, y + 7); cout << "ooooooo";
	gotoxy(x - 8, y + 8); cout << "ooooooo";
	gotoxy(x - 8, y + 9); cout << "ooooooo";
	gotoxy(x - 8, y + 10); cout << "ooooooo";
	gotoxy(x - 7, y + 11); cout << "ooooooo";
	gotoxy(x - 6, y + 12);  cout << "ooooooooooooooooooo";
	gotoxy(x - 5, y + 13);   cout << "oooooooooooooooooo";

	gotoxy(x - 3, y + 14);		cout << "oooooooooooooooo";
	gotoxy(x, y + 15);			   cout << "ooooooooooooo";
	SetConsoleTextAttribute(hConsoleColor, 14);
	gotoxy(x - 4 + 22, y + 4);				    cout << "ooooooooooo";
	gotoxy(x - 4 + 22 - 2, y + 5);		      cout << "ooooooooooooooo";
	gotoxy(x - 4 + 22 - 3, y + 6);		     cout << "ooooooooooooooooo";

	gotoxy(x - 4 + 22 - 3, y + 7);			 cout << "ooooooooooooooooo";
	gotoxy(x - 4 + 22 - 3, y + 8);			 cout << "oooooo     oooooo";
	gotoxy(x - 4 + 22 - 3, y + 9);			 cout << "oooooo     oooooo";

	gotoxy(x - 4 + 22 - 3, y + 10);		 cout << "oooooo     oooooo";
	gotoxy(x - 4 + 22 - 3, y + 11);		 cout << "ooooooooooooooooo";
	gotoxy(x - 4 + 22 - 3, y + 12);		 cout << "oooooo     oooooo";

	gotoxy(x - 4 + 22 - 3, y + 13);		 cout << "oooooo     oooooo";
	gotoxy(x - 4 + 22 - 3, y + 14);		 cout << "oooooo     oooooo";
	gotoxy(x - 4 + 22 - 3, y + 15);		 cout << "oooooo     oooooo";
	SetConsoleTextAttribute(hConsoleColor, 9);
	gotoxy(x + 33, y + 4);			cout << "ooooo   ooooooooo";
	gotoxy(x + 33, y + 5);			cout << "oooooooooooooooooo";
	gotoxy(x + 33, y + 6);			cout << "ooooooooooooooooooo";

	gotoxy(x + 33, y + 7);			cout << "oooooooooooooooooooo";
	gotoxy(x + 1 + 33, y + 8);			 cout << "ooooooo     ooooooo";
	gotoxy(x + 1 + 33, y + 9);			 cout << "ooooooo     ooooooo";

	gotoxy(x + 1 + 33, y + 10);		 cout << "ooooooo";
	gotoxy(x + 1 + 33, y + 11);		 cout << "ooooooo";
	gotoxy(x + 1 + 33, y + 12);		 cout << "ooooooo";

	gotoxy(x + 1 + 33, y + 13);		 cout << "ooooooo";
	gotoxy(x + 1 + 33, y + 14);		 cout << "ooooooo";
	gotoxy(x + 1 + 33, y + 15);		 cout << "ooooooo";
	SetConsoleTextAttribute(hConsoleColor, 10);
	gotoxy(x + 57, y + 4);					cout << "ooooooooooo";
	gotoxy(x + 57 - 2, y + 5);			  cout << "ooooooooooooooo";
	gotoxy(x + 57 - 3, y + 6);			 cout << "ooooooooooooooooo";
	gotoxy(x + 57 - 3, y + 7);			 cout << "ooooooooooooooooo";

	gotoxy(x + 57 - 3, y + 8);			 cout << "oooooo     oooooo";
	gotoxy(x + 57 - 3, y + 9);			 cout << "oooooo     oooooo";
	gotoxy(x + 57 - 3, y + 10);		 cout << "oooooo     oooooo";
	gotoxy(x + 57 - 3, y + 11);		 cout << "oooooo     oooooo";

	gotoxy(x + 57 - 3, y + 12);		 cout << "ooooooooooooooooo";
	gotoxy(x + 57 - 3, y + 13);		 cout << "ooooooooooooooooo";

	gotoxy(x + 57 - 2, y + 14);		  cout << "ooooooooooooooo";
	gotoxy(x + 57, y + 15);				cout << "ooooooooooo";
	SetConsoleTextAttribute(hConsoleColor, 15);
}
void about_us()
{
	int q = 0;
	int i = 40, j = 15;
	system("cls");
	do
	{
		textcolor(Green);
		_cout(i,j, "20120354  ");
		textcolor(Cyan);
		_cout(i + 10, j, "DANG   HUYNH   CUU   QUAN");
		//
		textcolor(Green);
		_cout(i, j+2, "20120385  ");
		textcolor(Cyan);
		_cout(i + 10, j+2, "TRAN   HOANG   TIN");
		//
		textcolor(Green);
		_cout(i, j+4, "20120349  ");
		textcolor(Cyan);
		_cout(i + 10, j+4, "NGO    HUU     PHUC");
		//
		textcolor(Green);
		_cout(i, j+6, "20120370  ");
		textcolor(Cyan);
		_cout(i + 10, j+6, "TRIEU  QUOC   THAI");
		//
		textcolor(Green);
		_cout(i, j+8, "20120440  ");
		textcolor(Cyan);
		_cout(i + 10, j+8, "LE     TRUONG   BAO   CHAU");
		//
		textcolor(White);
		for (int m = 0; m < 13; m++)
		{
			_cout(i - 2, j-2 + m, "|");
			_cout(i + 37, j-2 + m, "|");
		};
		for (int m = 0; m < 39; m++)
		{
			_cout(i - 2 + m, j - 2, "-");
		};
		textcolor(Yellow);
		_cout(i - 2, j - 2, "+"); _cout(i + 37, j - 2, "+");
		_cout(i - 2, j + 10, "+"); _cout(i + 37, j + 10, "+");
		_cout(i + 13, j + 10, "Nhom 14 !!");
		textcolor(White);
		for (int m = 0; m < 14; m++)
		{
			_cout(i - 2+m+1, j + 10 , "-");
		};
		for (int m = 0; m < 15; m++)
		{
			_cout(i - 2 + 14 + 10 + m, j + 10, "-");
		};
		textcolor(7);
		_cout(i - 1+5, j + 11, "(Press ESC or ENTER to quit)");
			
		_COMMAND = toupper(_getch());
		if ((_COMMAND == 27) or (_COMMAND == 13)) q = 1;
	} while (q != 1);

}
void Instruction()
{
	system("cls");
	int i = 10, j = 20, q = 0;
	do
	{
		_cout(i, j - 4, "Game has a 12x12 squares. Player take turn to putting  their marks in empty square");
		_cout(i, j - 2, "The First player  to get 5 of his mark  in a row (up, down, across, or diagonally) is the winner.");
		_cout(i, j, "Press ");
		textcolor(RED);
		_cout(i + 7, j, "A,");
		textcolor(BLUE);
		_cout(i + 9, j, "S,");
		textcolor(Green);
		_cout(i + 11, j, "D,");
		textcolor(Yellow);
		_cout(i + 13, j, "W,");
		textcolor(White);
		_cout(i + 15, j, "to move the cursor");
		_cout(i, j + 2, "Press ");
		textcolor(Yellow);
		_cout(i + 7, j + 2, "ENTER");
		textcolor(White);
		_cout(i + 14, j + 2, "to");
		textcolor(RED);
		_cout(i + 17, j+2, "X");
		textcolor(White);
		_cout(i + 19, j+2, "or");
		textcolor(BLUE);
		_cout(i + 22, j + 2, "O");
		textcolor(White);
		_cout(i, j + 4, "Press");
		textcolor(Yellow);
		_cout(i + 7, j + 4, "Q");
		textcolor(White);
		_cout(i + 9, j + 4, "to save Game");
		textcolor(Yellow);
		_cout(i, j + 6, "ESC");
		textcolor(White);
		_cout(i + 5, j + 6, "to quit Game");

		_COMMAND = toupper(_getch());
		if ((_COMMAND == 27) or (_COMMAND == 13)) q = 1;
	} while (q != 1);

};
void _Setcolor(int c)
{
	SetConsoleTextAttribute(hConsoleColor, c);
};
int _SelectCmd()
{


	ShowCur(0);
	int cn = 1;
	while (1)
	{
		_COMMAND = toupper(_getch());
		if (_COMMAND == 'S')
		{
			
			if (cn == 1)
			{
				cn++;
				system("cls");
				PrintCaro();
				_Setcolor(15);
				_cout(50, 30, A[0]);
				_Setcolor(14);
				_cout(49, 31, ">");
				_cout(50, 31, A[1]);
				_Setcolor(15);
				_cout(50, 32, A[2]);
				_cout(50, 33, A[3]);
				_cout(50, 34, A[4]);
			}
			else if (cn == 2)
			{
				cn++;
				system("cls");
				PrintCaro();
				_Setcolor(15);
				_cout(50, 30, A[0]);
				_cout(50, 31, A[1]);
				_Setcolor(14);
				_cout(49, 32, ">");
				_cout(50, 32, A[2]);
				_Setcolor(15);
				_cout(50, 33, A[3]);
				_cout(50, 34, A[4]);
			}
			else if (cn == 3)
			{
				cn++;
				system("cls");
				PrintCaro();
				_Setcolor(15);
				_cout(50, 30, A[0]);
				_cout(50, 31, A[1]);
				_cout(50, 32, A[2]);
				_Setcolor(14);
				_cout(49, 33, ">");
				_cout(50, 33, A[3]);
				_Setcolor(15);
				_cout(50, 34, A[4]);
			}
			else if (cn == 4)
			{
				cn++;
				system("cls");
				PrintCaro();
				_Setcolor(15);
				_cout(50, 30, A[0]);
				_cout(50, 31, A[1]);
				_cout(50, 32, A[2]);
				_cout(50, 33, A[3]);
				_Setcolor(14);
				_cout(49, 34, ">");
				_cout(50, 34, A[4]);
			}
			else if (cn == 5)
			{
				cn = 1;
				system("cls");
				PrintCaro();
				_Setcolor(14);
				_cout(49, 30, ">");
				_cout(50, 30, A[0]);
				_Setcolor(15);
				_cout(50, 31, A[1]);
				_cout(50, 32, A[2]);
				_cout(50, 33, A[3]);
				_cout(50, 34, A[4]);
			}
		}
		else if (_COMMAND == 'W')
		{
			
			if (cn == 1)
			{
				cn = 5;
				system("cls");
				PrintCaro();
				_Setcolor(15);
				_cout(50, 30, A[0]);
				_cout(50, 31, A[1]);
				_cout(50, 32, A[2]);
				_cout(50, 33, A[3]);
				_Setcolor(14);
				_cout(49, 34, ">");
				_cout(50, 34, A[4]);
			}
			else  if (cn == 2)
			{
				cn--;
				system("cls");
				PrintCaro();
				_Setcolor(14);
				_cout(49, 30, ">");
				_cout(50, 30, A[0]);
				_Setcolor(15);
				_cout(50, 31, A[1]);
				_cout(50, 32, A[2]);
				_cout(50, 33, A[3]);
				_cout(50, 34, A[4]);
			}
			else if (cn == 3)
			{
				cn--;
				system("cls");
				PrintCaro();
				_Setcolor(15);
				_cout(50, 30, A[0]);
				_Setcolor(14);
				_cout(49, 31, ">");
				_cout(50, 31, A[1]);
				_Setcolor(15);
				_cout(50, 32, A[2]);
				_cout(50, 33, A[3]);
				_cout(50, 34, A[4]);
			}
			else if (cn == 4)
			{
				cn--;
				system("cls");
				PrintCaro();
				_Setcolor(15);
				_cout(50, 30, A[0]);
				_cout(50, 31, A[1]);
				_Setcolor(14);
				_cout(49, 32, ">");
				_cout(50, 32, A[2]);
				_Setcolor(15);
				_cout(50, 33, A[3]);
				_cout(50, 34, A[4]);
			}
			else if (cn == 5)
			{
				cn--;
				system("cls");
				PrintCaro();
				_Setcolor(15);
				_cout(50, 30, A[0]);
				_cout(50, 31, A[1]);
				_cout(50, 32, A[2]);
				_Setcolor(14);
				_cout(49, 33, ">");
				_cout(50, 33, A[3]);
				_Setcolor(15);
				_cout(50, 34, A[4]);
			}
		}
		else if (_COMMAND == 13)
		{
		PlaySoundEffect(0);
			if (cn == 1)
				return 1;
			else if (cn == 2) return 2;
			else if (cn == 3) return 3;
			else if (cn == 4)return 4;
			else if (cn == 5)return 5;
		}
	}
	return 0;
};
void Menu()
{
		system("cls");
		PlaySoundEffect(1);
		PrintCaro();
		_Setcolor(14);
		_cout(49, 30, ">");
		_cout(50, 30, A[0]);
		_Setcolor(15);
		_cout(50, 31, A[1]);
		_cout(50, 32, A[2]);
		_cout(50, 33, A[3]);
		_cout(50, 34, A[4]);


}
//Ham save du lieu
void save()
{
	demf++;
	string str;
	//system("clr");
	gotoxy(1, 39);
	cout << "NAME:";
	//cin.getline(str, 30);
	getline(cin, str,'\n');
	str += ".txt";
	//demo
	ofstream filelist;
	filelist.open("List.txt", ios::out | ios::app);
	filelist << str << "\n";
	filelist.close();
	//
	ofstream FileDemo(str);
	_Loadfile[demf] = str;

	for (int i = 0; i < BOARD_SIZE - 1; i++)
	{
		for (int j = 0; j < BOARD_SIZE - 1; j++)
			FileDemo << _A[j][i].c << " ";

		FileDemo << "\n";
	}

	FileDemo << Score_p1 << " " << Score_p2 << " " << dem << " " << demy;

	FileDemo.close();

}

//Ham LOAD du lieu
void DICHUYENKHILOAD() // xuất các file da lưu
{
	system("cls");
	gotoxy(10, 3);
	int demfile = 1;
	cout << "  ESC DE VE MENU,'ENTER' de chon ";
	gotoxy(10, 4);
	cout << " 'W' DE di chuyen len,'S' DE di chuyen xuong!";
	ifstream filelist("List.txt");
	while (!filelist.eof())
	{
		char temp[255];
		filelist.getline(temp, 255);
		string line = temp;
		//cout << line << std::endl;
		_Loadfile[demfile] = line;
		demfile++;
	}
	filelist.close();
	char _COMMAND;
	int j = 1;
	demf = demfile;
	textcolor(White);
	for (int i = 1; i <= demf; i++)
	{

		gotoxy(3, i + 5);
		cout << _Loadfile[i] << endl;

	}  textcolor(BLUE);
	gotoxy(3, 6); cout << _Loadfile[1];
	while (1)
	{
		_COMMAND = toupper(_getch());
		if (_COMMAND == 'W')
		{

			if   (j > 1)
			{
				textcolor(White);
				gotoxy(3, j + 5);
				cout << _Loadfile[j];
				j--; textcolor(BLUE);
				gotoxy(3, j + 5);
				cout << _Loadfile[j];
			}
		}


		if (_COMMAND == 'S')
		{

			if (j < demf)
			{
				textcolor(White);
				gotoxy(3, j + 5);
				cout << _Loadfile[j];
				j++; textcolor(BLUE);
				gotoxy(3, j + 5);
				cout << _Loadfile[j];
			}
		}
		if (int(_COMMAND) == 13)
		{
			Load(_Loadfile[j]);
			KTLOADGAME = true;
			starGame();
			START();
			break;
		}
		if (int(_COMMAND) == 27)
		{
			break;
		}
	}


}
void Load(string fileName)    //Ham Load du lieu da luu
{
	system("cls");


	int v;
	ifstream ff(fileName);
	for (int i = 0; i < BOARD_SIZE - 1; i++)
		for (int j = 0; j < BOARD_SIZE - 1; j++)
		{
			ff >> v;
			_A[j][i].c = v;

		}
	ff >> Score_p1;
	ff >> Score_p2;
	ff >> dem;
	ff >> demy;

	//while (ff >> v)
	//	cout << v;
	ff.close();

}
