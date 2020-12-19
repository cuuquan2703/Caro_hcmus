#include "Header.h"
using namespace std;
int main() {

	
	FixConsoleWindow();
	SetConsoleWindow(940,690); 
	SetScreenBufferSize(113, 46);
	DisableCtrButton(0, 1, 0);
	textcolor(4);
	int k = 0, k1 = 0;
	do
	{
		Menu();
		k=_SelectCmd();
		if (k == 1)
		{
			starGame();
			START();


		}
		else if (k == 4) about_us();
		else if (k == 3) Instruction();
		else if (k == 2) Load_Game();
		
	} while (k != 5);
	gotoxy(0, 0);
	_Setcolor(15);



	return 0;
}

