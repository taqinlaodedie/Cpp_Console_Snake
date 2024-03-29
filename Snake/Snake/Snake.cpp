#include "pch.h"
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "colors.h"

// Size of the map
#define MAPX	50
#define MAPY	25

#define TRUE	1
#define FALSE	0

using namespace std;

// 4 directions
enum dir {				
	UP, DOWN, LEFT, RIGHT
};
typedef unsigned char boolean;

class Snake {
private:
	struct {
		int x;
		int y;
	} snake[100], food;	// Position of the head and food
	enum dir direction;	// Directions to move
	boolean isFailed;	// 1 if failed, 0 if alive
	boolean isStarted;	// 1 if the game is started, 0 if not
	int len;			// length of the snake
public:
	void init();		// Constructor
	void drawMap();		// Draw the map
	void createFood();	// Create a new food to eat
	void drawFood();	// draw the created food
	void drawSnake();	// Draw the snake
	void clearSnake();	// Clear the last position
	void toPosition(int x, int y);	// Go to the position (x, y)
	void move();		// Move the snake
	void run();			// Run the game
	void getKey();	// Change the move direction
	boolean deadCheck();// Check if the snake is dead
	boolean eatCheck();	// Check if the snake ate the food
	void grow();		// The snake grows up
};

void Snake::init() {
	len = 3;			// Length is 3
	direction = RIGHT;	// Face the right
	// Start at (5, 5)
	snake[0].x = 5; snake[1].x = 4; snake[2].x = 3;
	snake[0].y = 5; snake[1].y = 5; snake[2].y = 5;
	isFailed = FALSE;
	isStarted = FALSE;
	drawMap();
	drawSnake();
	createFood();
	drawFood();
	toPosition(MAPX + 2, MAPY / 2);
	cout << "Press SPACE to start/pause" << endl;
}

void Snake::toPosition(int x, int y) {
	COORD pos = { x, y };
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, pos);
}

void Snake::drawMap() {
	toPosition(0, 0);
	for (int i = 0; i <= MAPX; i++)
		cout << BLUE << "-";
	toPosition(0, MAPY + 1);
	for (int i = 0; i <= MAPX; i++)
		cout << BLUE << "-";
	for (int i = 0; i <= MAPY + 1; i++) {
		toPosition(0, i);
		cout << BLUE << "|";
		toPosition(MAPX + 1, i);
		cout << BLUE << "|";
	}
}

void Snake::drawSnake() {
	toPosition(snake[0].x, snake[0].y);
	cout << RED << "@";	// Head of the snake
	for (int i = 1; i < len; i++) {
		toPosition(snake[i].x, snake[i].y);
		cout << GREEN << "#";	// Body of the snake
	}
}

void Snake::clearSnake() {
	toPosition(snake[len - 1].x, snake[len - 1].y);
	cout << " ";
}

void Snake::createFood() {
	food.x = rand() % MAPX + 1;
	food.y = rand() % MAPY + 1;
	for (int i = 0; i < len; i++) {
		if (food.x == snake[i].x && food.y == snake[i].y) {
			createFood();
			break;
		}
	}
}

void Snake::drawFood() {
	toPosition(food.x, food.y);
	cout << YELLOW << "*";
}

void Snake::move() {
	// Refresh the body
	for (int i = len - 1; i > 0; i--) {
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}
	// Refresh the head
	switch (direction) {
	case LEFT:
		snake[0].x--;
		break;
	case RIGHT:
		snake[0].x++;
		break;
	case UP:
		snake[0].y--;
		break;
	case DOWN:
		snake[0].y++;
		break;
	default: 
		break;
	}
}

void Snake::getKey() {
	char ch1, ch2;
	if (_kbhit()) {
		if (ch1 = _getch()) {
			ch2 = _getch();
			switch (ch2)
			{
			case 72:
				if(direction != DOWN)
					direction = UP;
				break;
			case 80:
				if (direction != UP)
					direction = DOWN;
				break;
			case 75:
				if (direction != RIGHT)
					direction = LEFT;
				break;
			case 77:
				if (direction != LEFT)
					direction = RIGHT;
				break;
			default:
				break;
			}
		}
	}
}

boolean Snake::deadCheck() {
	// Check if the snake eats itself
	for (int i = 1; i < len; i++) {
		if (snake[i].x == snake[0].x && snake[i].y == snake[0].y)
			return TRUE;
	}
	if (snake[0].x > MAPX || snake[0].x <= 0 || snake[0].y > MAPY || snake[0].y <= 0)
		return TRUE;
	return FALSE;
}

boolean Snake::eatCheck() {
	if (snake[0].x == food.x && snake[0].y == food.y)
		return TRUE;
	return FALSE;
}

void Snake::grow() {
	boolean check = eatCheck();
	if (check) {
		len++;
		createFood();
		drawFood();
	}	
}

void Snake::run() {
	init();
	while (TRUE) {
		if (isFailed) {
			system("cls");
			cout << "GAME OVER! Press any key to restart..." << endl;
			_getch();
			init();
			isFailed = FALSE;
		}
		else if (!isStarted) {
			if (_getch() == ' ')
				isStarted = TRUE;
		}
		else {
			isFailed = deadCheck();
			getKey();
			clearSnake();
			move();
			drawSnake();
			toPosition(MAPX + 2, MAPY / 2 + 1);
			cout << "Score: " << len << endl;
			grow();
			Sleep(100);
		}
	}
}

int main()
{
	Snake s;
	/*// Test the function move()
	s.init();
	for (int i = 0; i < 10; i++) {
		s.clearSnake();
		s.move();
		s.drawSnake();
		Sleep(1000);
	}*/
	s.run();

	return 0;
}

