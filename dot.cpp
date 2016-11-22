#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "settings.cpp"
#include "lTexture.cpp"
#include "weapons.cpp"
#include "fighters.cpp"
#include "tile.cpp"

class Dot{
	public: 
		static const int DOT_WIDTH = 60;
		static const int DOT_HEIGHT = 34;

		Dot(int x, int y);

		void handleEvent(SDL_Event& e);
		void move(std::vector<Fighters*> trumps, Tile *tiles[]);
		void render(LTexture* dotTexture, SDL_Rect& camera);
		void fireShot(int x, int y);
		void loadBullets(std::vector<Weapons*>* bullets);
		void setCamera(SDL_Rect& camera);

		int getPosX();
		int getPosY();

		//gets the collision boxes of the dots. 
		std::vector<SDL_Rect>& getColliders();

	private:
		int posX, posY;
		int velX, velY;
		double angle;

		//collisions boxes for the dots
		std::vector<SDL_Rect> userJet;
		std::vector<Weapons*>* bulletArr;

		//moves the collision boxes of the dots with dots
		void shiftColliders();
};

bool checkCollision(std::vector<SDL_Rect>& a, std::vector<Fighters*> trumps);
bool checkTileCollision(std::vector<SDL_Rect>& a, SDL_Rect tile);

bool touchesWalls(std::vector<SDL_Rect>& box, Tile* tiles[]){
	for(int i = 0; i < totalTiles; ++i){
		if((tiles[i]->getType() >= tileCenter) && (tiles[i]->getType() <= tileTopLeft)){
			if(checkTileCollision(box, tiles[i]->getBox())){
				return true;
			}
		}
	}
	return false;
}

void Dot::setCamera(SDL_Rect& camera){
	camera.x = (userJet[0].x + DOT_WIDTH/2) - SCREEN_WIDTH/2;
	camera.y = (userJet[0].y + DOT_HEIGHT/2) - SCREEN_HEIGHT/2;

	if(camera.x < 0){
		camera.x = 0;
	}
	if(camera.y < 0){
		camera.y = 0;
	}
	if(camera.x > LEVEL_WIDTH - camera.w){
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if(camera.y > LEVEL_HEIGHT - camera.h){
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

Dot::Dot(int x, int y){
	posX = x;
	posY = y;

	userJet.resize(11);

	velX = 0;
	velY = 0;

	//Initialize the collision boxes' width and height
    userJet[ 0 ].w = 6;
    userJet[ 0 ].h = 1;

    userJet[ 1 ].w = 10;
    userJet[ 1 ].h = 1;

    userJet[ 2 ].w = 14;
    userJet[ 2 ].h = 1;

    userJet[ 3 ].w = 16;
    userJet[ 3 ].h = 2;

    userJet[ 4 ].w = 18;
    userJet[ 4 ].h = 2;

    userJet[ 5 ].w = 20;
    userJet[ 5 ].h = 6;

    userJet[ 6 ].w = 18;
    userJet[ 6 ].h = 2;

    userJet[ 7 ].w = 16;
    userJet[ 7 ].h = 2;

    userJet[ 8 ].w = 14;
    userJet[ 8 ].h = 1;

    userJet[ 9 ].w = 10;
    userJet[ 9 ].h = 1;

    userJet[ 10 ].w = 6;
    userJet[ 10 ].h = 1;

	shiftColliders();
}

void Dot::loadBullets(std::vector<Weapons*>* bullet){
	bulletArr = bullet;
}

void Dot::handleEvent(SDL_Event& e){
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_UP:
				velY -= DOT_VELOCITY;
				angle = -30.0;
				break;
			case SDLK_DOWN:
				velY += DOT_VELOCITY;
				angle = 30.0;
				break;
			case SDLK_LEFT:
				velX -= DOT_VELOCITY;
				angle = 0;
				break;
			case SDLK_RIGHT:
				velX += DOT_VELOCITY;
				angle = 0;
				break;
			case SDLK_SPACE:
				fireShot(getPosX(), getPosY());
				break;
		}
	}

	if(e.type == SDL_KEYUP && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_UP:
				velY += DOT_VELOCITY;
				angle = 0;
				break;
			case SDLK_DOWN:
				velY -= DOT_VELOCITY;
				angle = 0;
				break;
			case SDLK_LEFT:
				velX += DOT_VELOCITY;
				angle = 0;
				break;
			case SDLK_RIGHT:
				velX -= DOT_VELOCITY;
				angle = 0;
				break;
			case SDLK_SPACE:
				fireShot(getPosX(), getPosY());
				break;
		}
	}

	if((e.type == SDL_KEYUP || e.type == SDL_KEYDOWN) && e.key.repeat != 0){
		switch(e.key.keysym.sym){
			case SDLK_SPACE:
				fireShot(getPosX(), getPosY());
				break;
		}
	}

}

void Dot::fireShot(int x, int y){
	Weapons* bullet = new Weapons(x + 60, y + 15);
	bulletArr->push_back(bullet);
}

void Dot::move(std::vector<Fighters*> trumps, Tile *tiles[]){
	//dot goes left or right
	posX += velX;
	shiftColliders();
	if((posX < 0) || (posX + DOT_WIDTH > LEVEL_WIDTH) || checkCollision(userJet, trumps) || touchesWalls(userJet, tiles)){
		posX -= velX;
		shiftColliders();
	}
	//dot goes up or down
	posY +=velY;
	shiftColliders();
	if((posY < 0) || (posY + DOT_HEIGHT > LEVEL_HEIGHT) || checkCollision(userJet, trumps) || touchesWalls(userJet, tiles)){
		posY -= velY;
		shiftColliders();
	}
}

bool checkTileCollision(std::vector<SDL_Rect>& thisObject, SDL_Rect tile){
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

		leftW = tile.x;
	    rightW = tile.x + tile.w;
	    topW = tile.y;
	    bottomW = tile.y + tile.h;

		if(((bottomA <= topW) || (topA >= bottomW) || (rightA <= leftW) || (leftA >=rightW)) == false){
			//collision is detected
			return true;
		}
	}

    return false;
}

bool checkCollision(std::vector<SDL_Rect>& thisObject, std::vector<Fighters*> trumps){
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

		for(int Wbox = 0; Wbox < trumps.size(); Wbox++){
			std::vector<SDL_Rect> temp = trumps[Wbox]->trumpHead;
			for(int Zbox = 0; Zbox < temp.size(); Zbox++){
				leftW = temp[Zbox].x;
			    rightW = temp[Zbox].x + temp[Zbox].w;
			    topW = temp[Zbox].y;
			    bottomW = temp[Zbox].y + temp[Zbox].h;

				if(((bottomA <= topW) || (topA >= bottomW) || (rightA <= leftW) || (leftA >=rightW)) == false){
					//collision is detected
					return true;
				}
			}
		}
	}

    return false;
}

void Dot::shiftColliders(){
	int r = 0;

	for(int set = 0; set < userJet.size(); ++set){
		userJet[set].x = posX + (DOT_WIDTH - userJet[set].w) /2;;
		userJet[set].y = posY + r;
		r += userJet[set].h;
	}
}

std::vector<SDL_Rect>& Dot::getColliders(){
	return userJet;
}

int Dot::getPosX(){
	return posX;
}

int Dot::getPosY(){
	return posY;
}

void Dot::render(LTexture* dotTexture, SDL_Rect& camera){
	dotTexture->render(posX - camera.x, posY - camera.y, NULL, angle);
}
