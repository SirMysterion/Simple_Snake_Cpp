#include <stdio.h>
#include <iostream>
#include <cstdlib>
//#include <conio.h> //windows
#include <curses.h>
#include <termios.h>

static struct termios old, newer;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  newer = old; /* make new settings same as old settings */
  newer.c_lflag &= ~ICANON; /* disable buffered i/o */
  newer.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &newer); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
//char getch(void)
//{
//  return getch_(0);
//}

/* Read 1 character with echo */
char getche(void)
{
  return getch_(1);
}

/* Let's test it out */
//int main(void) {
//  char c;
//  printf("(getche example) please type a letter: ");
//  c = getche();
//  printf("\nYou typed: %c\n", c);
//  printf("(getch example) please type a letter...");
//  c = getch();
//  printf("\nYou typed: %c\n", c);
//  return 0;
//}

using namespace std;
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;



void Setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void Draw()
{
	system("clear"); //system("cls")
	for  (int i = 0; i < width+2; i++)
		cout << "#";
	cout << endl;

	for  (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0)
				cout << "#";
			if (i == y && j == x)
				cout << "0";
			else if (i == fruitY && j == fruitX)
				cout << "F";
			else
			{
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						print = true;
					}

				}
				if (!print)
					cout << " ";
			}
//				cout << " ";

			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}

	for (int i =0; i < width+2; i++)
		cout << "#";
	cout << endl;
	cout << "Score:" << score << endl;
}

void Input()
{
	char c;
	c = getche();
	//if (_kbhit())
	//{
		switch (c)
		{
			case 'a':
				dir = LEFT;
				break;
			case 'd':
				dir = RIGHT;
				break;
			case 's':
				dir = DOWN;
				break;
			case 'w':
				dir = UP;
				break;
			case 'x':
				gameOver = true;
				break;
		}
	//}

}

void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2x, prev2y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2x = tailX[i];
		prev2y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2x;
		prevY = prev2y;
	}

	switch (dir)
	{
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	}
	if (x > width-1 || x < 0 || y > height-1 || y < 0)
		gameOver = true;
	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;
	if (x == fruitX && y == fruitY)
	{
		score +=10;
		fruitX = rand() % width;
	        fruitY = rand() % height;
		nTail++;
	}
}


int main()
{
	Setup();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
		//sleep(10); //Sleep(10);
	}
	return 0;
}
