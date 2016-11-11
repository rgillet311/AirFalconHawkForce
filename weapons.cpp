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

struct Circle{
	int x, y;
	int r;
};

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
		Circle getCollider();

	private:
		int posX, posY;
		bool dead;

		Circle collider;
};

Weapons::Weapons(int x, int y){
	posX = x;
	posY = y;
	dead = false;
	collider.r = bulletWidth / 2;
}

Circle Weapons::getCollider(){
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
