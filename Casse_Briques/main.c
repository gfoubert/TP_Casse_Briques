#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780


//Option de build dans les linker setting mettre les libSDL2main.a libSDL2.dll.a
//Dans search directory
//Option de build compiler mettre les include
//Option de build linker mettre les lib

//mettre dans build linker -lmingw32

typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_texture;
     SDL_Surface *g_surface;


}game;

typedef struct gameState{

    int g_bRunning;
    int left;
    int right;


}gameState;


typedef struct coordonnees{

    double x;
    double y;



}coordonnees;


typedef struct ball{

    double x;
    double y;
    double direction;



}ball;

typedef enum destroyed{
True,False



}destroyed;


typedef struct SDL_Rect2{

    SDL_Rect brique;
    destroyed destroyed;




}SDL_Rect2;







int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame);
void handleEvents(gameState *state, SDL_Rect *paddle);
void cutBitmapTexture(game *myGame,gameState state, SDL_Rect paddle, ball ball,SDL_Rect2 briques[32]);
void delay(unsigned int frameLimit);
void destroyTexture(game *myGame);
void destroy(game *myGame);
void deplacementBalle(game *MyGame,gameState state, ball *ball, SDL_Rect paddle);
void initbriques(SDL_Rect2 briques[32]);



int main(int argc, char *argv[])
{

     game myGame;
     gameState state;
     double *paddlex=(15);
     double *paddley=(SCREEN_HEIGHT-10);
     SDL_Rect paddle;
     ball ball;
     int i;
     ball.x=SCREEN_WIDTH-50;
     ball.y=SCREEN_HEIGHT/2;
     ball.direction=3;
     paddle.x=15;
     paddle.y=(SCREEN_HEIGHT-10);
     paddle.w=80;
     SDL_Rect2 briques[32];

     initbriques(briques);
     for(i=0;i<32;i++){

        printf("%d  ", briques[i].brique.x);
     }





    //Pour les 60 fps
    unsigned int frameLimit = SDL_GetTicks() + 16;


    if(init("Chapter 1 setting up SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame)){

                state.g_bRunning=1;


    }else{

            return 1;//something's wrong

    }


    state.right=1;
    state.left=0;

    while(state.g_bRunning){

           handleEvents(&state,&paddle);
           deplacementBalle(&myGame,state,&ball, paddle);
           cutBitmapTexture(&myGame,state,paddle,ball,briques);


        // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;


    }

        destroy(&myGame);



        // unload the dynamically loaded image libraries
        IMG_Quit();
        //quit sdl2
        SDL_Quit();




  return 0;
}


int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame){


    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,height,width,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_ACCELERATED);
                SDL_SetRenderDrawColor(myGame->g_pRenderer, 0, 0, 0, 255);
                SDL_RenderClear(myGame->g_pRenderer);

                int flags=IMG_INIT_JPG|IMG_INIT_PNG;
                int initted=IMG_Init(flags);
                if((initted&flags) != flags) {
                        printf("IMG_Init: Failed to init required jpg and png support!\n");
                        printf("IMG_Init: %s\n", IMG_GetError());
                        // handle error
                }

            }


    }else{


        return 0;
    }




    return 1;


}

void handleEvents(gameState *state, SDL_Rect *paddle){


    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                            {
                                case SDLK_LEFT:state->left=1;state->right=0;paddle->x=paddle->x-20;
                                if (paddle->x<-10){
                                    paddle->x=paddle->x+20;
                                }  break;
                                case SDLK_RIGHT: state->right=1;state->left=0;paddle->x=paddle->x+20;
                                 if (paddle->x>(SCREEN_WIDTH-paddle->w)){
                                    paddle->x=paddle->x-20;
                                }
                                  break;
                                case SDLK_UP:    ; break;
                                case SDLK_DOWN:  ; break;
                                case SDLK_d:  break;
                                case SDLK_q:  break;
                            }
                            break;

        case SDL_KEYUP:;break;

        default:break;

        }
    }


}



void cutBitmapTexture(game *myGame,gameState state, SDL_Rect paddle, ball ball, SDL_Rect2 briques[32]){

        SDL_Rect rectangleDest;
        SDL_Rect rectangleSource;
        int i;


                myGame->g_surface = IMG_Load("./assets/paddle.png");//Chargement de l'image png






        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){
                    //1s = 1000ms
                    rectangleSource.x=0;//1 image tous les 100ms
                    rectangleSource.y=0;
                    rectangleSource.w=128;//1 image = w:128 et h:82
                    rectangleSource.h=32;




                    //Définition du rectangle dest pour dessiner le png
                    rectangleDest.x=paddle.x;//debut x
                    rectangleDest.y=paddle.y;//debut y
                    rectangleDest.w=100; //Largeur
                    rectangleDest.h=10; //Hauteur



                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest);


                    /*if(state.right)
                        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest); // Copie du sprite grâce au SDL_Renderer
                    if(state.left)
                        SDL_RenderCopyEx(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest,0,0,SDL_FLIP_HORIZONTAL);*/

                    //SDL_FLIP_NONE
                    //SDL_FLIP_HORIZONTAL
                    //SDL_FLIP_VERTICAL




                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }

//Chargement de la balle



                myGame->g_surface = IMG_Load("./assets/ball.png");//Chargement de l'image png






        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){
                    //1s = 1000ms

                       rectangleSource.x=0;//1 image tous les 100ms
                    rectangleSource.y=0;
                    rectangleSource.w=24;//1 image = w:128 et h:82
                    rectangleSource.h=24;

                    //Définition du rectangle dest pour dessiner le png
                 rectangleDest.x=ball.x;//debut x
                    rectangleDest.y=ball.y;//debut y
                    rectangleDest.w=24; //Largeur
                    rectangleDest.h=24; //Hauteur


                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest);


                    /*if(state.right)
                        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest); // Copie du sprite grâce au SDL_Renderer
                    if(state.left)
                        SDL_RenderCopyEx(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest,0,0,SDL_FLIP_HORIZONTAL);*/

                    //SDL_FLIP_NONE
                    //SDL_FLIP_HORIZONTAL
                    //SDL_FLIP_VERTICAL



                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }




//Chargement des blocs

for (i=0;i<32;i++){

                myGame->g_surface = IMG_Load("./assets/bricks.png");//Chargement de l'image png






        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){
                    //1s = 1000ms

                switch(i%4){
                case 0:   rectangleSource.x=0;//1 image tous les 100ms
                    rectangleSource.y=0;
                    break;

                case 1: rectangleSource.x=64;//1 image tous les 100ms
                    rectangleSource.y=0;
                    break;

                 case 2:   rectangleSource.x=0;//1 image tous les 100ms
                    rectangleSource.y=24;
                    break;

                case 3: rectangleSource.x=64;//1 image tous les 100ms
                    rectangleSource.y=24;
                    break;

                }
                    rectangleSource.w=64;//1 image = w:128 et h:82
                    rectangleSource.h=24;

                    //Définition du rectangle dest pour dessiner le png
                    rectangleDest.x=briques[i].brique.x;//debut x
                    rectangleDest.y=briques[i].brique.y;//debut y
                    rectangleDest.w=briques[i].brique.w; //Largeur
                    rectangleDest.h=briques[i].brique.h; //Hauteur


                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest);


                    /*if(state.right)
                        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest); // Copie du sprite grâce au SDL_Renderer
                    if(state.left)
                        SDL_RenderCopyEx(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest,0,0,SDL_FLIP_HORIZONTAL);*/

                    //SDL_FLIP_NONE
                    //SDL_FLIP_HORIZONTAL
                    //SDL_FLIP_VERTICAL



                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }


}
























  SDL_RenderPresent(myGame->g_pRenderer); // Affichage

                    SDL_RenderClear(myGame->g_pRenderer);





















    destroyTexture(myGame);

}

void delay(unsigned int frameLimit){
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}




void destroy(game *myGame){

    //Destroy render
    if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);


    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}



void destroyTexture(game *myGame){

    //Destroy texture
    if(myGame->g_texture!=NULL)
            SDL_DestroyTexture(myGame->g_texture);


}

void deplacementBalle(game *MyGame,gameState state, ball *ball, SDL_Rect paddle){

if (ball->direction==1){
        ball->x=ball->x+3;
ball->y=ball->y+3;
}


if (ball->direction==2){
        ball->x=ball->x+3;
ball->y=ball->y-3;
}


if (ball->direction==3){
        ball->x=ball->x-3;
ball->y=ball->y+3;
}


if (ball->direction==4){
        ball->x=ball->x-3;
ball->y=ball->y-3;
}

if (ball->x>SCREEN_WIDTH-24){
    ball->x=ball->x-3;

   if( ball->direction==1){
    ball->direction=3;
   }

   if( ball->direction==2){
    ball->direction=4;
   }

}


if (ball->x<0){
    ball->x=ball->x+3;

   if( ball->direction==3){
    ball->direction=1;
   }

   if( ball->direction==4){
    ball->direction=2;
   }

}



if (ball->y<0){
    ball->y=ball->y+3;

   if( ball->direction==2){
    ball->direction=1;
   }

   if( ball->direction==4){
    ball->direction=3;
   }

}


if ((ball->y>paddle.y)&&(ball->y<SCREEN_HEIGHT)&&(ball->x>paddle.x)&&(ball->x<(paddle.x+paddle.w))){
    ball->y=ball->y-3;

   if( ball->direction==1){
    ball->direction=2;
   }

   if( ball->direction==3){
    ball->direction=4;

}




}
}

void initbriques(SDL_Rect2 briques[32]){
int i,x,y;
x=0;
y=24;

for(i=0;i<32;i++){
  briques[i].brique.x=x;
  briques[i].brique.y=y;
  briques[i].brique.w=64;
  briques[i].brique.h=24;
  briques[i].destroyed=False;
  x=x+64;
  if(i==15){
        y=y+24;
        x=0;
  }

}























}
