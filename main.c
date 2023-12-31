#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "./constants.h"
#include "./setup.h"
#include <math.h>
#include <stdbool.h>
#include <SDL_ttf.h>


// Initializing game_is_running to false to let the while loop run only when the game is running
int game_is_running = FALSE;

// Required Initializations
int last_frame_time = 0;

// Initializing SDL window, SDL renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// x, y positions of bricks, brick type stored in an array
int bricks[Num_Rows][Num_Columns];

// From setup.h file
void setup();

// Variables to detect the side of the brick collided
int collidedLeft, collidedRight, collidedTop, collidedBottom;

//--------------------------------------------------------------------------------------------------------------------------------------------

int initialize_window(void){
    // SDL_Init(SDL_INIT_EVERYTHING) --> It initializes everything in SDL 
    // 0 --> it is somehting like okay in SDL
    // if something goes wrong like we don't have windows 
    // or dont have display set or you are running on a pure console mode it raises error. So,
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error Initializing SDL.\n");
        return FALSE;   // "FALSE" defined as '0' in constants.h         
    }

    // CREATING A WINDOW
    // SYNTAX : SDL_CreateWindow("some game title", x-coordinate, y-cpprdinate, screen width, screen height, FLAGS(like properties or decoratives etc ));
    window = SDL_CreateWindow("PONG BREAKER GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
    // if SDL coudn't create window 
    if (!window){
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }

    // CREATING A RENDERER --> it renders the objects, particles, colour buffers etc, in the game 
    // renderer = SDL_CreateRenderer( SDL_Window* window, driver code/display number, FLAGS)     
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);     // -1 --> this is to get the default driver,   0 --> no flags
    // if SDL coudn't create renderer
    if (!renderer){
        fprintf(stderr, "Error Creating SDL renderer.\n");
        return FALSE;
    }

    //Initialize bricks
    for (int i = 0; i < Num_Rows; ++i) {
        for (int j = 0; j < Num_Columns; ++j) {
            bricks[i][j] = 1;  // Set all bricks initially
        }
    }
   
    return TRUE;     // "TRUE" defined as '1' in constants.h
}
// Window destroyer function to destroy renderer and window and quit the game 
void destroy_window(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
// Function to proess every input
void process_input(){
    // naming the SDL_Events as event
    SDL_Event event;
    SDL_PollEvent (&event);  // reads all the events executed by the player, and [(this modifies the events to something which has types) -> not a sure way of explaining]
    switch (event.type){
        case SDL_QUIT:     // when we click that close button [X] on top right corner that peforms SDL_QUIT
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:  
            if (event.key.keysym.sym == SDLK_ESCAPE){       // to set game_is_running to false when ESC key is pressed
                game_is_running = FALSE;
            }
            break; 
    }

    // LOGIC TO KEEP FIXED TIME STEP OF THE GAME
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    last_frame_time = SDL_GetTicks();    
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void drawHomepage() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw start button
    SDL_Rect startButtonRect = {(WINDOW_WIDTH - startbutton.width) / 2, WINDOW_HEIGHT / 2, startbutton.width, startbutton.height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &startButtonRect);

    SDL_RenderPresent(renderer);
}
void drawPlayAgain() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw play again button
    SDL_Rect playAgainButtonRect = {(WINDOW_WIDTH - startbutton.width) / 2, WINDOW_HEIGHT / 2, startbutton.width, startbutton.height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &playAgainButtonRect);

    SDL_RenderPresent(renderer);
}
/*Functions required for render()*/
// Draw bricks
void drawBricks() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < Num_Rows; ++i) {
        for (int j = 0; j < Num_Columns; ++j) {
            if(bricks[i][j]){
                SDL_Rect brickRect = { j * (brick.width + 5), i * (brick.height + 5), brick.width, brick.height };
                SDL_RenderFillRect(renderer, &brickRect);
            }
        }
    }
}
//Draw a ball
void bigcircle(int x, int y, int radius) {
    // Draw a filled circle at the specified position with the given radius
    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            if (i * i + j * j <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x + i, y + j);
            }
        }
    }
}
void render(){
    // SDL_SetRenderDrawColor(SDL_Renderer* renderer, red, green, blue, transperancy)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /*SDL works with kind of double buffer technique 
    a front buffer which is currently shown and a back buffer is where we have a frredom of 
    going and rendering objects back buffer is hidden and when we are done rendering with 
    back buffer, we swap back and front buffer this */
    
    // Draw a paddle
    SDL_Rect paddle_rect = {(int)paddle.x, (int)paddle.y, (int)paddle.width, (int)paddle.height};
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    bigcircle(ball.x, ball.y, ball.radius);

    drawBricks();

    SDL_RenderPresent(renderer);
}

//--------------------------------------------------------------------------------------------------------------------------------------------



/*Functions required for update()*/
//1)Check collision with bricks
int checkCollisionWithBricks(int x, int y, int radius, int* collidedRow, int* collidedCol) {
    int ballLeft = x - radius;
    int ballRight = x + radius;
    int ballTop = y - radius;
    int ballBottom = y + radius;

    for (int i = 0; i < Num_Rows; ++i) {
        for (int j = 0; j < Num_Columns; ++j) {
            if (bricks[i][j]) {
                int brickLeft = j * (brick.width + 5);
                int brickRight = brickLeft + brick.width;
                int brickTop = i * (brick.height + 5);
                int brickBottom = brickTop + brick.height;

                if (!(ballRight < brickLeft || ballLeft > brickRight || ballBottom < brickTop || ballTop > brickBottom)) {
                    if (ballRight >= brickLeft){  // or if(!(ballRight < brickLeft))
                        collidedLeft = 1;
                    }
                    if (ballLeft <= brickRight){  // or if(!(ballLeft > brickRight))
                        collidedRight = 1;
                    }
                    if (ballBottom >= brickTop){  // or if(!(ballBottom < brickTop))
                        collidedTop = 1;
                    }
                    if (ballTop <= brickBottom){  // or if(!(ballTop > brickBottom))
                        collidedBottom = 1;
                    }
                    // bricks[i][j] = 0;  // Remove the brick when there's a collision it is done in handle brick collision
                    *collidedRow = i;
                    *collidedCol = j;
                    return 1;  // Collision detected
                }
            }
        }
    }
    return 0;  // No collision detected
}
//2) Check collision
int checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}
//3) Handling the brick collision
void handleBrickCollision(int collidedRow, int collidedCol) {
    // Remove the brick
    bricks[collidedRow][collidedCol] = 0;

    // Calculate the relative position of the ball to the edges of the brick
    int brickLeft = collidedCol * (brick.width + 5);
    int brickRight = brickLeft + brick.width;
    int brickTop = collidedRow * (brick.height + 5);
    int brickBottom = brickTop + brick.height;

    float relativePositionX = (float)(ball.x - (brickLeft + brickRight) / 2) / (brick.width / 2);
    float relativePositionY = (float)(ball.y - (brickTop + brickBottom) / 2) / (brick.height / 2);

    // Adjust the ball's angle based on the relative position
    if (fabs(relativePositionX) > fabs(relativePositionY)) {
        // Collision occurred on the left or right side of the brick
        ball.Angle = M_PI - ball.Angle;  // Bounce off horizontally
    } 
    else {
        // Collision occurred on the top or bottom of the brick
        ball.Angle = -ball.Angle;  // Bounce off vertically
    }
}
void update(){
    // Moving the paddle or uppdating paddle position
    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_LEFT] && paddle.x > 0) {
        paddle.x -= paddle.vel;
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT] && paddle.x < WINDOW_WIDTH - paddle.width) {
        paddle.x += paddle.vel;
    }

    // Moving the ball or updating ball position
    ball.x += ball.vel * cos(ball.Angle);
    ball.y += ball.vel * sin(ball.Angle);

    // Checking collisions with walls and making the ball bounce accordingly
    if (ball.x - ball.radius < 0 || ball.x + ball.radius > WINDOW_WIDTH) {
        ball.Angle = M_PI - ball.Angle;
    }
    if (ball.y - ball.radius < 0) {
        ball.Angle = -ball.Angle;
    }

    // Check collision with paddle
    SDL_Rect paddle_Rect = { paddle.x, paddle.y, paddle.width, paddle.height };
    SDL_Rect ball_Rect = { ball.x - ball.radius, ball.y - ball.radius, ball.width, ball.height };
    if (checkCollision(paddle_Rect, ball_Rect)) {
        ball.Angle = -ball.Angle;
    }
    
    int collidedRow, collidedCol;
    // If collisionWithBricks 
    if(checkCollisionWithBricks(ball.x, ball.y, ball.radius, &collidedRow, &collidedCol)) {
        handleBrickCollision(collidedRow, collidedCol);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void handleHomepage() {
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } 
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;  // Exit the loop to end the game
                    game_is_running = FALSE;  // Set game_is_running to FALSE to end the game loop
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                // Check if the start button is clicked
                if (mouseX >= (WINDOW_WIDTH - startbutton.width) / 2 && mouseX <= (WINDOW_WIDTH + startbutton.width) / 2 && mouseY >= WINDOW_HEIGHT / 2 && mouseY <= WINDOW_HEIGHT / 2 + startbutton.height) {
                    quit = 1;  // Exit the loop to start the game
                }
            }
        }
        drawHomepage();
    }
    // Initialize bricks for a new game
    for (int i = 0; i < Num_Rows; ++i) {
        for (int j = 0; j < Num_Columns; ++j) {
            bricks[i][j] = 1;  // Set all bricks initially
        }
    }
    if (game_is_running) {
        game_is_running = TRUE;  // Start the game only if game_is_running is still TRUE
    }
}

void handlePlayAgain() {
    SDL_Event event;
    int quit = 0;
    int playAgain = 0;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                // Check if the play again button is clicked
                if (mouseX >= (WINDOW_WIDTH - startbutton.width) / 2 && mouseX <= (WINDOW_WIDTH + startbutton.width) / 2 && mouseY >= WINDOW_HEIGHT / 2 && mouseY <= WINDOW_HEIGHT / 2 + startbutton.height) {
                    playAgain = 1;  // Set the flag to play again
                    quit = 1;       // Exit the loop to play again
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;  // Exit the loop to end the game
                }
            }
        }

        drawPlayAgain();
    }

    if (playAgain) {
        // Initialize bricks for a new game
        for (int i = 0; i < Num_Rows; ++i) {
            for (int j = 0; j < Num_Columns; ++j) {
                bricks[i][j] = 1;  // Set all bricks initially
            }
        }
        // Initializing ball position for new game
        ball.x = WINDOW_WIDTH/2;
        ball.y = WINDOW_HEIGHT/2;
        ball.Angle = -M_PI/4;
        game_is_running = TRUE;  // Start the game
    }
}

void handleGameplay() {
    SDL_Event event;
    int quit = 0;

    while (!quit && game_is_running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                game_is_running = FALSE;
                quit = 1;
            } 
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = FALSE;
                    quit = 1;
                }
            }
        }

        process_input();  // Process input events

        // Other gameplay logic...
        update();
        render();
    }

    if (!game_is_running) {
        handlePlayAgain();  // Ask if the player wants to play again
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    game_is_running = initialize_window();

    setup();

    while (game_is_running){
        handleHomepage();
        handleGameplay();
    }
    // When the while loop finishes it means the game_is_not_running anymore and destroy_window() function is called 
    destroy_window();

    return FALSE;
}


