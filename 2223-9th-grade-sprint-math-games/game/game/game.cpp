
#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
#include <map>

typedef Vector2 pos;

class game {
	const int screenWidth = 900;
	const int screenHeight = 500; 

public: 
	game() { // constructor 
		InitWindow(screenWidth, screenHeight, "Game");
		while (!WindowShouldClose()) {

			BeginDrawing();

			ClearBackground(BLACK);

			EndDrawing();

		}
		CloseWindow();

	}
};


int main()
{
	SetTargetFPS(60);
	game* g = new game();

}