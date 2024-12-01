#include "Paddle.h"
#include <raylib.h>

void Paddle::LimitMovement() {
    if (y <= 0) {
        y = 0;
    }
    if (y + height >= GetScreenHeight()) {
        y = GetScreenHeight() - height;
    }
}

void Paddle::Draw() {
    float scaleX = (width * 4.0f) / paddleTexture.width;
    float scaleY = (height * 4.0f) / paddleTexture.height;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    float textureX = x - (paddleTexture.width * scale - width) / 2 + textureOffsetX;
    float textureY = y - (paddleTexture.height * scale - height) / 2 + textureOffsetY;

    textureX = Clamp(textureX, 0.0f, (float)(GetScreenWidth() - paddleTexture.width * scale));
    textureY = Clamp(textureY, 0.0f, (float)(GetScreenHeight() - paddleTexture.height * scale));

    DrawTextureEx(paddleTexture, Vector2{ textureX, textureY }, 0.0f, scale, WHITE);
}