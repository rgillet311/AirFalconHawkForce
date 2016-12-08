#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

class Lasers{
public:
	bool dead;
	int frameCounter;

	SDL_Rect *beamClips;

	Lasers(){
		dead = false;
		frameCounter = 0;
	}

	void loadBeamClips(SDL_Rect clips[]){
		beamClips = clips;
	}

	void increment(){
		if(frameCounter > 6){
			dead = true;
		}
		++frameCounter;
	}

	bool isDead(){
		return dead;
	}

	void render(LTexture* beamSheet, int x, int y){
		SDL_Rect currentClip = beamClips[ frameCounter % 6 ];
		beamSheet->render(x, y, &currentClip);
	}

};
