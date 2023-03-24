
#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
#include <map>
#include <functional>


typedef Vector2 pos;
typedef Texture2D texture;

const int screenWidth = 1200;
const int screenHeight = 600;

class game {
protected:
	void gravity(pos& playerPos, int pHeight) {
		if (playerPos.y < floorHeight - pHeight) {
			playerPos.y += 5.0f;
		}
	}

	void moveRight(pos& playerPos, int pWidth) {
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
			if (playerPos.x <= screenWidth - (pWidth / 4) * 3)
				playerPos.x += 10.0f;
		}
	}

	void moveLeft(pos& playerPos, int pWidth) {
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
			if (playerPos.x >= 0 - pWidth / 4)
				playerPos.x -= 10.0f;
		}
	}

	int floorHeight = screenHeight - 100;

	struct difficulty {
		std::string difName;
		bool isSelected = false;
	};

public:
};

// --------------------------- PLAYER CLASS ------------------------------

class player : game {
public:

	Image pSpriteImg = LoadImage("images/sprite.png");
	texture pSprite;
	pos playerPos;
	int lives = NULL;
	float health;
	float starting_health;
	std::string username;
	std::string usernameChosing;

	difficulty* dif = new difficulty();

	player() {
		ImageResize(&pSpriteImg, pSpriteImg.width / 6, pSpriteImg.height / 6);
		pSprite = LoadTextureFromImage(pSpriteImg);
		playerPos = { 20, (float)floorHeight - pSprite.height };
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
		gravity(playerPos, pSprite.height);
	}


};

void drawCenterLines() {
	DrawRectangle(screenWidth / 2, 0, 1, 10000, BLACK);
	DrawRectangle(0, screenHeight / 2, 10000, 1, BLACK);
}

void selectRectangle(bool& flag, Rectangle rec, std::vector<bool*>& v) {
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		if (CheckCollisionPointRec(GetMousePosition(), rec)) {
			if (v.size() > 1) {
				for (auto itr : v) {
					*itr = false;
					v.pop_back();
				}
			}
			v.push_back(&flag);
			flag = true;
		}
	}
}

struct text {
	std::string str;
	Color clr;
	int fontSize;
};


// ----------------------------------- Main ----------------------------------

int main()
{
	SetTargetFPS(60);
	InitWindow(screenWidth, screenHeight, "Game");
	std::vector<bool*> boolSelected;
	bool difSelected = false;
	const int maxNameLength = 10;
	int key = 0;
	bool usernameChosen = false;
	bool wait = false;
	bool clickedOnTextbox = false;
	bool clickedOnSubmit = false;
	text submitMessage = {"Submit", BLACK, 30 };
	std::string usernameMessage = "Input your username:";
	player* p = new player();
	while (!WindowShouldClose()) {
		BeginDrawing();
			static float time = GetFrameTime();
			if (!usernameChosen) {
					Rectangle textbox = { screenWidth / 2 - 300 / 2, screenHeight / 2 - 50 / 2, 300,50 };
					Rectangle submit = { screenWidth / 2 - 150 / 2, screenHeight / 2 + 100 / 2, 150,75 };
					selectRectangle(clickedOnTextbox, textbox, boolSelected);
					selectRectangle(clickedOnSubmit, submit, boolSelected);
					if (clickedOnTextbox) {
						key = GetKeyPressed();

						if ((key >= 32) && (key <= 125) && (p->usernameChosing.size() < maxNameLength))
						{
							p->usernameChosing.push_back(key);
						}
						key = GetKeyPressed();
						if (IsKeyPressed(KEY_BACKSPACE)) {
							if(p->usernameChosing.size()>0)
							p->usernameChosing.pop_back();
						}
					}
					ClearBackground(LIGHTGRAY);
					DrawText(usernameMessage.c_str(), screenWidth / 2 - (MeasureText(usernameMessage.c_str(), 50) / 2), screenHeight / 2 - 100, 50, BLACK);
					DrawRectangleRec(textbox, GRAY);
					if (!clickedOnTextbox) DrawRectangleLinesEx(textbox, 3, BLACK);
					else DrawRectangleLinesEx(textbox, 3, RED);
					DrawText(p->usernameChosing.c_str(), (screenWidth / 2) - (300 / 2) + (textbox.width - MeasureText(p->usernameChosing.c_str(),40))/2, (screenHeight / 2) - (40 / 2) + (textbox.height - 40) / 2, 40, WHITE);
					DrawRectangleRec(submit, GRAY);
					DrawText(submitMessage.str.c_str(), submit.x + (submit.width - MeasureText(submitMessage.str.c_str(),submitMessage.fontSize))/2, submit.y + (submit.height - submitMessage.fontSize)/2, submitMessage.fontSize, submitMessage.clr);
					if(!clickedOnSubmit) {
						DrawRectangleLinesEx(submit, 3, BLACK);
						submitMessage.str = "Submit";
						submitMessage.clr = BLACK;
					}
					else {
						submitMessage.clr = RED;
						DrawRectangleLinesEx(submit, 3, RED);
						if (p->usernameChosing.size() > 0) {
							p->username = p->usernameChosing;
							usernameChosen = true;
						}
						else {
							submitMessage.str = "Too Short";
							submitMessage.fontSize = 20;
						}
					}	
			}
			else if (!p->dif->isSelected) {
				ClearBackground(LIGHTGRAY);
				//drawCenterLines();
				DrawText("Chose your difficulty", screenWidth / 2 - (MeasureText("Input your username:", 50) / 2), screenHeight / 2 - 100, 50, BLACK);
				Rectangle inputDifRec = { screenWidth / 2 - 400, screenHeight / 2, 800, 200 };
				DrawRectangleRec(inputDifRec, BLACK);
				std::map<std::string, Rectangle> difSelect;
				difSelect["Easy"] = { inputDifRec.x,inputDifRec.y,inputDifRec.width / 3,inputDifRec.height };
				difSelect["Normal"] = { inputDifRec.x + inputDifRec.width / 3,inputDifRec.y,inputDifRec.width / 3,inputDifRec.height };
				difSelect["Hard"] = { inputDifRec.x + (inputDifRec.width / 3) * 2 ,inputDifRec.y,inputDifRec.width / 3,inputDifRec.height };

				for (auto itr = difSelect.begin(); itr != difSelect.end(); itr++) {
					DrawRectangleRec(itr->second, GRAY);
					DrawRectangleLinesEx(itr->second, 4, LIGHTGRAY);
					DrawText(itr->first.c_str(), itr->second.x + itr->second.width / 2 - (MeasureText(itr->first.c_str(), 30) / 2), itr->second.y + itr->second.height / 2 - 15, 30, BLACK);
				}

				for (auto itr = difSelect.begin(); itr != difSelect.end(); itr++)
				{
					if (CheckCollisionPointRec(GetMousePosition(), itr->second)) {
						DrawRectangleLinesEx(itr->second, 5, BLACK);
					}
				}

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					for (auto itr = difSelect.begin(); itr != difSelect.end(); itr++)
					{
						if (!p->dif->isSelected) {
							selectRectangle(p->dif->isSelected, itr->second, boolSelected);
							p->dif->difName = itr->first;
						}
					}
				}


		}
		if (p->dif->isSelected && usernameChosen){
		ClearBackground(BLACK);
		p->movement();
		DrawTexture(p->pSprite, p->playerPos.x,p->playerPos.y, WHITE);
		p->showHealth();
		DrawRectangle(p->playerPos.x, p->playerPos.y, 10, 10, RED);
		DrawRectangle(p->playerPos.x + p->pSprite.width, p->playerPos.y, 10, 10, RED);
		}
		EndDrawing();
	}
	CloseWindow();
}