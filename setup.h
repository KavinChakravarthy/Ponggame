#include "./constants.h"

// Structures

struct paddle {
    float x;
    float y;
    float width;
    float height;
    float vel;
} paddle;

struct ball {
    float x;
    float y;
    float radius;
    float vel;
    float height;
    float width;
    float Angle;
} ball;

struct brick {
    float width;
    float height;
} brick;

struct startbutton {
    float x;
    float y;
    float width;
    float height;
} startbutton;

// Setup 1.Paddle 2.ball 3.brick 4.startbutton
void setup(){
    //1) Paddle
    paddle.width = 100;
    paddle.height = 20;
    paddle.y = WINDOW_HEIGHT - paddle.height;
    paddle.x = WINDOW_WIDTH/2-paddle.width/2;
    paddle.vel = 5;

    //2) Ball
    ball.x = WINDOW_WIDTH/2; 
    ball.y = WINDOW_HEIGHT/2;
    ball.radius = 10;
    ball.vel = 6;
    ball.height = 2*ball.radius;
    ball.width = 2*ball.radius;
    ball.Angle = -M_PI / 4;

    //3) Brick
    brick.width = 60;
    brick.height = 20;

    //4) Startbutton
    startbutton.width = 100;
    startbutton.height = 50;
    startbutton.x = WINDOW_WIDTH/2 -  startbutton.width/2;
    startbutton.x = WINDOW_HEIGHT/2 -  startbutton.height/2;

    
}
