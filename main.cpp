/* Spacefox 2D shooting game

Written by Thodoris Kouleris


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.


 */
#include<cstdio>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>

//constants
const int WIDTH = 1024;
const int HEIGHT = 768;
const int BPP = 32;

//events
SDL_Event event;
SDL_Event event_f1;

//sprites
SDL_Surface *screen = nullptr;
SDL_Surface *bg = nullptr;
SDL_Surface *sf = nullptr;
SDL_Surface *laser = nullptr;
SDL_Surface *enemy = nullptr;
SDL_Surface *enemy_bullet = nullptr;
SDL_Surface *title_screen = nullptr;
SDL_Surface *try_again = nullptr;
SDL_Surface *go = nullptr;
SDL_Surface *grtf = nullptr;



SDL_Rect offsetBG;
SDL_Rect offsetTMP;
SDL_Rect offsetSCR;
SDL_Rect offsetSF;
SDL_Rect offsetLSR;
SDL_Rect offsetENM[5];
SDL_Rect offsetEBL;
SDL_Rect offsetTRY;

//sounds
Mix_Chunk *sfx_laser = nullptr;

bool quit;
bool visible[5]; 
bool f1 = false;

int xVelSF = 0;
int i = 0;
int score = 0;
int lives = 2;

void delay(){
	for(int i = 0; i<100000;i++){
	    for(int j = 0; j<8899;j++){

	    }
	}
}
//init_media() function initializes Audio and Video, loads sprites, backgrounds and sound
void init_media(){
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
		printf("SDL Cannot Initialize\n");
	}else{
		printf("SDL Initialized...\n");
	}
	Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
	sfx_laser = Mix_LoadWAV( "./sfx/sfx_laser.wav" );
	if( sfx_laser == nullptr ){
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	screen = SDL_SetVideoMode(WIDTH,HEIGHT,BPP,SDL_DOUBLEBUF | SDL_HWSURFACE);
	SDL_WM_SetCaption("SpaceFox",nullptr);
	bg = IMG_Load("./images/background_1.png");
	sf = IMG_Load("./images/blackknight.png");
	laser = IMG_Load("./images/laser.png");
	enemy = IMG_Load("./images/enemy1.png");
	enemy_bullet = IMG_Load("./images/enemy_bullet.png");
	title_screen = IMG_Load("./images/titlescreen.png");
	try_again = IMG_Load("./images/lose.png");
	go = IMG_Load("./images/gameover.png");
	grtf = IMG_Load("./images/grtf.png");
	SDL_SetColorKey(sf, SDL_SRCCOLORKEY,SDL_MapRGB(sf->format,255,0,255));
	SDL_SetColorKey(laser, SDL_SRCCOLORKEY,SDL_MapRGB(laser->format,255,0,255));
	SDL_SetColorKey(enemy, SDL_SRCCOLORKEY,SDL_MapRGB(enemy->format,255,0,255));	
	SDL_SetColorKey(enemy_bullet, SDL_SRCCOLORKEY, SDL_MapRGB(enemy_bullet->format,255,0,255));
}


void titlescreen(){
	//Screen offset
	offsetSCR.x = 0;
	offsetSCR.y = 0;
	offsetSCR.w = 1024;
	offsetSCR.h = 768;

	offsetTMP.x = 0;
	offsetTMP.y = 0;
	offsetTMP.w = offsetSCR.w;
	offsetTMP.h = offsetSCR.h;

	SDL_BlitSurface( title_screen, &offsetTMP, screen, &offsetSCR );
	SDL_Flip(screen);
	while(!f1){
		while(SDL_PollEvent(&event_f1)){
			if(event_f1.type==SDL_KEYDOWN){
				switch(event_f1.key.keysym.sym){
					case SDLK_F1: f1 = true;break;
				}
			}	
		}
	}
}

void set_positions(){
	//Background offset
		offsetBG.x = 0;
		offsetBG.y = 768;
		offsetBG.w = 1024;
		offsetBG.h = 768;

	
		
		//Spacefox offset
		offsetSF.x = 370;
		offsetSF.y = 620;
		offsetSF.w = 110;
		offsetSF.h = 151;
	
		//Laser offset
		offsetLSR.x = offsetSF.x + 55;
		offsetLSR.y = 0;
		offsetLSR.w = 615;

		//temporary offset
		offsetTMP.x = offsetSF.x;
		offsetTMP.y = offsetSF.y;
		offsetTMP.w = 110;
		offsetTMP.h = 151;

		//enemy offset
		offsetENM[0].x = 150;
		offsetENM[0].y = 150;
		offsetENM[0].w = 90;
		offsetENM[0].h = 130;


		for(i = 1; i<= 4; i++){
			offsetENM[i].x = offsetENM[i-1].x+140;
			offsetENM[i].y = offsetENM[0].y;
			offsetENM[i].w = offsetENM[0].w;
			offsetENM[i].h = offsetENM[0].h;
		}

		//enemy bullet
		i = rand()%5;
		offsetEBL.x = offsetENM[i].x + 45;
		offsetEBL.y = offsetENM[i].y + offsetENM[i].h;
		offsetEBL.w = 5;
		offsetEBL.h = 5;
}

int main(int argc,char *argv[]){

	//initialize Audio, Video and load pictures and sounds
	init_media();

	//Loads the title screen and waits until F1 is pressed
	titlescreen();

	//Maximum number of enemies
	int enemies = 5;
	
	//Here Lives check
	while((lives != 0) || (enemies != 0)){
		
		//screen before a level starts
		SDL_BlitSurface( grtf, &offsetSCR, screen, &offsetSCR);
		SDL_Flip(screen);
		delay();
	 	for(i = 0; i<=4;i++)
			visible[i]=true;
		enemies = 5;
		
		//sets everything on the screen	
		set_positions();

		SDL_BlitSurface( bg, &offsetBG, screen, &offsetSCR );
		SDL_BlitSurface( sf, NULL, screen,  &offsetSF );

		
		for(i = 0; i<=4; i++){
			SDL_BlitSurface( enemy, NULL, screen, &offsetENM[i] );
		}

		SDL_Flip(screen);
		quit = false;
		while(!quit){
			offsetLSR.x = offsetSF.x + 55;
			SDL_BlitSurface(bg, &offsetEBL,screen,&offsetEBL);
			offsetEBL.y = offsetEBL.y + 1;

			if(offsetEBL.y == 768){
				score = score + 200;
				while(visible[i = rand()%5]!=true){
					
				}
				offsetEBL.x = offsetENM[i].x + 45;
				offsetEBL.y = offsetENM[i].y + offsetENM[i].h;
			}
			while(SDL_PollEvent(&event)){
				if(event.type == SDL_QUIT){
					quit = true;
					lives = 0;
					enemies = 0;
				}
				if(event.type==SDL_KEYDOWN){
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE: quit = true; lives = 0; enemies = 0; break;
					}
				}	
				if(event.type==SDL_KEYDOWN){
					switch(event.key.keysym.sym){
					case SDLK_SPACE:
						Mix_PlayChannel(-1,sfx_laser,0);
						SDL_BlitSurface (laser,NULL,screen,&offsetLSR);
						SDL_Flip(screen);
						for(i = 0 ; i <= 4; i++ ){
						if((offsetLSR.x + 6 > offsetENM[i].x+17) && (offsetLSR.x < offsetENM[i].x+75)){
							if(visible[i] == true){
								enemies = enemies - 1;
								score = score + 100 * (enemies + 5);
							}
							SDL_BlitSurface (bg,&offsetENM[i],screen,&offsetENM[i]);
							SDL_Flip(screen);
							visible[i] = false;
							if(enemies == 0)quit = true;
							SDL_BlitSurface(bg,&offsetLSR,screen,&offsetLSR);}
						else{
							SDL_Flip(screen);
							SDL_BlitSurface(bg,&offsetLSR,screen,&offsetLSR);
						}
					}
					break;
					}
				}
				if(event.type==SDL_KEYDOWN){
					switch(event.key.keysym.sym){
						case SDLK_RIGHT: xVelSF =+ 3;break; 
						case SDLK_LEFT: xVelSF =- 3;break;
					}
				}
				else if(event.type==SDL_KEYUP){
					switch(event.key.keysym.sym){
						case SDLK_RIGHT: xVelSF--;break;
						case SDLK_LEFT: xVelSF++;break;
					}
				}
			}
			//Apply image to screen
			SDL_BlitSurface( bg, &offsetTMP, screen, &offsetSF);
			SDL_BlitSurface( enemy_bullet,nullptr,screen,&offsetEBL);
			SDL_BlitSurface( sf, nullptr, screen,  &offsetSF );
			/*if(offsetEBL.y = offsetSF.y){
				printf("SF = %d",offsetSF.y);
				printf("\nEBL = %d",offsetEBL.y);
				quit = true;
	
			}*/
	   		SDL_Flip( screen );
			if(((offsetEBL.y >= offsetSF.y) && ((offsetEBL.x > (offsetSF.x + 38))&&(offsetEBL.x < offsetSF.x + (offsetSF.w - 37)))) || (offsetEBL.y >= (offsetSF.y + 104)&&(offsetEBL.x > (offsetSF.x + 17))&&(offsetEBL.x < (offsetSF.x + offsetSF.w - 15)))){
				lives = lives - 1;
				printf("Lives: %d\n",lives);
				if(lives !=0){
					SDL_BlitSurface( try_again, &offsetSCR, screen, &offsetSCR);
					SDL_Flip(screen);
					delay();
				}
				if(lives == 0){
					enemies = 0;
					quit = true;
				}
				break;
			}
			offsetTMP.y = offsetSF.y;	
			offsetSF.x = offsetSF.x + xVelSF;
			offsetTMP.x = offsetSF.x;
	
			//This hack makes the sprite stops after the keypress is up
			if(xVelSF == 2 || xVelSF == -2){
				xVelSF = 0;
			}
			if(xVelSF == -4) 
				xVelSF = -3;
			if(xVelSF == 4)
				xVelSF = 3;
			
			//Not to allow the sprite goes out of screen
			if(offsetSF.x >= 914)
				offsetSF.x = 914;
	
	   		//Update Screen
			//printf("SF.x = %d\n",offsetSF.x);
			//printf("TMP.x = %d\n\n",offsetTMP.x);
		}
	}

	//Game Over Sreen
	SDL_BlitSurface( go, &offsetSCR, screen, &offsetSCR);
	SDL_Flip(screen);
	delay();
		
	//clearing the memory
	if(quit){
		Mix_FreeChunk(sfx_laser);
		SDL_FreeSurface(bg);
		SDL_FreeSurface(sf);
		SDL_FreeSurface(go);
		SDL_FreeSurface(title_screen);
		SDL_FreeSurface(laser);
		SDL_FreeSurface(enemy);
		SDL_FreeSurface(screen);
		SDL_FreeSurface(enemy_bullet);
		Mix_CloseAudio();
		SDL_Quit();
		printf("Score: %d\n",score);
		printf("Quitting...\n");
	}
	return 0;
}
