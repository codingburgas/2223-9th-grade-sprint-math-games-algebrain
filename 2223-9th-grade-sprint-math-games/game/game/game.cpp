
#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
#include <map>


typedef Vector2 pos;
typedef Texture2D texture;

class game {
protected:
	const int screenWidth = 1600;
	const int screenHeight = 800;
	int floorHeight = screenHeight - 100;

public:
	game() { // constructor 
		//screenWidth = GetScreenWidth();
		//screenHeight = GetScreenHeight();
		InitWindow(screenWidth, screenHeight, "Game");
	}
};

class player : game {
	void gravity() {
			if (playerPos.y < floorHeight) {
				playerPos.y += 5.0f;
		}
	}

	void moveRight() {
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) playerPos.x += 3.0f;
	}

	void moveLeft() {
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) playerPos.x -= 3.0f;
	}


public:

	texture pSprite = LoadTexture("images/png.png");
	pos playerPos;
	int lives = NULL;
	float health;
	float starting_health;

	player(float health) {
		floorHeight -= pSprite.height;
		playerPos = { 20, float(floorHeight)};
		this->health = health;
		starting_health = health;
	}

	void ShowLives() {
		Image heart = LoadImage("images/heart.png");
		ImageResize(&heart,50,50);
		texture heart_texture = LoadTextureFromImage(heart);
		for (int i = 0; i < lives; i++) {
			DrawTexture(heart_texture, 10 + (i*heart.width), 10, WHITE);
		}
	}

	void showHealth() {
		DrawRectangle(10, 10, 300, 50, RED);
		DrawRectangle(15, 15, 290, 40, BLACK);
		DrawRectangle(20, 20, 280* (health / starting_health), 30*(health/starting_health), RED);
	}

	void movement() {
		moveRight();
		moveLeft();
		gravity();
	}

};


int main()
{
	SetTargetFPS(60);
	player* p = new player(5);
	while (!WindowShouldClose()) {

		p->movement();

		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexture(p->pSprite, p->playerPos.x, p->playerPos.y, WHITE);
		p->showHealth();
		EndDrawing();
	}
	CloseWindow();
}