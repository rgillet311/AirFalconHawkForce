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
#include "mobileObject.cpp"

class Dot : public mobileObject{
	private:
		double angle = 0;
		int exhaustDelta = 0;
		double exhaustAngle = -90;
		int exhaustCounter;

		std::vector<Weapons*>* bulletArr;
		SDL_Rect *exhaustClips;

	public: 
		Dot(int x, int y) : mobileObject(60, 34){
			posX = x;
			posY = y;

			exhaustCounter = 0;

			collider.resize(11);

			velX = 0;
			velY = 0;

			//Initialize the collision boxes' width and height
		    collider[ 0 ].w = 6;
		    collider[ 0 ].h = 1;

		    collider[ 1 ].w = 10;
		    collider[ 1 ].h = 1;

		    collider[ 2 ].w = 14;
		    collider[ 2 ].h = 1;

		    collider[ 3 ].w = 16;
		    collider[ 3 ].h = 2;

		    collider[ 4 ].w = 18;
		    collider[ 4 ].h = 2;

		    collider[ 5 ].w = 20;
		    collider[ 5 ].h = 6;

		    collider[ 6 ].w = 18;
		    collider[ 6 ].h = 2;

		    collider[ 7 ].w = 16;
		    collider[ 7 ].h = 2;

		    collider[ 8 ].w = 14;
		    collider[ 8 ].h = 1;

		    collider[ 9 ].w = 10;
		    collider[ 9 ].h = 1;

		    collider[ 10 ].w = 6;
		    collider[ 10 ].h = 1;

			shiftColliders();
		}

		void loadBullets(std::vector<Weapons*>* bullet){
			bulletArr = bullet;
		}

		void handleEvent(SDL_Event& e){
			if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
				switch(e.key.keysym.sym){
					case SDLK_UP:
						velY -= playerVelocity;
						angle = -30.0;
						exhaustAngle -= 30.0;
						exhaustDelta = 20;
						break;
					case SDLK_DOWN:
						velY += playerVelocity;
						angle = 30.0;
						exhaustAngle += 30;
						exhaustDelta = -20;
						break;
					case SDLK_LEFT:
						velX -= playerVelocity;
						angle = 0;
						exhaustAngle = -90;
						exhaustDelta = 0;
						break;
					case SDLK_RIGHT:
						velX += playerVelocity;
						angle = 0;
						exhaustAngle = -90;
						exhaustDelta = 0;
						break;
					case SDLK_SPACE:
						fireShot(getPosX(), getPosY());
						break;
				}
			}

			if(e.type == SDL_KEYUP && e.key.repeat == 0){
				switch(e.key.keysym.sym){
					case SDLK_UP:
						velY += playerVelocity;
						angle = 0;
						exhaustAngle = -90;
						exhaustDelta = 0;
						break;
					case SDLK_DOWN:
						velY -= playerVelocity;
						angle = 0;
						exhaustAngle = -90;
						exhaustDelta = 0;
						break;
					case SDLK_LEFT:
						velX += playerVelocity;
						angle = 0;
						exhaustAngle = -90;
						exhaustDelta = 0;
						break;
					case SDLK_RIGHT:
						velX -= playerVelocity;
						angle = 0;
						exhaustAngle = -90;
						exhaustDelta = 0;
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

		void fireShot(int x, int y){
			Weapons* bullet = new Weapons(x + 60, y + 15);
			bulletArr->push_back(bullet);
		}

		void move(std::vector<Fighters*> trumps, Tile *tiles[]){
			//dot goes left or right
			posX += velX;
			shiftColliders();
			if((posX < 0) || (posX + width > LEVEL_WIDTH) || checkCollision(collider, trumps) || touchesWalls(collider, tiles)){
				posX -= velX;
				shiftColliders();
			}
			//dot goes up or down
			posY +=velY;
			shiftColliders();
			if((posY < 0) || (posY + height > LEVEL_HEIGHT) || checkCollision(collider, trumps) || touchesWalls(collider, tiles)){
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
					std::vector<SDL_Rect> temp = trumps[Wbox]->collider;
					for(int Zbox = 0; Zbox < temp.size(); Zbox++){
						leftW = temp[Zbox].x;
					    rightW = temp[Zbox].x + temp[Zbox].w;
					    topW = temp[Zbox].y;
					    bottomW = temp[Zbox].y + temp[Zbox].h;

						if(((bottomA <= topW) || (topA >= bottomW) || (rightA <= leftW) || (leftA >=rightW)) == false){
							//collision is detected
							setIsDead(true, true);
							return true;
						}
					}
				}
			}

		    return false;
		}

		void loadClips(SDL_Rect clips[]){
			exhaustClips = clips;
		}

		void render(LTexture* dotTexture, LTexture* exhaustSheet, SDL_Rect& camera){
			++exhaustCounter;
			if(exhaustCounter / 5 > 10){
				exhaustCounter = 0;
			}
			SDL_Rect currentClip = exhaustClips[ exhaustCounter / 10 ];
			exhaustSheet->render(posX - camera.x - 25, posY - camera.y + (exhaustDelta), &currentClip, exhaustAngle);

			dotTexture->render(posX - camera.x, posY - camera.y, NULL, angle);
		}
		void setCamera(SDL_Rect& camera){
			camera.x = (collider[0].x + width/2) - SCREEN_WIDTH/2;
			camera.y = (collider[0].y + height/2) - SCREEN_HEIGHT/2;

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

			camera.x = 0;
			camera.y = 0;
		}

		bool touchesWalls(std::vector<SDL_Rect>& box, Tile* tiles[]){
			if(tiles){
				for(int i = 0; i < totalTiles; ++i){
					if((tiles[i]->getType() >= tileCenter) && (tiles[i]->getType() <= tileTopLeft)){
						if(checkTileCollision(box, tiles[i]->getBox())){
							return true;
						}
					}
				}
			}
			return false;
		}
};
