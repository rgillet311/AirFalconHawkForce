#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

#include </home/parallels/Desktop/c++/testProj/settings.cpp>
#include </home/parallels/Desktop/c++/testProj/lTexture.cpp>
#include </home/parallels/Desktop/c++/testProj/weapons.cpp>

class Dot{
	public: 
		static const int DOT_WIDTH = 60;
		static const int DOT_HEIGHT = 34;

		Dot(int x, int y);

		void handleEvent(SDL_Event& e);
		void move(std::vector<SDL_Rect>& bullets, std::vector<SDL_Rect>& jets);
		void render(LTexture* dotTexture);
		void fireShot(int x, int y);
		void loadBullets(std::vector<Weapons*>* bullets);
		
		int getPosX();
		int getPosY();

		//gets the collision boxes of the dots. 
		std::vector<SDL_Rect>& getColliders();

	private:
		int posX, posY;
		int velX, velY;

		//collisions boxes for the dots
		std::vector<SDL_Rect> userJet;
		std::vector<Weapons*>* bulletArr;

		//moves the collision boxes of the dots with dots
		void shiftColliders();
};

bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b, std::vector<SDL_Rect>& jets);

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
				break;
			case SDLK_DOWN:
				velY += DOT_VELOCITY;
				break;
			case SDLK_LEFT:
				velX -= DOT_VELOCITY;
				break;
			case SDLK_RIGHT:
				velX += DOT_VELOCITY;
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
				break;
			case SDLK_DOWN:
				velY -= DOT_VELOCITY;
				break;
			case SDLK_LEFT:
				velX += DOT_VELOCITY;
				break;
			case SDLK_RIGHT:
				velX -= DOT_VELOCITY;
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
	Weapons* bullet = new Weapons(x, y);
	printf("added bullet X-%d, Y-%d \n", (x, y));
	bulletArr->push_back(bullet);
	printf("bulletArr size %d \n", bulletArr->size());
}

void Dot::move(std::vector<SDL_Rect>& bullets, std::vector<SDL_Rect>& jets){
	//dot goes left or right
	posX += velX;
	shiftColliders();
	if((posX < 0) || (posX + DOT_WIDTH > LEVEL_WIDTH) || checkCollision(userJet, bullets, jets)){
		posX -= velX;
		shiftColliders();
	}
	//dot goes up or down
	posY +=velY;
	shiftColliders();
	if((posY < 0) || (posY + DOT_HEIGHT > LEVEL_HEIGHT) || checkCollision(userJet, bullets, jets)){
		posY -= velY;
		shiftColliders();
	}
}

bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b, std::vector<SDL_Rect>& jets){
	//The sides of the rectangles
    int leftA, leftB, leftW;
    int rightA, rightB, rightW;
    int topA, topB, topW;
    int bottomA, bottomB, bottomW;

    for(int Abox = 0; Abox < a.size(); Abox++){
	    //Calculate the sides of rect A
	    leftA = a[Abox].x;
	    rightA = a[Abox].x + a[Abox].w;
	    topA = a[Abox].y;
	    bottomA = a[Abox].y + a[Abox].h;

		for(int Bbox = 0; Bbox < b.size(); Bbox++){
		    //Calculate the sides of rect B
		    leftB = b[Bbox].x;
		    rightB = b[Bbox].x + b[Bbox].w;
		    topB = b[Bbox].y;
		    bottomB = b[Bbox].y + b[Bbox].h;
			
			if(((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >=rightB)) == false){
				//collision is detected
				return true;
			}
		}

		for(int Wbox = 0; Wbox < jets.size(); Wbox++){
			leftW = jets[Wbox].x;
		    rightW = jets[Wbox].x + jets[Wbox].w;
		    topW = jets[Wbox].y;
		    bottomW = jets[Wbox].y + jets[Wbox].h;

			if(((bottomA <= topW) || (topA >= bottomW) || (rightA <= leftW) || (leftA >=rightW)) == false){
				//collision is detected
				return true;
			}
		}
	}

    return false;
}

void Dot::shiftColliders(){
	int r = 0;

	for(int set = 0; set < userJet.size(); ++set){
		userJet[set].x = posX + (DOT_WIDTH - userJet[set].w) /2;
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

void Dot::render(LTexture* dotTexture){
	dotTexture->render(posX, posY);
}
