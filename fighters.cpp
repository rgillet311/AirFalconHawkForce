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

class Fighters{
	public: 
		static const int bulletWidth = 5;
		static const int bulletHeight = 5;

		Fighters(int x, int y, int trump_speed, LTexture * trumpPhoto);

		void render();
		void increment();
		int getPosX();
		int getPosY();
		void setIsDead(bool dead);
		bool isDead();

	private:
		int posX, posY;
		bool dead;
		int velocity;
		LTexture * trumpTexture;
};

Fighters::Fighters(int x, int y, int trump_speed, LTexture * trumpPhoto){
	posX = x;
	posY = y;
	dead = false;
	velocity = TRUMP_VELOCITY[trump_speed];
	trumpTexture = trumpPhoto;
}

void Fighters::increment(){
	posX -= velocity;
}

int Fighters::getPosX(){
	return posX;
}

int Fighters::getPosY(){
	return posY;
}

void Fighters::setIsDead(bool death){
	dead = death;
}

bool Fighters::isDead(){
	return dead;
}

void Fighters::render(){
	trumpTexture->render(posX, posY);
}
