#ifndef TILES_H_INCLUDED
#define TILES_H_INCLUDED

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#endif

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "texture.h"
#include "player.h"

using std::vector;
const int totalTiles_=3600;
//Initialize load media function for main file here so doors work
bool loadMedia();

Mix_Music *overworldTheme;
Mix_Music *flowerTheme;

bool loadMap(std::string path);

SDL_Rect tileSet [64];

bool loadTileSet(std::string path);

//String from which to load map
std::string map = "maps/1.map";

class Tile{
	public:
		//initialize tile
		Tile(int x, int y, int spT)
		{hitBox_.x=x; hitBox_.y=y; hitBox_.w=32; hitBox_.h=32; sprite_=spT;}

		//Virtual fuction to decide what happens on collision
		virtual void collision(Player &man)=0;

		//render individual tile
		void render(SDL_Rect & camera);


		int getSprite()
		{return sprite_;}
	protected:

		int sprite_;
		//hitbox for the block
		SDL_Rect hitBox_;
};

//Different types of tiles
class Wall: public Tile{
	public:
		Wall(int x, int y, int s):Tile(x,y,s)
		{}
		void collision(Player &man);

};

class Door: public Tile{
	public:
		Door(int x, int y, int s):Tile(x,y,s)
		{}
		void collision(Player &man);
};

class Bush: public Tile{
	public:
		Bush(int x, int y, int s):Tile(x,y,s)
		{}
		void collision(Player &man);

};

class Floor: public Tile{
	public:
		Floor(int x, int y, int s):Tile(x,y,s)
		{}
		void collision(Player &man);

};
//vector of tile pointers to store the state of the world
vector<Tile*> THEWORLD(totalTiles_);

//load and chop tilesheet into rects
bool loadTileSet(std::string path)
{
	if(tileSheet.loadFromFile(path))
	{
		int xc=0;
		int yc=0;
		for(int i=0; i<64; ++i)
		{
			tileSet[i].x=xc;
			tileSet[i].y=yc;

			tileSet[i].w=32;
			tileSet[i].h=32;

			xc+=32;
			if(xc==256)
			{
				xc=0;
				yc+=32;
			}
		}
		return true;
	}
	return false;
}


//load map

bool loadMap(std::string path)
{
    clearMobs();
    clearstaticMobs();
    clearchaseMobs();
    clearObjects();
	std::ifstream map(path.c_str());
	if(!map)
    {std::cout<<"unable to find map file, please check file names and strings\n"; return false;}
	else
	{
		int xc=0;
		int yc=0;
		int ST=-1;
		for(int i=0; i<totalTiles_; ++i)
		{
			ST=-1;
			map>>ST;
			if((ST>=0 && ST<17) || (ST>=56 && ST<64))
			{THEWORLD[i]= new Wall(xc, yc, ST);}
			else if(ST>=17 && ST<43)
			{THEWORLD[i]= new Floor(xc, yc, ST);}
			else if(ST>=43 && ST<48)
			{THEWORLD[i]= new Bush(xc, yc, ST);}
			else if(ST>=48 && ST<56)
			{THEWORLD[i]= new Door(xc, yc, ST);}
			else
			{std::cout<<"Error reading tile from map\n"<<std::endl;}
		
			xc+=32;
			if(xc==2560)
			{
				xc=0;
				yc+=32;
			}
		}
	}
    loadMobs(mPath);
	map.close();
return true;
}

void Tile::render(SDL_Rect & camera)
{
	//renders only tiles on screen
	if(checkCollision(camera, hitBox_))
	{
		tileSheet.render(hitBox_.x-camera.x, hitBox_.y-camera.y, &tileSet[sprite_]);
	}
}

void BuildWorld(SDL_Rect & cam)
{
	//feeds the vector of tiles to the renderfunction this gets called in the main loop in place of a background image
	for(int i=0; i<totalTiles_; ++i)
	{
		THEWORLD[i]->render(cam);
	}
}

//knockback if player touches a wall tile
void Wall::collision(Player &man)
{
	if(checkCollision(man.getmCollide(), hitBox_))
	{
		man.Xknockback();
		man.Yknockback();
	}
    
    /*
     
     BULLET addition
     
        delets bullets when they hit a wall
     
    */
    //ADDED FOR BULLET
    for(int i=0;i<man.boomBooms.size();++i)
    {
        if(checkCollision(man.boomBooms[i].getmCollide(), hitBox_))
        {
            man.bulletHit(i);
        }
    }
}

//loads new map and repositions player when door tile is touched
void Door::collision(Player &man)
{
	//int count;
	if(checkCollision(man.getmCollide(), hitBox_) && man.getType() == 'p')
	{
		if(map=="maps/1.map" && man.getPosX()>=(LEVEL_WIDTH/2) && man.getPosY()<=(LEVEL_HEIGHT/2))
		{
			map="maps/disco.map";
            mPath = "maps/mobs/Mobs.mobs";
            loadMap(map);
//			if(!loadMedia())
//        	{ std::cout << "UNABLE TO LOAD GRAPHICS! NOTHING TO SEE HERE!" << std::endl; }
        	man.setPos(1120,825);
            man.clearBullets();
    	}
    	else if(map=="maps/1.map" && man.getPosX()>=(LEVEL_WIDTH/2) && man.getPosY()>=(LEVEL_HEIGHT/2))
		{
			map="maps/2.map";
            mPath = "maps/mobs/Mobs2.mobs";
            loadMap(map);
//			if(!loadMedia())
//        	{ std::cout << "UNABLE TO LOAD GRAPHICS! NOTHING TO SEE HERE!" << std::endl; }
            Mix_HaltMusic();
            Mix_PlayMusic(flowerTheme, -1);
       	 	man.setPos(715, 450);
            man.clearBullets();
            
    	}
    	else if(map=="maps/2.map")
    	{
    		map="maps/1.map";
            mPath = "maps/mobs/Mobs1.mobs";
            loadMap(map);
//    		if(!loadMedia())
//        	{ std::cout << "UNABLE TO LOAD GRAPHICS! NOTHING TO SEE HERE!" << std::endl; }
       	 	man.setPos(2455, 1020);
            man.clearBullets();
            Mix_HaltMusic();
            Mix_PlayMusic(overworldTheme, -1);
    	}
    	else if((map=="maps/disco.map" ||map=="maps/disco01.map" ||map=="maps/disco02.map" ||map=="maps/disco03.map" ||map=="maps/disco04.map" ||map=="maps/disco05.map" ||map=="maps/disco06.map" ||map=="maps/disco07.map" ||map=="maps/disco08.map" ||map=="maps/disco09.map") && man.getPosY()==860 && man.getPosX()==1120)
    	{
    		map="maps/1.map";
            mPath = "maps/mobs/Mobs1.mobs";
            loadMap(map);
//    		if(!loadMedia())
//        	{ std::cout << "UNABLE TO LOAD GRAPHICS! NOTHING TO SEE HERE!" << std::endl; }
        	man.setPos(1635, 380);
            man.clearBullets();
    	}
	}
}

void Bush::collision(Player &man)
{
	if(checkCollision(man.getmCollide(), hitBox_))
	{
		
	}
}

void Floor::collision(Player &man)
{}

//loop to check all colisions
void checkAll(Player &man)
{
	for(int i=0; i<THEWORLD.size(); ++i)
	{
		THEWORLD[i]->collision(man);
	}
}
#endif