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

bool init();
bool loadMedia();
void close();

//The window we'll be rendering to
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
LTexture dotTexture;
LTexture bgTexture;
LTexture bulletTexture;

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

			Dot dot(0,0);
			std::vector<Weapons*> bullets;
			std::vector<SDL_Rect> bulletBoxes;
			dot.loadBullets(&bullets);

			std::vector<SDL_Rect> jets;
			jets.resize(0);

			int scrollingOffset = 0;

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

				dot.move(bulletBoxes, jets);
				--scrollingOffset;
				if(scrollingOffset < -bgTexture.getWidth()){
					scrollingOffset = 0;
				}

				//Clear screen
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(renderer);

				//renderBackground
				bgTexture.render(scrollingOffset, 0);
				bgTexture.render(scrollingOffset + bgTexture.getWidth(), 0);

				//Render objects
				dot.render(&dotTexture);

				printf("bulletArr %d \n", bullets.size());

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

				//Update screen
				SDL_RenderPresent(renderer);
				
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