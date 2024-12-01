#ifndef PADDLE_H
#define PADDLE_H

#include <raylib.h>
#include <raymath.h>

class Paddle {
protected:
	void LimitMovement();

public:
	float x, y, width, height;
	float textureOffsetX = 20;
	float textureOffsetY = 20;
	int speed;
	Texture2D paddleTexture;
	Texture2D avatar;

	void Draw();

};

#endif