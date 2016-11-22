/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include "settings.cpp"

#include "lTexture.cpp"
#include "dot.cpp"
#include "lTimer.cpp"
#include "weapons.cpp"
#include "fighters.cpp"
#include "explosion.cpp"
#include "tile.cpp"

bool init();
bool loadMedia(Tile* tiles[]);
void close(Tile* tiles[]);

//bool checkTileCollision(SDL_Rect a, SDL_Rect b);
//bool touchesWalls(SDL_Rect box, Tile* tiles[]);
bool setTiles(Tile* tiles[]);

//The window we'll be rendering to
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
LTexture dotTexture;
LTexture bgTexture;
LTexture bulletTexture;
LTexture trump1Texture;
LTexture trump2Texture;
LTexture trump3Texture;

const int explosionFrames = 25;
SDL_Rect explosionClips[explosionFrames];
LTexture explosionSheet;
int score = 0;

SDL_Rect tileClips[totalTileSprites];
LTexture tileTexture;
LTexture textTexture;
TTF_Font *font = NULL;

bool init(){
	bool success = true;

	if(TTF_Init() == -1){
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not nitialize! SDL_Errpr: %s\n", SDL_GetError() );
		success = false;
	}else{
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}else{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(renderer == NULL){
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			dotTexture.loadRenderer(renderer);
			bgTexture.loadRenderer(renderer);
			bulletTexture.loadRenderer(renderer);
			trump1Texture.loadRenderer(renderer);
			trump2Texture.loadRenderer(renderer);
			trump3Texture.loadRenderer(renderer);
			explosionSheet.loadRenderer(renderer);
			textTexture.loadRenderer(renderer);
			tileTexture.loadRenderer(renderer);
		}
	}
	return success;
}

bool changeScore(int score, SDL_Color textColor){
	bool success = true;
	//std::string str = "Score: %d", score;
	char strScore[50]; 
	sprintf(strScore,"Score: %d", score); 
	if(!textTexture.loadFromRenderedText(strScore, textColor, font)){
		printf("Failed to render text texture! \n");
		success = false;
	}
	return success;
}

bool setTiles(Tile* tiles[]){
	bool tilesLoaded = true;

	int x = 0, y = 0;
	std::fstream map("maps/lazy.map");

	//depending on the compiles, might have to check if !map.is_open();
	if(map==NULL){
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}else{
		for(int i = 0; i < totalTiles; ++i){
			//to determine what kind of tiletype will be used. 
			int tileType = -1;

			//read the map from the tileType
			map >> tileType;

			if(map.fail()){
				printf("Error loading the map: unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}
			//if the numebr is a valid tile number else we dont recognize it
			if((tileType >= 0) && (tileType < totalTileSprites)){
				tiles[i] = new Tile(x, y, tileType);
			}else{
				//Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
			}

			x+= tileWidth;

			//if we have gone too far
			if(x >= LEVEL_WIDTH){
				//move back
				x = 0;
				//and move to the next row
				y+= tileHeight;
			}
		}

		if(tilesLoaded){
			tileClips[ tileRed ].x = 0;
            tileClips[ tileRed ].y = 0;
            tileClips[ tileRed ].w = tileWidth;
            tileClips[ tileRed ].h = tileHeight;

            tileClips[ tileGreen ].x = 0;
            tileClips[ tileGreen ].y = 80;
            tileClips[ tileGreen ].w = tileWidth;
            tileClips[ tileGreen ].h = tileHeight;

            tileClips[ tileBlue ].x = 0;
            tileClips[ tileBlue ].y = 160;
            tileClips[ tileBlue ].w = tileWidth;
            tileClips[ tileBlue ].h = tileHeight;

            tileClips[ tileTopLeft ].x = 80;
            tileClips[ tileTopLeft ].y = 0;
            tileClips[ tileTopLeft ].w = tileWidth;
            tileClips[ tileTopLeft ].h = tileHeight;

            tileClips[ tileLeft ].x = 80;
            tileClips[ tileLeft ].y = 80;
            tileClips[ tileLeft ].w = tileWidth;
            tileClips[ tileLeft ].h = tileHeight;

            tileClips[ tileBottomLeft ].x = 80;
            tileClips[ tileBottomLeft ].y = 160;
            tileClips[ tileBottomLeft ].w = tileWidth;
            tileClips[ tileBottomLeft ].h = tileHeight;

            tileClips[ tileTop ].x = 160;
            tileClips[ tileTop ].y = 0;
            tileClips[ tileTop ].w = tileWidth;
            tileClips[ tileTop ].h = tileHeight;

            tileClips[ tileCenter ].x = 160;
            tileClips[ tileCenter ].y = 80;
            tileClips[ tileCenter ].w = tileWidth;
            tileClips[ tileCenter ].h = tileHeight;

            tileClips[ tileBottom ].x = 160;
            tileClips[ tileBottom ].y = 160;
            tileClips[ tileBottom ].w = tileWidth;
            tileClips[ tileBottom ].h = tileHeight;

            tileClips[ tileTopRight ].x = 240;
            tileClips[ tileTopRight ].y = 0;
            tileClips[ tileTopRight ].w = tileWidth;
            tileClips[ tileTopRight ].h = tileHeight;

            tileClips[ tileRight ].x = 240;
            tileClips[ tileRight ].y = 80;
            tileClips[ tileRight ].w = tileWidth;
            tileClips[ tileRight ].h = tileHeight;

            tileClips[ tileBottomRight ].x = 240;
            tileClips[ tileBottomRight ].y = 160;
            tileClips[ tileBottomRight ].w = tileWidth;
            tileClips[ tileBottomRight ].h = tileHeight;
		}
	}

	map.close();
	return tilesLoaded;
}

bool loadMedia(Tile* tiles[]){
	bool success = true;

	font = TTF_OpenFont("fonts/lazy.ttf", 28);
	if(font == NULL){
		printf("Failed to open the font! SDL_TTF Error: %s \n", TTF_GetError());
		success = false;
	}else{
		SDL_Color textColor = {0, 0, 0};
		if(!textTexture.loadFromRenderedText("Score: 0", textColor, font)){
			printf("Failed to render text texture! \n");
			success = false;
		}
	}

	if(!dotTexture.loadFromFile("images/jet1.png")){
		printf( "Failed to load dot texture!\n" );
		success=false;
	}
	if(!bgTexture.loadFromFile("backgrounds/background.png")){
		printf( "Failed to load bg texture!\n" );
		success=false;
	}
	if(!bulletTexture.loadFromFile("images/tinydot.png")){
		printf( "Failed to load tinydot texture!\n" );
		success=false;
	}
	if(!trump1Texture.loadFromFile("images/trump1.png")){
		printf( "Failed to load tinydot texture!\n" );
		success=false;
	}
	if(!trump2Texture.loadFromFile("images/trump2.png")){
		printf( "Failed to load tinydot texture!\n" );
		success=false;
	}
	if(!trump3Texture.loadFromFile("images/trump3.png")){
		printf( "Failed to load tinydot texture!\n" );
		success=false;
	}
	if(!tileTexture.loadFromFile("images/tiles.png")){
		printf( "Failed to load tiles texture!\n" );
		success=false;
	}
	if(!setTiles(tiles)){
		printf( "Failed to load tile set!\n" );
		success=false;
	}
	if(!explosionSheet.loadFromFile( "images/explosion.png" )){
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }else{
    	int counter = 0;
        for(int row = 0; row < 225; row+=45){
        	for(int col = 0; col < 225; col+=45){
		        explosionClips[ counter ].x =   col;
		        explosionClips[ counter ].y =   row;
		        explosionClips[ counter ].w =   45;
		        explosionClips[ counter ].h =   45;
		    	++counter;	
		    }
	    }
    }

	return success;
}

void close(Tile* tiles[]){
	textTexture.free();
	dotTexture.free();
	bgTexture.free();
	bulletTexture.free();
	tileTexture.free();

	//Deallocate tiles
	for(int i = 0; i < totalTiles; ++i){
		 if(tiles[i] == NULL){
			delete tiles[ i ];
			tiles[i] = NULL;
		 }
	}

	TTF_CloseFont(font);
	font = NULL;
	//Destroy window
	SDL_DestroyWindow( window );
	window = NULL;
	SDL_DestroyRenderer( renderer );
	renderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] ){
	if(!init()){
		printf("Error on init \n");
	}else{
		Tile* tileSet[totalTiles];
		if(!loadMedia(tileSet)){
			printf("Error on loadMedia \n");
		}else{
			
			printf("Start \n");
			bool quit = false;
			SDL_Event e;
			LTimer fpsTimer;
			LTimer capTimer;

			int countedFrames = 0;
			fpsTimer.start();

			int frame = 0;
			
			Dot dot(0,0);
			std::vector<Weapons*> bullets;
			std::vector<Fighters*> trumps;
			std::vector<Explosions*> explos;
			std::vector<SDL_Rect> bulletBoxes;
			LTexture * trumpImgs[] = {&trump1Texture, &trump2Texture, &trump3Texture};
			dot.loadBullets(&bullets);
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			int scrollingOffset = 0;
			SDL_Color textColor = {0, 0, 0};

			srand(time(NULL));

			while(!quit){
				capTimer.start();
				
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					dot.handleEvent(e);
				}

				float avgFPS = countedFrames / (fpsTimer.getTicks() / 500.f);
				if(avgFPS > 2000000){
					avgFPS = 0;
				}

				--scrollingOffset;
				if(scrollingOffset < -bgTexture.getWidth()){
					scrollingOffset = 0;
				}

				if((rand() % 50) == 1){
					Fighters* trumper = new Fighters(SCREEN_WIDTH, (rand() % SCREEN_HEIGHT), (rand() % 3), trumpImgs[(rand() % 3)]);
					trumps.push_back(trumper);
				}

				dot.move(trumps, tileSet);
				dot.setCamera(camera);

				//Clear screen
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(renderer);

				//renderBackground
				//bgTexture.render(scrollingOffset, 0);
				//bgTexture.render(scrollingOffset + bgTexture.getWidth(), 0);
				
				//renderTileBackground
				for(int i = 0; i < totalTiles; ++i){
					SDL_Rect clip = tileClips[tileSet[i]->getType()];
					tileSet[i]->render(camera, &tileTexture, clip);
				}

				//Render objects
				dot.render(&dotTexture, camera);

				for(int counter = 0; counter < bullets.size(); counter++){
					Weapons *bullet = bullets[counter];
					if(bullet->getPosX() < SCREEN_WIDTH){
						bullet->render(&bulletTexture);
						bullet->increment();
					}else{
						bullet->setIsDead(true);
					}
				}
				for(int counter = 0; counter < bullets.size(); counter++){
					Weapons *bullet = bullets[counter];
					if(bullet->isDead()){
						bullets[counter] = bullets[bullets.size() - 1];
						bullets[bullets.size() - 1] = bullet;
						bullets.pop_back();
						delete bullet;
						--counter;
					}
				}	

				for(int counter = 0; counter < trumps.size(); counter++){
					Fighters *trump = trumps[counter];
					if(trump->getPosX() > 3){
						trump->render();
						bool hit = trump->increment(&bullets);
						if(hit == true){
							trump->setIsDead(true, true);
							++score;
						}
					}else{
						trump->setIsDead(true, false);
						--score;
					}
				}
				for(int counter = 0; counter < trumps.size(); counter++){
					Fighters *trump = trumps[counter];
					if(trump->isDead()){
						if(trump->exploded()){
							Explosions* explo = new Explosions(trump->getPosX(), trump->getPosY());
							explo->loadClips(explosionClips);
							explos.push_back(explo);
						}

						trumps[counter] = trumps[trumps.size() - 1];
						trumps[trumps.size() - 1] = trump;
						trumps.pop_back();
						delete trump;
						--counter;
					}
				}	

				for(int counter = 0; counter < explos.size(); counter++){
					Explosions *explo = explos[counter];
					if(explo->isDead()){
						explos[counter] = explos[explos.size() - 1];
						explos[explos.size() - 1] = explo;
						explos.pop_back();
						delete explo;
						--counter;
					}else{
						explo->render(&explosionSheet);
						explo->increment();
					}
				}	

				if(score > 0){
					textColor = {0, 255, 0};
				}else if(score < 0){
					textColor = {255, 0, 0};
				}else{
					textColor = {0, 0, 0};
				}
				changeScore(score, textColor);
				textTexture.render((SCREEN_WIDTH - textTexture.getWidth() - 30), 10);


				//Update screen
				SDL_RenderPresent(renderer);
				
				++frame;
				if(frame / 25 >= explosionFrames){
					frame = 0;
				}

				++countedFrames;

				int frameTicks = capTimer.getTicks();
				if(frameTicks < SCREEN_TICKS_PER_FRAME){
					SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
				}
			}
		}
		close(tileSet);
	}

	printf("End \n");
	return 0;
}