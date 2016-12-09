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
	int maxIterator;

	SDL_Rect *beamClips;

	Lasers(int x, int y) : mobileObject(117.2, 164){
		posX = x;
		posY = y;
		maxIterator = 0;
		
		collider.resize(1);

		//Initialize the collision boxes' width and height
	    collider[ 0 ].w = 117.2;
	    collider[ 0 ].h = 164;

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
		int beamflip = 90;
		int frameHolder = frameCounter / 9;
		int frameNum = 0;
		if(frameCounter > 72){
			if(frameCounter % 2 == 0){
				frameNum = 8;
				++maxIterator;
			}else{
				frameNum = 7;
			}
		}else{
			frameNum = frameHolder;
		}
		if(maxIterator > 20){
			dead = true;	
		}
		SDL_Rect currentClip = beamClips[ frameNum ];
		beamSheet->render(x, y, &currentClip, beamflip);
	}

};
