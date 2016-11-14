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
		static const int trumpWidth = 40;
		static const int trumpHeight = 40;

		Fighters(int x, int y, int trump_speed, LTexture * trumpPhoto);

		void render();
		bool increment(std::vector<Weapons*>* bullets);
		int getPosX();
		int getPosY();
		void setIsDead(bool dead);
		bool isDead();

		//gets the collision boxes of the dots. 
		std::vector<SDL_Rect>& getColliders();
		//collisions boxes for the trumps
		std::vector<SDL_Rect> trumpHead;

	private:
		int posX, posY;
		bool dead;
		int velocity;
		LTexture * trumpTexture;

		//moves the collision boxes of the dots with dots
		void shiftColliders();
};

double distanceSquared(int x1, int y1, int x2, int y2);

bool checkCollision(std::vector<SDL_Rect>& thisObject, std::vector<Weapons*>* bullets);

Fighters::Fighters(int x, int y, int trump_speed, LTexture * trumpPhoto){
	posX = x;
	posY = y;
	dead = false;
	velocity = TRUMP_VELOCITY[trump_speed];
	trumpTexture = trumpPhoto;

	trumpHead.resize(11);

	//Initialize the collision boxes' width and height
    trumpHead[ 0 ].w = 10;
    trumpHead[ 0 ].h = 2;

    trumpHead[ 1 ].w = 16;
    trumpHead[ 1 ].h = 2;

    trumpHead[ 2 ].w = 22;
    trumpHead[ 2 ].h = 4;

    trumpHead[ 3 ].w = 28;
    trumpHead[ 3 ].h = 4;

    trumpHead[ 4 ].w = 34;
    trumpHead[ 4 ].h = 6;

    trumpHead[ 5 ].w = 40;
    trumpHead[ 5 ].h = 8;

    trumpHead[ 6 ].w = 34;
    trumpHead[ 6 ].h = 6;

    trumpHead[ 7 ].w = 28;
    trumpHead[ 7 ].h = 4;

    trumpHead[ 8 ].w = 22;
    trumpHead[ 8 ].h = 4;

    trumpHead[ 9 ].w = 16;
    trumpHead[ 9 ].h = 2;

    trumpHead[ 10 ].w = 10;
    trumpHead[ 10 ].h = 2;

    shiftColliders();

}

double distanceSquared(int x1, int y1, int x2, int y2){
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

bool checkCollision(std::vector<SDL_Rect>& thisObject, std::vector<Weapons*>* bullets){
	//The sides of the rectangles
    int leftA, leftB, leftW;
    int rightA, rightB, rightW;
    int topA, topB, topW;
    int bottomA, bottomB, bottomW;

    for(int Abox = 0; Abox < thisObject.size(); Abox++){
	    //Calculate the sides of rect A
	    leftA = thisObject[Abox].x;
	    rightA = thisObject[Abox].x + thisObject[Abox].w;
	    topA = thisObject[Abox].y;
	    bottomA = thisObject[Abox].y + thisObject[Abox].h;

		for(int Bbox = 0; Bbox < bullets->size(); Bbox++){
			Weapons* bullet = bullets->at(Bbox);
			Circle a = bullet->getCollider();
			leftW = a.x - a.r;
		    rightW = a.x + a.r;
		    topW = a.y + a.r;
		    bottomW = a.x - a.r;

			if(((bottomA <= topW) || (topA >= bottomW) || (rightA <= leftW) || (leftA >=rightW)) == false){
				//collision is detected
				return true;
			}

			/**
			int cx, cy;
			if(a.x < leftA){
				cx = leftA;
			}else if(a.x > rightA){
				cx = rightA;
			}else{
				cx = a.x;
			}

			if(a.y < topA){
				cy = topA;
			}else if(a.y > bottomA){
				cy = bottomA;
			}else{
				cy = a.y;
			}

			if(distanceSquared(a.x, a.y, cx, cy) < a.r * a.r){
				printf("HITTED");
				return true;
			}
			**/
		}
	}

    return false;
}

void Fighters::shiftColliders(){
	int r = 0;

	for(int set = 0; set < trumpHead.size(); ++set){
		trumpHead[set].x = posX + (trumpWidth - trumpHead[set].w) / 2;
		trumpHead[set].y = posY + r;
		r += trumpHead[set].h;
	}
}

std::vector<SDL_Rect>& Fighters::getColliders(){
	return trumpHead;
}

bool Fighters::increment(std::vector<Weapons*>* bullets){
	//dot goes left or right
	posX -= velocity;
	shiftColliders();
	if((posX < 0) || checkCollision(trumpHead, bullets)){
		posX += velocity;
		shiftColliders();
		return true;
	}
	return false;
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
