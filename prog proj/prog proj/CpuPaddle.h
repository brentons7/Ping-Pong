#ifndef CPU_PADDLE_H
#define CPU_PADDLE_H

#include "Paddle.h"
#include <raylib.h>
#include <raymath.h>

class CpuPaddle : public Paddle {
public:
    void Update(int ball_y);
};

#endif