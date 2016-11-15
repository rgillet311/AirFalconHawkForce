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

class Weapons{
	public: 
		static const int bulletWidth = 5;
		static const int bulletHeight = 5;

		Weapons(int x, int y);

		void render(LTexture* bulletTexture);
		void increment();
		int getPosX();
		int getPosY();
		void setIsDead(bool dead);
		bool isDead();
		std::vector<SDL_Rect>& getColliders();

	private:
		int posX, posY;
		bool dead;

		std::vector<SDL_Rect> collider;
		//moves the collision boxes of the dots with dots
		void shiftColliders();
};

Weapons::Weapons(int x, int y){
	posX = x;
	posY = y;
	dead = false;
	
	collider.resize(11);

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

void Weapons::shiftColliders(){
	int r = 0;

	for(int set = 0; set < collider.size(); ++set){
		collider[set].x = posX + (bulletWidth - collider[set].w) /2;
		collider[set].y = posY + r;
		r += collider[set].h;
	}
}

std::vector<SDL_Rect>& Weapons::getColliders(){
	return collider;
}

void Weapons::increment(){
	posX += BULLET_VELOCITY;
}

int Weapons::getPosX(){
	return posX;
}

int Weapons::getPosY(){
	return posY;
}

void Weapons::setIsDead(bool death){
	dead = death;
}

bool Weapons::isDead(){
	return dead;
}

void Weapons::render(LTexture* bulletTexture){
	bulletTexture->render(posX, posY);
}
