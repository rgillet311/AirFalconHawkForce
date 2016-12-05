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

class mobileObject {
	public:
		int velX, velY;
		int posX, posY;
		int width, height;
		bool dead = false;
		bool explode = false;
		std::vector<SDL_Rect> collider;

		mobileObject(int w, int h){
			width = w;
			height = h;
		}

		void render(LTexture* objTexture){
			objTexture->render(posX, posY);
		}
		int getPosX(){
			return posX;
		}

		int getPosY(){
			return posY;
		}
		void setIsDead(bool death, bool exploded){
			dead = death;
			explode = exploded;
		}

		bool exploded(){
			return explode;
		}

		bool isDead(){
			return dead;
		}
		std::vector<SDL_Rect>& getColliders(){
			return collider;
		}
		void shiftColliders(){
			int r = 0;

			for(int set = 0; set < collider.size(); ++set){
				collider[set].x = posX + (width - collider[set].w) /2;
				collider[set].y = posY + r;
				r += collider[set].h;
			}
		}
};