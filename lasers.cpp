#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

class Lasers: public mobileObject{
public:
	int frameCounter;

	SDL_Rect *beamClips;

	Lasers(int x, int y) : mobileObject(90, 13){
		posX = x;
		posY = y;
		
		collider.resize(1);

		//Initialize the collision boxes' width and height
	    collider[ 0 ].w = 90;
	    collider[ 0 ].h = 13;

		frameCounter = 0;

		shiftColliders();
	}

	void loadBeamClips(SDL_Rect clips[]){
		beamClips = clips;
	}

	void increment(){
		++frameCounter;
		shiftColliders();
	}

	void render(LTexture* beamSheet, int x, int y){
		posX = x;
		posY = y;
		int beamflip = 180;
		SDL_Rect currentClip = beamClips[ frameCounter % 6 ];
		beamSheet->render(x, y, &currentClip, beamflip);
	}

};
