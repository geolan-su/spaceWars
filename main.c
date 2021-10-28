/*
main file that will be calling the draw functions and initializing
the rendering of everything cool in the program
*/
//gcc main.c -o main -lSDL2 -lSDL2_image 
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

//makes the player ship
typedef struct {
    int x, y;
    double hp;
    int score;
    char* name;
    int speedBoost, damageBoost;
    double velocity;
} player;

//makes diff types of enemies
typedef struct {
    int x, y;
    int type; //would determine weapon speed/hp
    double hp;
    int lastRemaining;
    double velocity;
} enemy;

typedef struct {

} rock;

typedef struct {

} lasers;

//#include <obama.h>
void initialize(SDL_Window* window, SDL_Renderer* renderer, 
SDL_Surface* surface, SDL_Texture* texture, SDL_Rect rect,
enemy wave[][10], int speed);

void draw();

void ioControl(SDL_Window* window, SDL_Renderer* renderer, 
SDL_Surface* surface, SDL_Texture* texture, SDL_Rect rect,
enemy wave[][10], int speed);

void destroyer(SDL_Window* window, SDL_Renderer*
renderer, SDL_Texture* texture);

int main(void) { //int argc, char* argv[] were parameters
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect rect;
    enemy wave[5][10];
    int speed;

    initialize(window, renderer, surface, texture, rect, wave, speed);

    ioControl(window, renderer, surface, texture, rect, wave, speed);



    return 0;
}

//starts the renderer and drawing things, prob passes back through
//reference
//each wave slightly lower and slightly faster
void initialize(SDL_Window* window, SDL_Renderer* renderer, 
SDL_Surface* surface, SDL_Texture* texture, SDL_Rect rect,
enemy wave[][10], int speed) {
    

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow("Cosmic Marauders",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    1024, 768, 0);

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    surface = IMG_Load("${workspaceFolder}/assets/Untitled.png");

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

    rect.w /= 5;
    rect.h /= 5;

    rect.x = (1024 - rect.w) / 2;
    rect.y = (768 - rect.h) / 2;

    speed = 300;
    }

void draw() {

}

void ioControl(SDL_Window* window, SDL_Renderer* renderer, 
SDL_Surface* surface, SDL_Texture* texture, SDL_Rect rect,
enemy wave[][10], int speed) {
    int stop = 0;

    while(!stop) {

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                stop = 1;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    rect.x -= speed / 30;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    rect.x += speed / 30;
                    break;
                default:
                    break;
                }   
            }
        }
        //right bounds
        if(rect.x + rect.w > 1024) {
            rect.x = 1024 - rect.w;
        }
        //left bounds
        if(rect.x < 0) {
            rect.x = 0;
        }
        // bottom boundary
        if (rect.y + rect.h > 768) {
            rect.y = 768 - rect.h;
        }
        // upper boundary
        if (rect.y < 0) {
            rect.y = 0;
        }
        //screen clear
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);

        //multiple render or something
        SDL_RenderPresent(renderer);

        //60fps gamer
        SDL_Delay(1000 / 61);
    }
}

void destroyer(SDL_Window* window, SDL_Renderer*
renderer, SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


