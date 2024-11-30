#include <iostream>
#include <raylib.h>
#include <raymath.h>
using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
    Sound cpuSound;
    Sound scoreSound;                                  //Sound variables
    Texture2D texture_ball;
    float x, y;
    float rotation = 0.0;
    int speed_x, speed_y;
    int radius;
   
    ~Ball (){                                           //Destructor to delete the sounds/texture
        UnloadSound(scoreSound);
        UnloadSound(cpuSound);
        UnloadTexture(texture_ball);
    }
    void Draw() {                                      //rotation texture (Yarn ball)
        Rectangle sourceRec = { -radius,-radius,texture_ball.width,texture_ball.height };
        Rectangle ballRect = { x - radius, y - radius, radius * 2, radius * 2 };
        DrawTexturePro(texture_ball, sourceRec, ballRect, Vector2{(float)radius,(float)radius}, rotation, WHITE); 
    }

    void Update() {
        x += speed_x;
        y += speed_y;
        rotation += speed_x;

        if (y >= GetScreenHeight() || y - radius * 2 <= 0) {
            speed_y *= -1;
        }
        // Cpu wins
        if (x >= GetScreenWidth()) {
            cpu_score++;
            PlaySound(cpuSound);                 //Adding sound
            ResetBall();
        }

        if (x - radius * 2 <= 0) {
            player_score++;
            PlaySound(scoreSound);                  //Adding sound (Works)
            ResetBall();
        }
    }
     
    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = { -1, 1 };
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {                                          //inheritance
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;
    Texture2D texture_paddle;

    ~Paddle() {
        UnloadTexture(texture_paddle);
    }

    void Draw() {
        //DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, BLACK);  //Paddle color
        Rectangle sourceRec = { 0,0, texture_paddle.width,texture_paddle.height };
        Rectangle ballRect = { x, y, width, height };
        DrawTexturePro(texture_paddle, sourceRec, ballRect, Vector2{ 0,0 }, 0, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle {                           //inheritance
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y) {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

bool Paused = false;


int main() {
    cout << "Starting the game" << endl;
    const int screenWidth = 1280; 
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "My Pong Game!");
    InitAudioDevice();
    SetTargetFPS(60);
    ball.radius = 40;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speed_x = 8;                                                  //Speed of ball
    ball.speed_y = 8;
    ball.scoreSound = LoadSound("resources/yippee.mp3");              //Adding sounds for winning
    ball.cpuSound = LoadSound("resources/aww.mp3");                             //Adding sounds for losing
    ball.texture_ball = LoadTexture("resources/yarn.png");                      //Adding the texture for ball

    player.width = 45;
    player.height = 150;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight / 2 - player.height / 2;
    player.speed = 6;
    player.texture_paddle = LoadTexture("resources/cat_paw_pixel.png");

    cpu.height = 150;
    cpu.width = 45;
    cpu.x = 10;
    cpu.y = screenHeight / 2 - cpu.height / 2;
    cpu.speed = 6;
    cpu.texture_paddle = LoadTexture("resources/cat_paw_pixel.png");


    Texture2D texture = LoadTexture("resources/PinkBackground.png");            //Adding image in, only png images MYCODE
    Sound sound = LoadSound("resources/meow.ogg");                             //Adding sound to ball
      
    float scaleX = (float)screenWidth / texture.width;    //Depending on Image MYCODE
    float scaleY = (float)screenHeight / texture.height;  //MYCODE

    float scale = (scaleX < scaleY) ? scaleX : scaleY;     // MYCODE

    while (WindowShouldClose() == false) {
        BeginDrawing();

        if (IsKeyPressed(KEY_SPACE)) {                     //To pause the game
            Paused = !Paused;
        }
        if (!Paused) {                                     //Not pasuing the game
            // Updating
            ball.Update();
            player.Update();
            cpu.Update(ball.y);
                                                            // Checking for collisions (rotational as well)
            if (CheckCollisionCircleRec({ ball.x - ball.radius, ball.y - ball.radius }, ball.radius, { player.x, player.y, player.width, player.height })) {
                ball.speed_x *= -1;
                PlaySound(sound);                        // Playing sound
            }

            if (CheckCollisionCircleRec({ ball.x - ball.radius, ball.y - ball.radius }, ball.radius, { cpu.x, cpu.y, cpu.width, cpu.height })) {
                ball.speed_x *= -1;
                PlaySound(sound);                        // Playing Sound
            }
        }
        ClearBackground(WHITE);                         // Drawing
        DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

        Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height }; //Size of the image MYCODE
        // Define destination rectangle (screen area where to draw the texture)
        Rectangle destRec = {                             // MYCODE
            (screenWidth - texture.width * scale) / 2,   // X position (centered) MYCODE
            (screenHeight - texture.height * scale) / 2, // Y position (centered) MYCODE
            texture.width * scale,                       // Scaled width MYCODE
            texture.height * scale                       // Scaled height MYCODE
        };
        Vector2 origin = { 0,0 };                        // Where origin begins, starting the destination at this point MYCODE

        DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE); //MYCODE
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), screenWidth / 4 - 20, 20, 80, BLACK);
        DrawText(TextFormat("%i", player_score), 3 * screenWidth / 4 - 20, 20, 80, BLACK);
        if (Paused) {
            DrawText("Paused", 490, 350, 80, BLACK);
        }
        EndDrawing();
    }
    UnloadSound(sound);                                 // Playing sound
    UnloadTexture(texture);
    UnloadTexture(ball.texture_ball);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
