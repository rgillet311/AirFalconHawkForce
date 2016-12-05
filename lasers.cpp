#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "settings.cpp"
#include "lTexture.cpp"
#include "mobileObject.cpp"

class Bullets: public mobileObject{
	public: 
		Bullets(int x, int y) : mobileObject(5, 5){
			posX = x;
			posY = y;
			
			collider.resize(9);

			//Initialize the collision boxes' width and height
		    collider[ 0 ].w = 1;
		    collider[ 0 ].h = 1;

		    collider[ 1 ].w = 2;
		    collider[ 1 ].h = 1;

		    collider[ 2 ].w = 3;
		    collider[ 2 ].h = 1;

		    collider[ 3 ].w = 4;
		    collider[ 3 ].h = 1;

		    collider[ 4 ].w = 5;
		    collider[ 4 ].h = 1;

		    collider[ 5 ].w = 4;
		    collider[ 5 ].h = 1;

		    collider[ 6 ].w = 3;
		    collider[ 6 ].h = 1;

		    collider[ 7 ].w = 2;
		    collider[ 7 ].h = 1;

		    collider[ 8 ].w = 1;
		    collider[ 8 ].h = 1;

			shiftColliders();
		}

		void increment(){
			posX += bulletVelocity;
			shiftColliders();
		}

};
