#pragma once 

//Screen dimension constants
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int LEVEL_WIDTH = 1280;
int LEVEL_HEIGHT = 480;

//FPS
int SCREEN_FPS = 60;
int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

//tiles
const int tileWidth = 80;
const int tileHeight = 80;
const int totalTiles = 192;
const int totalTileSprites = 12;

//tile sprites
const int tileRed = 0;
const int tileGreen = 1;
const int tileBlue = 2;
const int tileCenter = 3;
const int tileTop = 4;
const int tileTopRight = 5;
const int tileRight = 6;
const int tileBottomRight = 7;
const int tileBottom = 8;
const int tileBottomLeft = 9;
const int tileLeft = 10;
const int tileTopLeft = 11;

//MISC
int DOT_VELOCITY = 6;
int BULLET_VELOCITY = 5;
int TRUMP_VELOCITY[] = {3, 5, 7};
const int gameOverTop = 30;
const int gameOverBottom = SCREEN_HEIGHT - 30;