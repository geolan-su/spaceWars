#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <stdlib.h>

//const SHIP_HEIGHT =;
//const SHIP_WIDTH =;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768; 
const int PLAYER_HP = 100;
const int ALIEN_ONE_HP = 10;
const int ALIEN_TWO_HP = 20;
const int ALIEN_THREE_HP = 30;
const int PLAYER_W;
const int PLAYER_H;

int waveDirection = 0;
int GAMEOVER = 0;
int godmode = 0;
int frame = 0;
int delay = 0;
int enemyDelay = 0;
int cheats = 1;

//makes the player ship
typedef struct {
    int x, y;
    double hp;
    int score;
    char* name;
    int speedBoost, damageBoost;
    double velocity;
    SDL_Rect rect;
    int rectSpeed;
} player;

//makes diff types of enemies
typedef struct {
    int x, y;
    int type; //would determine weapon speed/hp
    double hp;
    int lastRemaining;
    double velocity;
    SDL_Rect rect;
    int dead;
} enemy;

typedef struct {
    enemy enemies[5][10];
    int cleared; 
} wave;

typedef struct {
    //make a grid of rocks
    int x, y;
    int destructionGrid[5][5];
    SDL_Rect rect;
} rock;

typedef struct {
    int x, y;
    double velocity;
    int damage;
    SDL_Rect rect;
    int refill;
} laser;

void initializePlayer(player* x);
void reinitializeWave(wave* w);
void initializeEnemy(enemy* e, int type);
int detectColision(SDL_Rect a, SDL_Rect b);
void playerLaser(player* steve, laser playerLasers[], size_t pLSize ,SDL_Texture* laserTexture);
void enemyLaser(enemy* bad, laser enemyLasers[500], size_t eLSize, SDL_Texture* laserTexture);
void manageBounds(SDL_Rect* r);
void destroyEnemy(enemy* x);
void shiftWaveDown(wave* w);
void killWave(wave* w);
void reinitializeRocks(rock rocks[][8][8], size_t rSize);

int main(int argc, char *argv[])
{
 
    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("Cosmic Marauders", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);
 
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, render_flags);
 
    // creates a surface
    SDL_Surface* surface;
 
    //${workspaceFolder}/assets/Untitled
    surface = IMG_Load("OBAMA.png");
 
    // loads image
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
 
    // clears main-memory
    SDL_FreeSurface(surface);
 
    SDL_Rect rect;
    int speed = 700;

    player steve;
    steve.rectSpeed = speed * steve.velocity;
    //put stuff here in initialize function or initializePlayer function
    //intitializePlayer(&steve);

    SDL_QueryTexture(texture, NULL, NULL, &steve.rect.w, &steve.rect.h);
 
    steve.rect.w /= 10;
    steve.rect.h /= 10;
    
    initializePlayer(&steve);
    // sets initial x-position
    //steve.rect.x = (1024 - steve.rect.w) / 2;
 
    // sets initial y-position
    //steve.rect.y = (768 - steve.rect.h) * 19 / 20;
 
    // controls annimation loop
    int stop = 0;
 
    // speed of box

    steve.rectSpeed = speed * steve.velocity;
    //enemy wave[5][10];
    wave w;
    w.cleared = 0;

    //initializes bullets
    //laser* playerLasers = malloc(100 * sizeof(laser));
    laser playerLasers[100];
    laser enemyLasers[500];
    rock rocks[3][8][8];

    SDL_Surface* A1surface = IMG_Load("reagan.png");
    SDL_Surface* A2surface = IMG_Load("immigrant.png");
    SDL_Surface* A3surface = IMG_Load("immigrantKid.png");
    SDL_Surface* laserSurface = IMG_Load("laser.png");
    SDL_Surface* rockSurface = IMG_Load("rock.png");

    SDL_Texture* A1texture = SDL_CreateTextureFromSurface(renderer, A1surface);
    SDL_Texture* A2texture = SDL_CreateTextureFromSurface(renderer, A2surface);
    SDL_Texture* A3texture = SDL_CreateTextureFromSurface(renderer, A3surface);
    SDL_Texture* laserTexture = SDL_CreateTextureFromSurface(renderer, laserSurface);
    SDL_Texture* rockTexture = SDL_CreateTextureFromSurface(renderer, rockSurface);


    SDL_FreeSurface(A1surface);
    SDL_FreeSurface(A2surface);
    SDL_FreeSurface(A3surface);
    SDL_FreeSurface(laserSurface);
    SDL_FreeSurface(rockSurface);

    int yPos = 10;
    int xPos = 50;
    for(int r = 0; r < sizeof(w.enemies)/sizeof(w.enemies[0]); r++) {
        for(int c = 0; c < sizeof(w.enemies[0])/sizeof(w.enemies[0][0]); c++) {
            //sets the enemy type based on row
            switch(r) {
                case 0:
                case 2:
                    initializeEnemy(&w.enemies[r][c], 0);
                    SDL_QueryTexture(A1texture, NULL, NULL, &w.enemies[r][c].rect.w, &w.enemies[r][c].rect.h);
                    //printf("%d", w.enemies[r][c].type);
                    break;
                case 1:
                case 4:
                    initializeEnemy(&w.enemies[r][c], 1);
                    SDL_QueryTexture(A2texture, NULL, NULL, &w.enemies[r][c].rect.w, &w.enemies[r][c].rect.h);
                    //printf("%d", w.enemies[r][c].type);
                    break;
                case 3:
                    initializeEnemy(&w.enemies[r][c], 2);
                    SDL_QueryTexture(A3texture, NULL, NULL, &w.enemies[r][c].rect.w, &w.enemies[r][c].rect.h);
                    //printf("%d", w.enemies[r][c].type);
                    break;
                default: 
                    break;
                } 
                w.enemies[r][c].rect.w /= 24;
                w.enemies[r][c].rect.h /= 24;
    
                // sets initial x-position
                w.enemies[r][c].rect.x = (1024 - w.enemies[r][c].rect.w) * 1 / 20 + xPos;
 
                // sets initial y-position
                w.enemies[r][c].rect.y = (768 - w.enemies[r][c].rect.h) * 1 / 20 + yPos;
            
            //would determine weapon speed/hp
            //SDL_Rect rect;
            xPos += 80;
        }
        xPos = 50;
        yPos += 60;
    }
    xPos = 100;
    yPos = 5;
    //assigns and queries rocks or something idk
    /*for(int i = 0; i<sizeof(rocks)/sizeof(rocks[0]); i++) {
        SDL_QueryTexture(rockTexture, NULL, NULL, &rocks[i].rect.w, &rocks[i].rect.h);
        rocks[i].rect.w /= 4;
        rocks[i].rect.h /= 4;

        rocks[i].rect.x = (1024 - rocks[i].rect.w) * 1 / 20 + xPos;
        rocks[i].rect.y = (768 - rocks[i].rect.h) * 7 / 10;
        xPos += 300;
    }*/ 
    for(int i = 0; i < sizeof(rocks)/sizeof(rocks[0]); i++) {
        for(int r = 0; r<sizeof(rocks[0])/sizeof(rocks[0][0]); r++) {
            for(int c = 0; c< sizeof(rocks[0][0])/sizeof(rocks[0][0][0]); c++) {
                SDL_QueryTexture(rockTexture, NULL, NULL, &rocks[i][r][c].rect.w, &rocks[i][r][c].rect.h);
                rocks[i][r][c].rect.w /= 144;
                rocks[i][r][c].rect.h /= 144;

                rocks[i][r][c].rect.x = (1024 - rocks[i][r][c].rect.w) * 1 / 20 + xPos;
                rocks[i][r][c].rect.y = (768 - rocks[i][r][c].rect.h) * 7 / 10 + yPos;
                xPos +=10;
            }
            yPos += 10;
            xPos -= 80;
        }
        xPos += 300;
        yPos = 5;
    }


    //assigns rect properties to lasers
    for(int i = 0; i<sizeof(playerLasers)/sizeof(playerLasers[0]); i++) {
        SDL_QueryTexture(laserTexture, NULL, NULL, &playerLasers[i].rect.w, &playerLasers[i].rect.h);    
        playerLasers[i].velocity = 1;
        playerLasers[i].damage = 10;
        playerLasers[i].rect.w /= 16;
        playerLasers[i].rect.h /= 16;
        playerLasers[i].rect.x = 69000;
        playerLasers[i].rect.y = 69000;
        playerLasers[i].refill = 0;

    }

    for(int i = 0; i<sizeof(enemyLasers)/sizeof(enemyLasers[0]); i++) {
        SDL_QueryTexture(laserTexture, NULL, NULL, &enemyLasers[i].rect.w, &enemyLasers[i].rect.h);    
        enemyLasers[i].velocity = 1;
        enemyLasers[i].damage = 10;
        enemyLasers[i].rect.w /= 16;
        enemyLasers[i].rect.h /= 16;
        enemyLasers[i].rect.x = 69000;
        enemyLasers[i].rect.y = 69000;
        enemyLasers[i].refill = 0;

    }
    
    // annimation loop
    while (!stop) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // make go stop
                stop = 1;
                break;
 
            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    steve.rect.x -= steve.rectSpeed / 30;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    steve.rect.x += steve.rectSpeed / 30;
                    break;
                default:
                    break;
                }
            case SDL_KEYUP:
                switch(event.key.keysym.scancode) {
                    case SDL_SCANCODE_SPACE: {
                        //create laser that moves across screen
                        //until reaching border
                        //how can I manage bullet data?
                        //printf("initial steve x: %i", steve.rect.x);
                        if(!delay) {
                            playerLaser(&steve, playerLasers, sizeof(playerLasers), laserTexture);
                            delay = 1;
                        }
                        //printf("%i", playerLasers[0].rect.x);
                        break;
                    
                    }
                    case SDL_SCANCODE_P: {
                        if(cheats) {
                            killWave(&w);
                        }
                    }
                    case SDL_SCANCODE_R: {
                        reinitializeRocks(rocks, sizeof(rocks));
                    }
                    default:
                        break;
                }
            }
        }

        frame += 1;
        if(frame % 20 == 0) {
            delay = 0;
        }
        if(frame % 60 == 0) {
            enemyDelay = 0;
        }
        srand(time(NULL));

        

        if(!enemyDelay) {
            int exists = 0;
            int x, y;
            while(!exists) {
                x = rand() % (sizeof(w.enemies)/sizeof(w.enemies[0]));
                y = rand() % (sizeof(w.enemies[0])/sizeof(w.enemies[0][0]));
                if (!w.enemies[x][y].dead) {
                    exists = 1;
                }
            }
            enemyLaser(&w.enemies[x][y], enemyLasers, sizeof(enemyLasers), laserTexture);
            enemyDelay = 1;
        }
        
        if(GAMEOVER) {
            while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
                /*case SDL_QUIT:
                    // make go stop
                    stop = 1;
                    break;
 */

                case SDL_KEYUP:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_KP_ENTER: {
                            //create laser that moves across screen
                            //until reaching border
                            //how can I manage bullet data?
                         //printf("initial steve x: %i", steve.rect.x);
                            initializePlayer(&steve);
                            reinitializeWave(&w);
                            reinitializeRocks(rocks, sizeof(rocks));
                            //printf("%i", playerLasers[0].rect.x);
                            break;
                            }
                        case SDL_SCANCODE_ESCAPE: {
                            SDL_DestroyTexture(texture);
                            SDL_DestroyTexture(A1texture);
                            SDL_DestroyTexture(A2texture);
                            SDL_DestroyTexture(A3texture);
                            SDL_DestroyTexture(laserTexture);
                            SDL_DestroyTexture(rockTexture);
    
                            //kills the lasers
                            //free(playerLasers);

                            // rectroy renderer
                            SDL_DestroyRenderer(renderer);
 
                            // rectroy window
                            SDL_DestroyWindow(win);
     
                        // close SDL
                            SDL_Quit();
 
                        }
                        default:
                            break;
                }
            }
        }
            return 0;
        }

        


        //bro make a bounds function
        // right boundary
        if (steve.rect.x + steve.rect.w > 1024)
            steve.rect.x = 1024 - steve.rect.w;
 
        // left boundary
        if (steve.rect.x < 0)
            steve.rect.x = 0;

        for(int r = 0; r < sizeof(w.enemies)/sizeof(w.enemies[0]); r++) {
            for(int c = 0; c < sizeof(w.enemies[0])/sizeof(w.enemies[0][0]); c++) {
                if(waveDirection)
                    w.enemies[r][c].rect.x += 1;
                else if(!waveDirection) {
                    w.enemies[r][c].rect.x -= 1;
                }
                if (w.enemies[r][c].rect.x + w.enemies[r][c].rect.w > 1024 && !w.enemies[r][c].dead) {
                    w.enemies[r][c].rect.x = 1024 - w.enemies[r][c].rect.w;
                    waveDirection = !waveDirection;
                    shiftWaveDown(&w);
                }
                    
        // left boundary
                if (w.enemies[r][c].rect.x < 0 && !w.enemies[r][c].dead) {
                    w.enemies[r][c].rect.x = 0;
                    waveDirection = !waveDirection;
                    shiftWaveDown(&w);
                }
                    
        // bottom boundary
                if (w.enemies[r][c].rect.y + w.enemies[r][c].rect.h > 1024 && !w.enemies[r][c].dead) {
                    w.enemies[r][c].rect.y = 1024 - w.enemies[r][c].rect.h;
                    waveDirection = !waveDirection;
                }
                    
        // upper boundary
                if (w.enemies[r][c].rect.y < 0 && !w.enemies[r][c].dead) {
                    w.enemies[r][c].rect.y = 0;
                    waveDirection = !waveDirection;
                }
                    
            }
        }
        //make player lasers go up
        for(int i = 0; i < sizeof(playerLasers)/sizeof(playerLasers[0]); i++) {
            playerLasers[i].rect.y -= 3;
        }
        //make enemy lasers go down
        for(int i = 0; i < sizeof(enemyLasers)/sizeof(enemyLasers[0]); i++) {
            enemyLasers[i].rect.y += 3 * enemyLasers[i].velocity;
            if(enemyLasers[i].velocity != 1) {
                printf("%f\n", enemyLasers[i].velocity);
            }
        }
        int numDead = 0;
        for(int r = 0; r < sizeof(w.enemies)/sizeof(w.enemies[0]); r++) {
            for(int c = 0; c < sizeof(w.enemies[0])/sizeof(w.enemies[0][0]); c++) {
                for(int z = 0; z < sizeof(playerLasers)/sizeof(playerLasers[0]); z++) {

                        //SDL_CollidePixel
                        //player hits enemy
                        if(SDL_HasIntersection(&w.enemies[r][c].rect, &playerLasers[z].rect) && !w.enemies[r][c].dead) {
                            w.enemies[r][c].hp -= playerLasers[z].damage;
                            playerLasers[z].rect.x = 69000;
                        }
                        //laser hits rock
                        /*if(SDL_HasIntersection(&rocks[h].rect, &playerLasers[z].rect)) {
                            playerLasers[z].rect.x = 69000;
                        }
                        //enemy reach rock
                        if(SDL_HasIntersection(&w.enemies[r][c].rect, &rocks[h].rect) && !w.enemies[r][c].dead) {
                            GAMEOVER = 1;
                        } */
                        if(w.enemies[r][c].hp <= 0) {
                            w.enemies[r][c].dead = 1;
                        }
                }
                if(w.enemies[r][c].dead) {
                    numDead++;
                }
                if(numDead == 50) {
                    w.cleared = 1;
                }
            }
        }

        if(w.cleared) {
            //printf("cleared");
            reinitializeWave(&w);
        }

        for(int i = 0; i < sizeof(rocks)/sizeof(rocks[0]); i++) {
            for(int r = 0; r<sizeof(rocks[0])/sizeof(rocks[0][0]); r++) {
                for(int c = 0; c< sizeof(rocks[0][0])/sizeof(rocks[0][0][0]); c++) {
                    for(int j = 0; j < sizeof(playerLasers)/sizeof(playerLasers[0]); j++) {

                        if(SDL_HasIntersection(&rocks[i][r][c].rect, &playerLasers[j].rect)) {
                            playerLasers[j].rect.x = 69000;
                            rocks[i][r][c].rect.x = 69000;
                        }

                    }
                }
            }
        } 

        for(int i = 0; i < sizeof(rocks)/sizeof(rocks[0]); i++) {
            for(int r = 0; r<sizeof(rocks[0])/sizeof(rocks[0][0]); r++) {
                for(int c = 0; c< sizeof(rocks[0][0])/sizeof(rocks[0][0][0]); c++) {
                    for(int j = 0; j < sizeof(enemyLasers)/sizeof(enemyLasers[0]); j++) {

                        if(SDL_HasIntersection(&rocks[i][r][c].rect, &enemyLasers[j].rect)) {
                            enemyLasers[j].rect.x = 69000;
                            rocks[i][r][c].rect.x = 69000;
                        }

                    }
                }
            }
        }           
        for(int i = 0; i<sizeof(enemyLasers)/sizeof(enemyLasers[0]); i++) {
            if(SDL_HasIntersection(&steve.rect, &enemyLasers[i].rect)) {
                //printf("man down");
                enemyLasers[i].x = 69000;
                steve.hp -= enemyLasers[i].damage;
            }
        } 

        if(steve.hp <= 0 && !godmode) {
            GAMEOVER = 1;
        }

        // clears the screen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &steve.rect);
        for(int r = 0; r < sizeof(w.enemies)/sizeof(w.enemies[0]); r++) {
            for(int c = 0; c < sizeof(w.enemies[0])/sizeof(w.enemies[0][0]); c++) {
                if(w.enemies[r][c].type == 0 && !w.enemies[r][c].dead) {
                    SDL_RenderCopy(renderer, A1texture, NULL, &w.enemies[r][c].rect);
                }
                else if(w.enemies[r][c].type == 1 && !w.enemies[r][c].dead) {
                    SDL_RenderCopy(renderer, A2texture, NULL, &w.enemies[r][c].rect);
                }
                else if(w.enemies[r][c].type == 2 && !w.enemies[r][c].dead) {
                    SDL_RenderCopy(renderer, A3texture, NULL, &w.enemies[r][c].rect);
                }
            }
        }
        
        //printf("%ld", sizeof(playerLasers)/sizeof(playerLasers[0]));
        for(int i = 0; i < sizeof(playerLasers)/sizeof(playerLasers[0]); i++) {
            SDL_RenderCopy(renderer, laserTexture, NULL, &playerLasers[i].rect);
        }

        for(int i = 0; i < sizeof(enemyLasers)/sizeof(enemyLasers[0]); i++) {
            SDL_RenderCopy(renderer, laserTexture, NULL, &enemyLasers[i].rect);
        }
        /*for(int i = 0; i < sizeof(rocks)/sizeof(rocks[0]); i++) {
            SDL_RenderCopy(renderer, rockTexture, NULL, &rocks[i].rect);
        }*/
        for(int i = 0; i < sizeof(rocks)/sizeof(rocks[0]); i++) {
            for(int r = 0; r<sizeof(rocks[0])/sizeof(rocks[0][0]); r++) {
                for(int c = 0; c< sizeof(rocks[0][0])/sizeof(rocks[0][0][0]); c++) {
                    SDL_RenderCopy(renderer, rockTexture, NULL, &rocks[i][r][c].rect);
                }
            }
        }

        // triggers the double buffers
        // for multiple rendering - dunno what this does tbh
        SDL_RenderPresent(renderer);
 
        // calculates to 24 fps
        SDL_Delay(1000 / 24);
    }
 
    // rectroy texture
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(A1texture);
    SDL_DestroyTexture(A2texture);
    SDL_DestroyTexture(A3texture);
    SDL_DestroyTexture(laserTexture);
    SDL_DestroyTexture(rockTexture);
    
    //kills the lasers
    //free(playerLasers);

    // rectroy renderer
    SDL_DestroyRenderer(renderer);
 
    // rectroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();
 
    return 0;
}

void shiftWaveDown(wave* w) {
    for(int r = 0; r < sizeof(w->enemies)/sizeof(w->enemies[0]); r++) {
            for(int c = 0; c < sizeof(w->enemies[0])/sizeof(w->enemies[0][0]); c++) {
                w->enemies[r][c].rect.y += 20;
            }
        }
}

int detectColision(SDL_Rect a, SDL_Rect b) {
    if (a.y + a.h < b.y) {
		return 0;
	}
				
	if (a.y > b.y + b.h) {				
		return 0;
	}
				
	if (a.x > b.x + b.w) {				
		return 0;
	}
			
	if (a.x + a.w < b.x) {				
		return 0;
	}
	return 1;
}

void initializePlayer(player* x) {
    x->hp = PLAYER_HP;
    //velocity acts as a multiplier of boxspeed;
    x->velocity = 1;
    //boolean for whether abilities are enabled;
    x->speedBoost = 0;
    x->damageBoost = 0;
    x->score = 0;
    x->name = "steve";
    x->rect.x = (1024 - x->rect.w) / 2;
    x->rect.y = (768 - x->rect.h) * 19 / 20;
}

void reinitializeRocks(rock rocks[][8][8], size_t rSize) {
    int xPos = 100;
    int yPos = 5;
    for(int i = 0; i < rSize/sizeof(rocks[0]); i++) {
        for(int r = 0; r < sizeof(rocks[0])/sizeof(rocks[0][0]); r++) {
            for(int c = 0; c < sizeof(rocks[0][0])/sizeof(rocks[0][0][0]); c++) {
                rocks[i][r][c].rect.x = (1024 - rocks[i][r][c].rect.w) * 1 / 20 + xPos;
                rocks[i][r][c].rect.y = (768 - rocks[i][r][c].rect.h) * 7 / 10 + yPos;
                xPos += 10;
            }
            yPos += 10;
            xPos -= 80;
        }
        xPos += 300;
        yPos = 5;
    }

}

void initializeEnemy(enemy* x, int type) {
    switch(type) {
        case 0:
            x->type = 0;
            x->hp = ALIEN_ONE_HP;
            x->velocity = 1;
            x->dead = 0;
            break;
        case 1:
            x->type = 1;
            x->hp = ALIEN_TWO_HP;
            x->velocity = 1;
            x->dead = 0;
            break;
        case 2:
            x->type = 2;
            x->hp = ALIEN_THREE_HP;
            x->velocity = 1;
            x->dead = 0;
            break;
        default:
            break;
    }
}

void reinitializeWave(wave* w) {
    w->cleared = 0;
    int yPos = 10;
    int xPos = 50;
    for(int r = 0; r < sizeof(w->enemies)/sizeof(w->enemies[0]); r++) {
        for(int c = 0; c < sizeof(w->enemies[0])/sizeof(w->enemies[0][0]); c++) {
            switch(w->enemies[r][c].type) {
                case 0:
                    w->enemies[r][c].hp = ALIEN_ONE_HP;
                case 1:
                    w->enemies[r][c].hp = ALIEN_TWO_HP;
                case 2: 
                    w->enemies[r][c].hp = ALIEN_THREE_HP;
                 
            }
            w->enemies[r][c].dead = 0;
            w->enemies[r][c].rect.x = (1024 - w->enemies[r][c].rect.w) * 1 / 20 + xPos;
 
                // sets initial y-position
            w->enemies[r][c].rect.y = (768 - w->enemies[r][c].rect.h) * 1 / 20 + yPos;
            xPos += 80;
        } 
        xPos = 50;
        yPos += 60;
    }       
}

void killWave(wave* w) {
    for(int r = 0; r < sizeof(w->enemies)/sizeof(w->enemies[0]); r++) {
        for(int c = 0; c < sizeof(w->enemies[0])/sizeof(w->enemies[0][0]); c++) {
            w->enemies[r][c].dead = 1;
        } 
    }  
}

void manageBounds(SDL_Rect* r) {

}

void playerLaser(player* steve, laser playerLasers[100], size_t pLSize, SDL_Texture* laserTexture) {
    //printf("banged lol");
    //make so once reach the maximum, start storing new
    //lasers in the bottom
    //printf("%i\n", playerLasers[0].rect.x);

    for(int i = 0; i < 100; i++) {
        if(i == 99) {
            for(int j = 0; j < pLSize/sizeof(playerLasers[0]); j++) {
                playerLasers[j].refill = 1;
                //printf("there has been a refill");
            }
        }

        if(playerLasers[i].rect.x == 69000) {
            //printf("it's null");
            //printf("%i", steve->rect.x);
            playerLasers[i].rect.x = steve->rect.x + 25;
            playerLasers[i].rect.y = steve->rect.y - 5;
            //SDL_QueryTexture(laserTexture, NULL, NULL, &playerLasers[i].rect.w, &playerLasers[i].rect.h);
            //printf("%i", i);
            break;
        }

        if(playerLasers[i].refill) {
            //printf("refilled");
            playerLasers[i].rect.x = steve->rect.x + 25;
            playerLasers[i].rect.y = steve->rect.y - 5;
            //SDL_QueryTexture(laserTexture, NULL, NULL, &playerLasers[i].rect.w, &playerLasers[i].rect.h);
            playerLasers[i].refill = 0;
            break;
        }
    }
}

void enemyLaser(enemy* bad, laser enemyLasers[500], size_t eLSize, SDL_Texture* laserTexture) {
    
    for(int i = 0; i < 500; i++) {
        if(i == 499) {
            for(int j = 0; j < eLSize/sizeof(enemyLasers[0]); j++) {
                enemyLasers[j].refill = 1;
                //printf("there has been an enemy refill");
            }
        }

        if(enemyLasers[i].rect.x == 69000) {
            //printf("it's null");
            //printf("%i", steve->rect.x);
            enemyLasers[i].rect.x = bad->rect.x + 25;
            enemyLasers[i].rect.y = bad->rect.y - 5;
            if(bad->type = 0) {
                enemyLasers[i].velocity = 3;
                enemyLasers[i].damage = 20;
            }
            else if (bad->type = 1){
                enemyLasers[i].velocity = 1;
                enemyLasers[i].damage = 35;
            }
            else if (bad->type = 2){
                enemyLasers[i].velocity = .75;
                enemyLasers[i].damage = 65;
            }
            break;
        }

        if(enemyLasers[i].refill) {
            printf("refilled");
            enemyLasers[i].rect.x = enemyLasers->rect.x + 25;
            enemyLasers[i].rect.y = enemyLasers->rect.y - 5;
            if(bad->type = 0) {
                enemyLasers[i].velocity = 3;
                enemyLasers[i].damage = 20;
            }
            else if (bad->type = 1){
                enemyLasers[i].velocity = 1.25;
                enemyLasers[i].damage = 35;
            }
            else if (bad->type = 2){
                enemyLasers[i].velocity = .75;
                enemyLasers[i].damage = 65;
            }
            enemyLasers[i].refill = 0;
            break;
        }
    }
}

void destroyEnemy(enemy* x) {
    
}