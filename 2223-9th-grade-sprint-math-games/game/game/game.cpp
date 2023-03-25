
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
struct question {
	std::string qst;
	std::string answers[3];
	int correctAnswer;
};


texture textureMake(Image* img, int width, int height) {
	ImageResize(img, width, height);
	return LoadTextureFromImage(*img);
}

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

	int floorHeight = screenHeight - 130;

	struct difficulty {
		std::string difName;
		bool isSelected = false;
	};

	Image enemyImg = LoadImage("images/enemy.png");
	texture enemy;
	pos enemyPos;
	int enemyHealth = 3;

	std::vector<question> questions{
	{ "1010 & 1101?",{"1001","1111","1000"},3},
	{ "0101 & 1101?",{"0101","1010","1101"},1},
	{ "0011 ^ 0101?",{"0100","0010","0110"},2},
	{ "1101 | 1011?",{"1001","1000","1110"},1}
	};

public:
	game() {
		ImageFlipHorizontal(&enemyImg);
		enemy = textureMake(&enemyImg, enemyImg.width * 2, enemyImg.height * 2);
		enemyPos = { (float)screenWidth - 50 - enemy.width, (float)floorHeight - enemy.height };
	}
};

// --------------------------- PLAYER CLASS ------------------------------

class player : public game {
public:

	std::vector<bool*> boolSelected;
	Image pSpriteImg = LoadImage("images/sprite.png");
	texture pSprite;
	pos playerPos;
	int lives = NULL;
	float health;
	float starting_health;
	std::string username;
	std::string usernameChosing;
	bool attacking = false;
	difficulty* dif = new difficulty();
	question q = questions[GetRandomValue(0, questions.size() - 1)];
	bool answerClicked = false;
	bool rightAnswer;
	bool takeDamage = true;


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
		std::string healthStr = std::to_string((int)health) + "|" + std::to_string((int)starting_health);
		DrawText(healthStr.c_str(), 320, 20 + (30 / 2) - 50 / 2, 50, RED);
	}

	void movement() {
		moveRight(playerPos, pSprite.width);
		moveLeft(playerPos, pSprite.width);
		gravity(playerPos, pSprite.height);
	}
	int getFloorHeight() {
		return floorHeight;
	}
	texture* getEnemy() {
		return &enemy;
	}

	pos* getEnemyPos() {
		return &enemyPos;
	}

	const std::vector<question>& getQuestions() {
		return questions;
	}

	void showEnemyHealth() {

		Rectangle rec1 = { enemyPos.x + (enemy.width - 200) / 2,enemyPos.y - 30,100,20 };
		Rectangle rec2 = { enemyPos.x + (enemy.width - 200) / 2, enemyPos.y - 30, 100 * ((float)enemyHealth / 3), 20 };
		DrawRectangleRec(rec1, BLACK);
		DrawRectangleRec(rec2, RED);
		DrawRectangleLinesEx(rec1, 2, BLACK);
	}

	void attack() {
		if (!attacking) {
			if (playerPos.x > enemyPos.x - 100 - pSprite.width) {
				DrawText("Press E to attack", playerPos.x + (pSprite.width - MeasureText("Press E to attack", 30)) / 2, playerPos.y - 50, 30, BLACK);
				if (IsKeyDown(KEY_E)) {
					attacking = true;
				}
			}
		}
		else {
			Rectangle questionRec = { screenWidth / 2 - (MeasureText(q.qst.c_str(),60)) / 1.5, screenHeight / 2 - 200, (MeasureText(q.qst.c_str(),60) + 120), 400 };
			DrawRectangle(0, 0, screenWidth, screenHeight, { 0,0,0,100 });
			DrawRectangleRec(questionRec, LIGHTGRAY);
			DrawRectangleLinesEx(questionRec, 3, BLACK);
			DrawText(q.qst.c_str(), questionRec.x + 60, questionRec.y + 60, 60, BLACK);
			Rectangle answerRec[3];
			for (int i = 0; i < 3; i++) {
				answerRec[i] = { questionRec.x + (questionRec.width - MeasureText(q.qst.c_str(),60)) / 2 , questionRec.y + 130 + (i * 80) ,(float)MeasureText(q.qst.c_str(),60), 60 };
				DrawRectangleRec(answerRec[i], GRAY);
				DrawRectangleLinesEx(answerRec[i], 3, BLACK);
				DrawText(q.answers[i].c_str(), answerRec[i].x + (answerRec[i].width - MeasureText(q.answers[i].c_str(), 60)) / 2, answerRec[i].y, 60, BLACK);
				if (i == q.correctAnswer - 1) DrawRectangle(answerRec[i].x, answerRec[i].y, 10, 10, GREEN);
			}
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				selectRectangle(answerClicked, answerRec[q.correctAnswer - 1], boolSelected);
				if (answerClicked) {
					enemyHealth -= 1;
				}
				else {
					health -= 25;

				}
				attacking = false;
				answerClicked = false;
				q = questions[GetRandomValue(0, questions.size() - 1)];
			}
		}

	}
};



// ----------------------------------- Main ----------------------------------

int main()
{
	SetTargetFPS(60);
	InitWindow(screenWidth, screenHeight, "Game");
	Image background_1Img = LoadImage("images/background.png");
	texture background_1 = textureMake(&background_1Img, screenWidth, screenHeight);
	bool setDifVal = true;
	bool difSelected = false;
	const int maxNameLength = 10;
	int key = 0;
	bool usernameChosen = false;
	bool wait = false;
	bool clickedOnTextbox = false;
	bool clickedOnSubmit = false;
	text submitMessage = { "Submit", BLACK, 30 };
	std::string usernameMessage = "Input your username:";
	player* p = new player();
	while (!WindowShouldClose()) {
		BeginDrawing();
		static float time = GetFrameTime();
		if (!usernameChosen) {
			Rectangle textbox = { screenWidth / 2 - 300 / 2, screenHeight / 2 - 50 / 2, 300,50 };
			Rectangle submit = { screenWidth / 2 - 150 / 2, screenHeight / 2 + 100 / 2, 150,75 };
			selectRectangle(clickedOnTextbox, textbox, p->boolSelected);
			selectRectangle(clickedOnSubmit, submit, p->boolSelected);
			if (clickedOnTextbox) {
				key = GetKeyPressed();

				if ((key >= 32) && (key <= 125) && (p->usernameChosing.size() < maxNameLength))
				{
					p->usernameChosing.push_back(key);
				}
				key = GetKeyPressed();
				if (IsKeyPressed(KEY_BACKSPACE)) {
					if (p->usernameChosing.size() > 0)
						p->usernameChosing.pop_back();
				}
			}
			ClearBackground(LIGHTGRAY);
			DrawText(usernameMessage.c_str(), screenWidth / 2 - (MeasureText(usernameMessage.c_str(), 50) / 2), screenHeight / 2 - 100, 50, BLACK);
			DrawRectangleRec(textbox, GRAY);
			if (!clickedOnTextbox) DrawRectangleLinesEx(textbox, 3, BLACK);
			else DrawRectangleLinesEx(textbox, 3, RED);
			DrawText(p->usernameChosing.c_str(), (screenWidth / 2) - (300 / 2) + (textbox.width - MeasureText(p->usernameChosing.c_str(), 40)) / 2, (screenHeight / 2) - (40 / 2) + (textbox.height - 40) / 2, 40, WHITE);
			DrawRectangleRec(submit, GRAY);
			DrawText(submitMessage.str.c_str(), submit.x + (submit.width - MeasureText(submitMessage.str.c_str(), submitMessage.fontSize)) / 2, submit.y + (submit.height - submitMessage.fontSize) / 2, submitMessage.fontSize, submitMessage.clr);
			if (!clickedOnSubmit) {
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
						if (CheckCollisionPointRec(GetMousePosition(), itr->second)) p->dif->isSelected = true;
						p->dif->difName = itr->first;
					}
				}
			}

			if (setDifVal && p->dif->isSelected) {
				if (p->dif->difName == "Easy") {
					p->starting_health = 150;
				}
				if (p->dif->difName == "Normal") {
					p->starting_health = 100;
				}
				if (p->dif->difName == "Hard") {
					p->starting_health = 50;
				}
				p->health = p->starting_health;
				setDifVal = false;
			}

		}
		if (p->dif->isSelected && usernameChosen) {

			DrawTexture(background_1, 0, 0, WHITE);
			//DrawRectangle(0, p->getFloorHeight(), screenWidth, screenWidth - p->getFloorHeight(), GRAY);
			p->movement();
			DrawTexture(*p->getEnemy(), p->getEnemyPos()->x, p->getEnemyPos()->y, WHITE);
			DrawTexture(p->pSprite, p->playerPos.x, p->playerPos.y, WHITE);
			p->attack();
			p->showHealth();
			p->showEnemyHealth();
			/*DrawRectangle(p->playerPos.x, p->playerPos.y, 10, 10, RED);
			DrawRectangle(p->playerPos.x + p->pSprite.width, p->playerPos.y, 10, 10, RED);*/
		}
		//for (int i = 0; i < screenHeight; i += 20) {
		//	DrawRectangle(0, i, 10, 10, RED);
		//}
		EndDrawing();
	}
	delete p;
	CloseWindow();
}