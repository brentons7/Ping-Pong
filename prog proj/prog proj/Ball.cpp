#include "Ball.h"
#include "game.h"

Ball::~Ball() {
	UnloadSound(bump);
	UnloadSound(collision);
	UnloadSound(score);
	UnloadSound(cpuMeow);
	UnloadSound(playerMeow);
	UnloadTexture(ballTexture);
    UnloadTexture(background);
}

void Ball::Draw() {
	Rectangle sourceRec = { 0.0f, 0.0f, (float)ballTexture.width, (float)ballTexture.height };
	Rectangle ballRect = { x - radius, y - radius, radius * 2, radius * 2 };
	DrawTexturePro(ballTexture, sourceRec, ballRect, Vector2{ (float)radius, (float)radius }, rotationAngle, WHITE);
}

void Ball::Update() {
    x += speed_x;
    y += speed_y;


    rotationAngle += 2.0f;

    if (y + radius >= GetScreenHeight() || y - radius <= 0) {
        speed_y *= -1;
        PlaySound(bump);
    }

    if (x + radius >= GetScreenWidth()) {
        cpu_score++;
        PlaySound(score);
        PlaySound(cpuMeow);
        ResetBall();
    }

    if (x - radius <= 0) {
        player_score++;
        PlaySound(score);
        PlaySound(playerMeow);
        ResetBall();
    }
}

void Ball::ResetBall() {
    x = GetScreenWidth() / 2;
    y = GetScreenHeight() / 2;

    int speed_choices[2] = { -1, 1 };
    speed_x *= speed_choices[GetRandomValue(0, 1)];
    speed_y *= speed_choices[GetRandomValue(0, 1)];
}