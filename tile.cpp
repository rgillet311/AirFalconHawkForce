#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>

class Tile{
public:
	//init vars
	Tile(int x, int y, int tileType);
	//deallocates memory
	void render(SDL_Rect& camera, LTexture* tileTexture, SDL_Rect& tileBox);
	int getType();
	SDL_Rect getBox();

private:
	SDL_Rect box;
	int myType;
};

Tile::Tile(int x, int y, int tileType){
	box.x = x;
	box.y = y;

	box.w = tileHeight;
	box.h = tileHeight;

	myType = tileType;
}

int Tile::getType(){
	return myType;
}

SDL_Rect Tile::getBox(){
	return box;
}

void Tile::render(SDL_Rect& camera, LTexture* tileTexture, SDL_Rect& tileBox){
	tileTexture->render(box.x - camera.x, box.y - camera.y, &tileBox);
}