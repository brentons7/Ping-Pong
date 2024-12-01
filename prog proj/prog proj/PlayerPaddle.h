#ifndef PLAYER_PADDLE_H
#define PLAYER_PADDLE_H

#include "Paddle.h"
#include <raylib.h>
#include <raymath.h>

class PlayerPaddle : public Paddle {
public:
	void Update();
};

#endif
