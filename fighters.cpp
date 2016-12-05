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

class Fighters: public mobileObject{
	private:
		bool wall;
		LTexture * image;

	public: 
		Fighters(int x, int y, int speed, LTexture * texture) : mobileObject(40, 40){
			posX = x;
			posY = y;
			wall = false;
			velX = fighterVelocity[speed];
			image = texture;

			collider.resize(11);

			//Initialize the collision boxes' width and height
		    collider[ 0 ].w = 10;
		    collider[ 0 ].h = 2;

		    collider[ 1 ].w = 16;
		    collider[ 1 ].h = 2;

		    collider[ 2 ].w = 22;
		    collider[ 2 ].h = 4;

		    collider[ 3 ].w = 28;
		    collider[ 3 ].h = 4;

		    collider[ 4 ].w = 34;
		    collider[ 4 ].h = 6;

		    collider[ 5 ].w = 40;
		    collider[ 5 ].h = 8;

		    collider[ 6 ].w = 34;
		    collider[ 6 ].h = 6;

		    collider[ 7 ].w = 28;
		    collider[ 7 ].h = 4;

		    collider[ 8 ].w = 22;
		    collider[ 8 ].h = 4;

		    collider[ 9 ].w = 16;
		    collider[ 9 ].h = 2;

		    collider[ 10 ].w = 10;
		    collider[ 10 ].h = 2;

		    shiftColliders();

		}

		bool increment(std::vector<Bullets*>* bullets){
			//dot goes left or right
			posX -= velX;
			shiftColliders();
			if((posX < 0) || checkCollision(collider, bullets)){
				posX += velX;
				shiftColliders();
				return true;
			}
			return false;
		}

		void setIsWall(bool walls){
			wall = walls;
		}

		bool isWall(){
			return wall;
		}

		void render(){
			image->render(posX, posY);
		}

		double distanceSquared(int x1, int y1, int x2, int y2){
			int deltaX = x2 - x1;
			int deltaY = y2 - y1;
			return deltaX*deltaX + deltaY*deltaY;
		}

		bool checkCollision(std::vector<SDL_Rect>& thisObject, std::vector<Bullets*>* bullets){
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
					Bullets* bullet = bullets->at(Bbox);
					std::vector<SDL_Rect> temp = bullet->getColliders();
					for(int Zbox = 0; Zbox < temp.size(); Zbox++){
						leftW = temp[Zbox].x;
					    rightW = temp[Zbox].x + temp[Zbox].w;
					    topW = temp[Zbox].y;
					    bottomW = temp[Zbox].y + temp[Zbox].h;

						if(((bottomA <= topW) || (topA >= bottomW) || (rightA <= leftW) || (leftA >=rightW)) == false){
							//collision is detected
							bullet->setIsDead(true, false);
							return true;
						}
					}
				}
			}

		    return false;
		}


};
