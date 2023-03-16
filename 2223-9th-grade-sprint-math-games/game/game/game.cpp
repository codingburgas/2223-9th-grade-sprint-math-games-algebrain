
#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
#include <map>

typedef Vector2 pos;

class game {
protected:
	const int screenWidth = 900;
	const int screenHeight = 500; 
	int floorHeight = 100;

public: 
	game() { // constructor 
		InitWindow(screenWidth, screenHeight, "Game");
	}
};

class player : game{
public: 
	player() {
		pos playerPos = { screenWidth / 2, screenHeight / 2 };
		while (!WindowShouldClose()) {


			BeginDrawing();
			ClearBackground(BLACK);
			DrawCircleV(playerPos, 50, WHITE);
			EndDrawing();
		}
		CloseWindow();
	}

};


int main()
{
	SetTargetFPS(60);
	player* p = new player();

}