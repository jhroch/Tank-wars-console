#include <iostream>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <random>
#include <chrono> 

#define G 9.81


HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

// Variables
int battlefieldLenght = 110;
int deg, alpha, v0;
bool repeatingB = true;

void clearScreen() {

	system("cls");
}

double kmh2ms(int kmh) 
{
	double ms = kmh / 3.6;
	return ms;
}

double deg2rad(int deg)
{
	double rad = deg * 3.14159 / 180;
	return rad;
}

int shotDistance(int alpha, int v0) // Calculates shot distance by user inputs
{
	double radsin = sin(2 * deg2rad(alpha));
	double range = ((kmh2ms(v0) * kmh2ms(v0)) * radsin) / G ;
	int ranger = double (range);
	int rangeros = ranger / 100;
	if (0 < rangeros < 1)
	{
		return +2;
	}
	else
	{
		return rangeros;
	}
}

double shotTime(int alpha, int v0) // Calculates time of shot in the air
{
	double radsin = sin(deg2rad(alpha));
	double sTime = (kmh2ms(v0) * radsin) / G;
	return sTime;
}


double shotHeight(int alpha, int v0) // Calculates highest point of the shot
{
	double radsin = sin(deg2rad(alpha));
	double heights = ((kmh2ms(v0) * kmh2ms(v0)) * (radsin * radsin)) / (2 * G);
	round(heights);
	int height = 19 - (heights / 250);
	return height;
}

void gotoxy(int x, int y) // Position in console function
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void Write(int x, int y, std::string znak) // Write/Draw into console function
{
	gotoxy(x, y);
	if (x == 333)
	{
		znak = "";
	}
	else
	{
		std::cout << znak;
	}
}

class Player
{
private:
	int x;
	int y;
public:

	void Draw(int x, int y, char znak)
	{
		gotoxy(x, y);
		std::cout << znak;
	}
};


// Random number generator from 20 to 109
auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
int randomN = timenow % 89 + 20;

// Generate 2 players
Player tank1;
Player tank2;
// Players position
int r1 = 11;
int r2 = randomN;

void printGame() // Generate game
{
	// Write game info
	std::cout << "The length of the battlefield: 110km\n";
	std::cout << "One field = 1km\n";
	std::cout << "You are & [" << r1 << "]\n";
	std::cout << "Enemy is % [" << r2 << "]\n";
	// Draw players
	tank1.Draw(r1, 19, '&');
	tank2.Draw(r2, 19, '%');

	// Draw battlefield
	for (int i = 5; i < battlefieldLenght ; i++)
	{
		Write(i, 20, "-");
	}
	std::cout << "\n";
	// Asking for angle and speed input
	printf("Enter the angle of the shot [degree](max 89): ");
	scanf_s("%i", &alpha);

	printf("Enter the speed of the bullet [km/h]: ");
	scanf_s("%i", &v0);
	// If user entered invalid value end 
	if (alpha > 89) // If user entered more than 89 degree 
	{
		repeatingB = false;
		clearScreen();
		Write(40, 10, "ENTER OVER MAX ANGLE");
		for (int i = 0; i < 119; i++)
		{
			Write(i, 20, "-");
		}
	}

	else if (v0 > 9999) // If user entered more than 9999 kmph
	{
		repeatingB = false;
		clearScreen();
		Write(40, 10, "ENTER OVER MAX SPEED");
		for (int i = 0; i < 119; i++)
		{
			Write(i, 20, "-");
		}
	}
}

int hitX() // X Position of hit (*)
{
	int hit = shotDistance(alpha, v0) + r1;
	if (hit > 109)
	{
		return 333;
	}
	else
	{
		return hit;
	}
}

int hitHeightX() // X Position of highest point of trajectory (.)
{
	int hit = (shotDistance(alpha, v0) / 2) + r1;
	if (hit > 109)
	{
		return 54;
	}
	else
	{
		return hit;
	}
}

int hitHeightY() // Y Position of heightest point of trajectory (.)
{
	int hitHeightY = shotHeight(alpha, v0);
	if (hitHeightY < -2)
	{
		return 1;
	}
	else
	{
		return hitHeightY;
	}
}


int checkWin(int x) // Checks if user or pc lost
{
	if (x == r2)
	{
		return 1;
	}
	else if (x == 13)
	{
		return 2;
	}
	else
	{ 
		return 0;
	}
}

void repeating(int win) // Game loop function
{
	if (win == 1) // If you won
	{
		Write(hitX(), 19, "*");
		Write(hitHeightX(), hitHeightY(), ".");
		Write(hitX() - 3, 17, "[DEAD]");
		Write(42, 25, "Congratulation! YOU WON!");
		repeatingB = false;
		Sleep(999999);
	}
	else if (win == 2) // If you lost
	{
		Write(hitX(), 19, "*");
		Write(hitHeightX(), hitHeightY(), ".");
		Write(hitX() - 3, 17, "[DEAD]");
		Write(50, 25, "YOU LOST!");
		repeatingB = false;
		Sleep(999999);
	}
	else // If nobody lost keep playing
	{
		if (hitX() == 333)
		{
			std::cout << "Hit X: OVER";
		}
		else
		{
			std::cout << "Hit X: " << hitX();
		}
		Write(hitX(), 19, "*"); // Draw hit 
		Write(hitHeightX(), hitHeightY(), "."); // Draw heighest point (.)
		Sleep(5000);
		clearScreen();
		printGame();
	}
}


int main() // Main loop
	{
		printGame();
		while (repeatingB) {
			repeating(checkWin(hitX()));
		}
	}
