
#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
#include <map>


typedef Vector2 pos;
typedef Texture2D img;

class game {
protected:
	const int screenWidth = 900;
	const int screenHeight = 500;
	int floorHeight = screenHeight - 100;

public:
	game() { // constructor 
		//screenWidth = GetScreenWidth();
		//screenHeight = GetScreenHeight();
		InitWindow(screenWidth, screenHeight, "Game");
	}
};

class player : game {

	bool isJumping = false;
	int jumpLimit = 50;

	void gravity() {
		if (!isJumping) {
			if (playerPos.y < floorHeight) {
				playerPos.y += 5.0f;
			}
		}
	}

	void moveRight() {
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) playerPos.x += 3.0f;
	}

	void moveLeft() {
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) playerPos.x -= 3.0f;
	}

	void jump() {
		if (!isJumping) {
			if (IsKeyDown(KEY_SPACE)) {
				isJumping = true;

					while(jumpLimit>0){
						playerPos.y -= 2.0f;
						jumpLimit--;
					}
				isJumping = false;
				if (playerPos.y == floorHeight) {
					jumpLimit = 50;
				}
			}
		}
	}

public:

	img pSprite = LoadTexture("images/png.png");
	pos playerPos;
	player() {
		playerPos = { float(screenWidth) / float(2), float(screenHeight) / float(2) };
	}



	void movement() {
		moveRight();
		moveLeft();
		jump();
		gravity();
	}

};


int main()
{
	SetTargetFPS(60);
	player* p = new player();
	while (!WindowShouldClose()) {

		p->movement();

		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexture(p->pSprite, p->playerPos.x, p->playerPos.y, WHITE);
		EndDrawing();
	}
	CloseWindow();
}