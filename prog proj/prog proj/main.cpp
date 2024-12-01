#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "Ball.h"
#include "Paddle.h"
#include "PlayerPaddle.h"
#include "CpuPaddle.h"
#include "game.h"

int player_score = 0;
int cpu_score = 0;

Ball ball;
PlayerPaddle player;
CpuPaddle cpu;
Music music;

bool Paused = false;

void LoadResources() {
	ball.ballTexture = LoadTexture("assets/yarnBall.png");
	ball.bump = LoadSound("assets/bump.mp3");
	ball.collision = LoadSound("assets/pop1.mp3");
	ball.score = LoadSound("assets/score.mp3");
	ball.playerMeow = LoadSound("assets/meowwin.mp3");
	ball.cpuMeow = LoadSound("assets/meowlose.mp3");

	music = LoadMusicStream("assets/music.mp3");
	PlayMusicStream(music);
	SetMusicVolume(music, 0.5f);

	player.paddleTexture = LoadTexture("assets/playerCatPaddle.png");
	cpu.paddleTexture = LoadTexture("assets/cpuCatPaddle.png");


	ball.background = LoadTexture("assets/PinkBackground.png");
	player.avatar = LoadTexture("assets/playerCat.png");
	cpu.avatar = LoadTexture("assets/cpuCat.png");
}

void UnloadResources() {
	UnloadTexture(ball.ballTexture);
	UnloadSound(ball.bump);
	UnloadSound(ball.collision);
	UnloadSound(ball.score);
	UnloadSound(ball.playerMeow);
	UnloadSound(ball.cpuMeow);

	UnloadMusicStream(music);
	CloseAudioDevice();

	UnloadTexture(player.paddleTexture);
	UnloadTexture(cpu.paddleTexture);

	UnloadTexture(ball.background);
	UnloadTexture(player.avatar);
	UnloadTexture(cpu.avatar);
}

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 800;
	InitWindow(screenWidth, screenHeight, "My Pong Game!");
	InitAudioDevice();
	SetTargetFPS(60);

	LoadResources();

	player.width = 30;
	player.height = 120;
	player.x = screenWidth - player.width - 10;
	player.y = screenHeight / 2 - player.height / 2;
	player.speed = 6;

	cpu.width = 30;
	cpu.height = 120;
	cpu.x = 10;
	cpu.y = screenHeight / 2 - cpu.height / 2;
	cpu.speed = 6;

	ball.radius = 32;
	ball.x = screenWidth / 2;
	ball.y = screenHeight / 2;
	ball.speed_x = 7;
	ball.speed_y = 7;

	float scaleX = (float)screenWidth / ball.background.width;
	float scaleY = (float)screenHeight / ball.background.height;

	float scale = (scaleX < scaleY) ? scaleX : scaleY;

	while (WindowShouldClose() == false) {
		BeginDrawing();
		UpdateMusicStream(music);

		if (GetMusicTimePlayed(music) >= GetMusicTimeLength(music)) {
			PlayMusicStream(music);
		}

		if (IsKeyPressed(KEY_SPACE)) {
			Paused = !Paused;
		}

		if (!Paused) {

			ball.Update();
			player.Update();
			cpu.Update(ball.y);

			if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player.x, player.y, player.width, player.height })) {
				ball.speed_x *= -1;
				PlaySound(ball.collision);
			}

			if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { cpu.x, cpu.y, cpu.width, cpu.height })) {
				ball.speed_x *= -1;
				PlaySound(ball.collision);
			}
		}

		ClearBackground(WHITE);

		DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

		Rectangle sourceRec = { 0.0f, 0.0f, (float)ball.background.width, (float)ball.background.height };

		Rectangle destRec = {
			(screenWidth - ball.background.width * scale) / 2,
			(screenHeight - ball.background.height * scale) / 2,
			ball.background.width * scale,
			ball.background.height * scale
		};
		Vector2 origin = { 0,0 };

		DrawTexturePro(ball.background, sourceRec, destRec, origin, 0.0f, WHITE);
		cpu.Draw();
		player.Draw();
		ball.Draw();

		DrawText(TextFormat("%i", cpu_score), screenWidth / 4 - 20, 20, 80, BLACK);
		DrawTextureEx(cpu.avatar, Vector2{ screenWidth / 4.5 - 130, 20 }, 0.0f, 0.3f, WHITE);
		DrawText(TextFormat("%i", player_score), 3 * screenWidth / 4 - 20, 20, 80, BLACK);
		DrawTextureEx(player.avatar, Vector2{ screenWidth / 4 + 680, 20 }, 0.0f, 0.3f, WHITE);

		if (Paused) {
			DrawText("Paused", 490, 350, 80, BLACK);
		}

		EndDrawing();
	}

	UnloadResources();
	CloseWindow();
	return 0;
}



