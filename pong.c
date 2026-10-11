#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MOVEMENT_SPEED 80.0f
#define RECTANGLE_LENGTH 80
#define BALL_SIZE 10

void setNewGame(bool* isReversedMovement, Vector2* ballPosition, Vector2* ballVelocity, bool* isBallBounced) {
	SetRandomSeed(time(NULL));
	*ballPosition = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	ballVelocity->x = GetRandomValue(35, 50);
	ballVelocity->y = GetRandomValue(10, 60);
	*isReversedMovement = (bool)GetRandomValue(0, 1);
	*isBallBounced = (bool)GetRandomValue(0, 1);
}

int main(void) {
	SetRandomSeed(time(NULL));

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ping pong");
	HideCursor();

	float firstPosition = SCREEN_HEIGHT/2 - RECTANGLE_LENGTH/2;
	float secondPosition = firstPosition;
	
	short firstPlayerScore = 0;
	short secondPlayerScore = 0;

	char firstPlayerLastKey = '0';
	char secondPlayerLastKey = '0';

	char scoreText[16] = "0 : 0";

	bool isBallBounced = false;
	bool isReversedMovement;
	bool pauseGame = true;

	Vector2 ballPosition = (Vector2){0,0};
	Vector2 ballVelocity = (Vector2){0,0};
	setNewGame(&isReversedMovement, &ballPosition, &ballVelocity, &isBallBounced);

	SetTargetFPS(100);
	while (!WindowShouldClose()) {

	/*** Game pausing and restarting segment ***/	
	if(IsKeyPressed(KEY_SPACE)) {
	  if (pauseGame) pauseGame = false;
	  else pauseGame = true;
	}
	if(IsKeyPressed(KEY_R)) setNewGame(&isReversedMovement, &ballPosition, &ballVelocity, &isBallBounced);
	if (!pauseGame) {

	  /*** Ball movement and velocity upgrades ***/
	  if (!isBallBounced) ballPosition.y += GetFrameTime()*6.0f*ballVelocity.y;
	  else ballPosition.y -= GetFrameTime()*6.0f*ballVelocity.y;

	  if (isReversedMovement) {
		  ballPosition.x -= GetFrameTime()*6.0f*ballVelocity.x;
	  } else {
		  ballPosition.x += GetFrameTime()*6.0f*ballVelocity.x;
	  }
	  
	  if (ballPosition.y > SCREEN_HEIGHT - BALL_SIZE) isBallBounced = true;
	  if (ballPosition.y < 0 + BALL_SIZE) isBallBounced = false;
	  
	  /*** Score system and ball manipulation ***/
	  if (ballPosition.x > SCREEN_WIDTH) {
		  firstPlayerScore++;
		  setNewGame(&isReversedMovement, &ballPosition, &ballVelocity, &isBallBounced);
		  snprintf(scoreText, sizeof(scoreText), "%d : %d", firstPlayerScore, secondPlayerScore);
	  }
	  if (ballPosition.x < 0) {
		  secondPlayerScore++;
		  setNewGame(&isReversedMovement, &ballPosition, &ballVelocity, &isBallBounced);
		  snprintf(scoreText, sizeof(scoreText), "%d : %d", firstPlayerScore, secondPlayerScore);
	  }
	  
	  /*** Collision checking segment ***/
	  struct Rectangle rec = (Rectangle){20, firstPosition, 10, RECTANGLE_LENGTH};	

			// firstPlayer collision check
	  bool collisionCheck = CheckCollisionCircleRec(ballPosition, BALL_SIZE, rec);
	  if (collisionCheck) {
		  isReversedMovement = false;
		  ballVelocity.x += GetRandomValue(1, 3);
		  ballVelocity.y += GetRandomValue(1, 3);
		  if (firstPlayerLastKey == 'W') {
			  ballVelocity.y += GetRandomValue(-1, -3);
		  } else {
			  ballVelocity.y += GetRandomValue(1, 3);
		  }
	  }

			// secondPlayer collision check
	  rec = (Rectangle){SCREEN_WIDTH - 20, secondPosition, 10, RECTANGLE_LENGTH};
	  collisionCheck = CheckCollisionCircleRec(ballPosition, BALL_SIZE, rec);
	  if (collisionCheck) {
		  isReversedMovement = true;
		  ballVelocity.x += GetRandomValue(1, 3);
		  if (secondPlayerLastKey == 'W') {
			  ballVelocity.y += GetRandomValue(-1, -3);
		  } else {
			  ballVelocity.y += GetRandomValue(1, 3);
		  }
	  }

	  /*** Key reading segment ***/
	  if(IsKeyDown(KEY_W)) {
		  firstPosition -= GetFrameTime() * 6.0f * MOVEMENT_SPEED;
		  firstPlayerLastKey = 'W';
		  if (firstPosition < 0) firstPosition = 0;
	  }
	  if(IsKeyDown(KEY_S)) {
		  firstPosition += GetFrameTime() * 6.0f * MOVEMENT_SPEED;
		  firstPlayerLastKey = 'S';
		  if (firstPosition > SCREEN_HEIGHT - RECTANGLE_LENGTH) firstPosition = (float)(SCREEN_HEIGHT - RECTANGLE_LENGTH);
	  }
	  if(IsKeyDown(KEY_UP)) {
		  secondPosition -= GetFrameTime() * 6.0f * MOVEMENT_SPEED;
		  secondPlayerLastKey = 'W';
		  if (secondPosition < 0) secondPosition = 0;
	  }
	  if(IsKeyDown(KEY_DOWN)) {
		  secondPosition += GetFrameTime() * 6.0f * MOVEMENT_SPEED;
		  secondPlayerLastKey = 'S';
		  if (secondPosition > SCREEN_HEIGHT - RECTANGLE_LENGTH) secondPosition = (float)(SCREEN_HEIGHT - RECTANGLE_LENGTH);
	  }
	}

		BeginDrawing();
			ClearBackground(DARKGREEN);
			DrawRectangleV((Vector2){20, firstPosition}, (Vector2){10, RECTANGLE_LENGTH}, BLACK);
			DrawRectangleV((Vector2){780, secondPosition}, (Vector2){10, RECTANGLE_LENGTH}, BLACK);
			DrawCircleV(ballPosition, BALL_SIZE, RAYWHITE);
			DrawText(scoreText, SCREEN_WIDTH/2 - 32, 20, 32, BLACK);
			if (pauseGame) {
				DrawText("GAME PAUSED", SCREEN_WIDTH/2 - 228, SCREEN_HEIGHT/2 - 32, 64, RAYWHITE);
				DrawText("press [SPACE] to unpause", SCREEN_WIDTH/2 - 208, SCREEN_HEIGHT/2 + 64, 32, RAYWHITE);		
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
