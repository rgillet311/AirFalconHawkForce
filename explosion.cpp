#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

class Explosions{
	public: 
		Explosions(int x, int y);

		void render(LTexture* explosionSheet);
		void increment();
		bool isDead();
		void loadClips(SDL_Rect clips[]);

	private:
		int posX, posY;
		bool dead;
		int frameCounter;

		SDL_Rect *explosionClips;
};

Explosions::Explosions(int x, int y){
	posX = x;
	posY = y;
	dead = false;
	frameCounter = 0;
}

void Explosions::loadClips(SDL_Rect clips[]){
	explosionClips = clips;
}

void Explosions::increment(){
	if(frameCounter > 24){
		dead = true;
	}
	++frameCounter;
}

bool Explosions::isDead(){
	return dead;
}

void Explosions::render(LTexture* explosionSheet){
	SDL_Rect currentClip = explosionClips[ frameCounter / 5 ];
	explosionSheet->render(posX, posY, &currentClip);
}
