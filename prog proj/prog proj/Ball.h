#ifndef BALL_H
#define BALL_H

#include <raylib.h>
#include <raymath.h>

class Ball {
public:
    float x, y, rotationAngle;
    int speed_x, speed_y, radius;
    Texture2D ballTexture;
    Sound bump, collision, score, cpuMeow, playerMeow;

    Texture2D background;

    ~Ball();
    void Draw();
    void Update();
    void ResetBall();
};

#endif
