/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "settings.cpp"

#include "lTexture.cpp"
#include "dot.cpp"
#include "lTimer.cpp"
#include "weapons.cpp"
#include "fighters.cpp"
#include "explosion.cpp"

bool init();
bool loadMedia();
void close();

//The window we'll be rendering to
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
LTexture dotTexture;
LTexture bgTexture;
LTexture bulletTexture;
LTexture trump1Texture;
LTexture trump2Texture;
LTexture trump3Texture;

const int explosionFrames = 25;
SDL_Rect explosionClips[explosionFrames];
LTexture explosionSheet;

bool init(){
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not nitialize! SDL_Errpr: %s\n", SDL_GetError() );
		success = false;
	}else{
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}else{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(renderer == NULL){
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			dotTexture.loadRenderer(renderer);
			bgTexture.loadRenderer(renderer);
			bulletTexture.loadRenderer(renderer);
			trump1Texture.loadRenderer(renderer);
			trump2Texture.loadRenderer(renderer);
			trump3Texture.loadRenderer(renderer);
			explosionSheet.loadRenderer(renderer);
		}
	}
	return success;
}

bool loadMedia(){
	bool success = true;

	if(!dotTexture.loadFromFile("images/jet1.png")){
		printf( "Failed to load dot texture!\n" );
		success=false;
	}
	if(!bgTexture.loadFromFile("backgrounds/background.png")){
		printf( "Failed to load bg texture!\n" );
		success=false;
	}
	if(!bulletTexture.loadFromFile("images/tinydot.png")){
		printf( "Failed to load tinydot texture!\n" );
		success=false;
	}
	if(!trump1Texture.loadFromFile("images/trump1.png")){
		printf( "Failed to load tinydot texture!\n" );
		success=false;
	}
	if(!trump2Texture.loadFromFile("images/trump2.png")){
		printf( "Failed to load tinydot texture!\n" );
		success=false;
	}
	if(!trump3Texture.loadFromFile("images/trump3.png")){
		printf( "Failed to load tinydot texture!\n" );
		success=false;
	}
	if( !explosionSheet.loadFromFile( "images/explosion.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }else{
    	int counter = 0;
        for(int row = 0; row < 225; row+=45){
        	for(int col = 0; col < 225; col+=45){
		        explosionClips[ counter ].x =   col;
		        explosionClips[ counter ].y =   row;
		        explosionClips[ counter ].w =   45;
		        explosionClips[ counter ].h =   45;
		    	++counter;	
		    }
	    }
    }

	return success;
}



void close(){
	dotTexture.free();
	bgTexture.free();
	bulletTexture.free();
	//Destroy window
	SDL_DestroyWindow( window );
	window = NULL;
	SDL_DestroyRenderer( renderer );
	renderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] ){
	if(!init()){
		printf("Error on init \n");
	}else{
		if(!loadMedia()){
			printf("Error on loadMedia \n");
		}else{
			
			printf("Start \n");
			bool quit = false;
			SDL_Event e;
			LTimer fpsTimer;
			LTimer capTimer;

			int countedFrames = 0;
			fpsTimer.start();

			int frame = 0;
			
			Dot dot(0,0);
			std::vector<Weapons*> bullets;
			std::vector<Fighters*> trumps;
			std::vector<Explosions*> explos;
			std::vector<SDL_Rect> bulletBoxes;
			LTexture * trumpImgs[] = {&trump1Texture, &trump2Texture, &trump3Texture};
			dot.loadBullets(&bullets);

			int scrollingOffset = 0;

			srand(time(NULL));

			while(!quit){
				capTimer.start();
				
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					dot.handleEvent(e);
				}

				float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
				if(avgFPS > 2000000){
					avgFPS = 0;
				}

				--scrollingOffset;
				if(scrollingOffset < -bgTexture.getWidth()){
					scrollingOffset = 0;
				}

				if((rand() % 50) == 1){
					Fighters* trumper = new Fighters(SCREEN_WIDTH, (rand() % SCREEN_HEIGHT), (rand() % 3), trumpImgs[(rand() % 3)]);
					trumps.push_back(trumper);
				}

				dot.move(trumps);

				//Clear screen
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(renderer);

				//renderBackground
				bgTexture.render(scrollingOffset, 0);
				bgTexture.render(scrollingOffset + bgTexture.getWidth(), 0);

				//Render objects
				dot.render(&dotTexture);

				for(int counter = 0; counter < bullets.size(); counter++){
					Weapons *bullet = bullets[counter];
					if(bullet->getPosX() < SCREEN_WIDTH){
						bullet->render(&bulletTexture);
						bullet->increment();
					}else{
						bullet->setIsDead(true);
					}
				}
				for(int counter = 0; counter < bullets.size(); counter++){
					Weapons *bullet = bullets[counter];
					if(bullet->isDead()){
						bullets[counter] = bullets[bullets.size() - 1];
						bullets[bullets.size() - 1] = bullet;
						bullets.pop_back();
						delete bullet;
						--counter;
					}
				}	

				for(int counter = 0; counter < trumps.size(); counter++){
					Fighters *trump = trumps[counter];
					if(trump->getPosX() > 3){
						trump->render();
						bool hit = trump->increment(&bullets);
						if(hit == true){
							trump->setIsDead(true);
						}
					}else{
						trump->setIsDead(true);
					}
				}
				for(int counter = 0; counter < trumps.size(); counter++){
					Fighters *trump = trumps[counter];
					if(trump->isDead()){
						Explosions* explo = new Explosions(trump->getPosX(), trump->getPosY());
						explo->loadClips(explosionClips);
						explos.push_back(explo);

						trumps[counter] = trumps[trumps.size() - 1];
						trumps[trumps.size() - 1] = trump;
						trumps.pop_back();
						delete trump;
						--counter;
					}
				}	

				for(int counter = 0; counter < explos.size(); counter++){
					Explosions *explo = explos[counter];
					if(explo->isDead()){
						explos[counter] = explos[explos.size() - 1];
						explos[explos.size() - 1] = explo;
						explos.pop_back();
						delete explo;
						--counter;
					}else{
						explo->render(&explosionSheet);
						explo->increment();
					}
				}	

				//Update screen
				SDL_RenderPresent(renderer);
				
				++frame;
				if(frame / 25 >= explosionFrames){
					frame = 0;
				}



				++countedFrames;

				int frameTicks = capTimer.getTicks();
				if(frameTicks < SCREEN_TICKS_PER_FRAME){
					SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
				}
			}
		}
	}

	close();

	printf("End \n");
	return 0;
}