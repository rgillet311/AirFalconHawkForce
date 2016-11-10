#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

#include </home/parallels/Desktop/c++/testProj/settings.cpp>
#include </home/parallels/Desktop/c++/testProj/lTexture.cpp>

class Weapons{
	public: 
		static const int bulletWidth = 5;
		static const int bulletHeight = 5;

		Weapons(int x, int y);

		void render(LTexture* bulletTexture);
		void increment();
		int getPosX();
		int getPosY();


	private:
		int posX, posY;
};

Weapons::Weapons(int x, int y){
	posX = x;
	posY = y;
}

void Weapons::increment(){
	posX += BULLET_VELOCITY;
	printf("increment by %d = %d \n", BULLET_VELOCITY, posX);
}

int Weapons::getPosX(){
	return posX;
}

int Weapons::getPosY(){
	return posY;
}

void Weapons::render(LTexture* bulletTexture){
	printf("render %d \n", posX);
	bulletTexture->render(posX, posY);
}
