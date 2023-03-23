
#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
#include <map>


typedef Vector2 pos;
typedef Texture2D texture;

const int screenWidth = 1600;
const int screenHeight = 800;

class game {
protected:
	void gravity(pos& playerPos) {
		if (playerPos.y < floorHeight) {
			playerPos.y += 5.0f;
		}
	}

	void moveRight(pos& playerPos, int pWidth) {
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
			if (playerPos.x <= screenWidth - (pWidth/4) * 3)
				playerPos.x += 10.0f;
		}
	}

	void moveLeft(pos& playerPos, int pWidth) {
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
			if(playerPos.x >= 0-pWidth/4)
			playerPos.x -= 10.0f;
		}
	}

	int floorHeight = screenHeight - 100;

public:
};

// --------------------------- PLAYER CLASS ------------------------------

class player : game {
public:

	texture pSprite = LoadTexture("images/png.png");
	pos playerPos;
	int lives = NULL;
	float health;
	float starting_health;
	std::string username;

	player(float health, std::string username) {
		floorHeight -= pSprite.height;
		playerPos = { 20, float(floorHeight) };
		this->health = health;
		starting_health = health;
		this->username = username;
	}

	void ShowLives() {
		Image heart = LoadImage("images/heart.png");
		ImageResize(&heart, 50, 50);
		texture heart_texture = LoadTextureFromImage(heart);
		for (int i = 0; i < lives; i++) {
			DrawTexture(heart_texture, 10 + (i * heart.width), 10, WHITE);
		}
	}

	void showHealth() {
		DrawRectangle(10, 10, 300, 50, RED);
		DrawRectangle(15, 15, 290, 40, BLACK);
		DrawRectangle(20, 20, float(280) * (health / starting_health), 30, RED);
	}

	void movement() {
		moveRight(playerPos, pSprite.width);
		moveLeft(playerPos, pSprite.width);
		gravity(playerPos);
	}

};

void drawCenterLines() {
	DrawRectangle(screenWidth / 2, 0, 1, 10000, BLACK);
	DrawRectangle(0, screenHeight / 2, 10000, 1, BLACK);
}

void selectRectangle(bool& flag, Rectangle rec) {
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		if (CheckCollisionPointRec(GetMousePosition(), rec)) {
			flag = true;
		}
	}
}

// ----------------------------------- Main ----------------------------------

int main()
{
	SetTargetFPS(60);
	InitWindow(screenWidth, screenHeight, "Game");
	bool gameStart = false;
	bool clickedOnTextbox = false;
	const int maxNameLength = 10;
	char usernameInput[maxNameLength + 1];
	usernameInput[0] = '\0';
	int letterCount = 0;
	int key = 0;
	player* p = new player(1000, "");
	p->health -= 200;
	while (!WindowShouldClose()) {

		if (!gameStart) {
			//	Rectangle textbox = { screenWidth / 2 - 300 / 2, screenHeight / 2 - 50 / 2, 300,50 };
			//	selectRectangle(clickedOnTextbox, textbox);
			//	if (clickedOnTextbox) {
			//		key = GetKeyPressed();

			//		if ((key >= 32) && (key <= 125) && (maxNameLength < maxNameLength))
			//		{
			//			usernameInput[letterCount] = char(key);
			//			usernameInput[letterCount + 1] = '\0';
			//			letterCount++;
			//		}
			//		key = GetKeyPressed();
			//		if (IsKeyPressed(KEY_BACKSPACE)) {
			//			letterCount--;
			//			if (letterCount < 0) letterCount = 0;
			//			usernameInput[letterCount] = '\0';
			//		}
			//	}

			//	BeginDrawing();
			//	ClearBackground(LIGHTGRAY);
			//	DrawText(TextFormat("%i", key), 10, 10, 10, BLACK);
			//	drawCenterLines();
			//	DrawText("Input your username:", screenWidth / 2 - (MeasureText("Input your username:", 50) / 2), screenHeight / 2 - 100, 50, BLACK);
			//	DrawRectangleRec(textbox, GRAY);
			//	if (!clickedOnTextbox) DrawRectangleLines(textbox.x, textbox.y, textbox.width, textbox.height, BLACK);
			//	else DrawRectangleLines(textbox.x, textbox.y, textbox.width, textbox.height, RED);
			//	DrawText(usernameInput, screenWidth / 2 - 300 / 2, screenHeight / 2 - 50 / 2, 50, WHITE);

			//}

			BeginDrawing();
			ClearBackground(LIGHTGRAY);
			//drawCenterLines();
			DrawText("Chose your difficulty", screenWidth / 2 - (MeasureText("Input your username:", 50) / 2), screenHeight / 2 - 100, 50, BLACK);
			Rectangle inputDifRec = {screenWidth/2 - 400, screenHeight/2, 800, 200};
			DrawRectangleRec(inputDifRec, BLACK);
			std::map<std::string, Rectangle> difSelect;
			difSelect["Easy"] = {inputDifRec.x,inputDifRec.y,inputDifRec.width/3,inputDifRec.height};
			difSelect["Normal"] = { inputDifRec.x+inputDifRec.width/3,inputDifRec.y,inputDifRec.width / 3,inputDifRec.height};
			difSelect["Hard"] = { inputDifRec.x + (inputDifRec.width / 3)*2 ,inputDifRec.y,inputDifRec.width / 3,inputDifRec.height };
			for (auto itr = difSelect.begin(); itr != difSelect.end(); itr++) {
				DrawRectangleRec(itr->second,GRAY);
				DrawRectangleLinesEx(itr->second,4,LIGHTGRAY);
				DrawText(itr->first.c_str(), itr->second.x + itr->second.width/2 - (MeasureText(itr->first.c_str(), 30) / 2), itr->second.y + itr->second.height/2 - 15, 30, BLACK);
			}


		}
		else {

		ClearBackground(BLACK);
		p->movement();
		DrawTexture(p->pSprite, p->playerPos.x, p->playerPos.y, WHITE);
		p->showHealth();
		DrawRectangle(p->playerPos.x, p->playerPos.y, 10, 10, RED);
		DrawRectangle(p->playerPos.x+p->pSprite.width, p->playerPos.y, 10, 10, RED);
	}
		EndDrawing();
	}
	CloseWindow();
}
