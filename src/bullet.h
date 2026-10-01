//
//  bullet.h
//  Build-1
//
//  Created by BOB on 4/27/15.
//  Copyright (c) 2015 BOB. All rights reserved.
//

#ifndef __Build_1__bullet__
#define __Build_1__bullet__

#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>

class Bullet
{
public:
    Bullet(int angle,int x, int y):xVel(10.0*(double)cos((double)angle*(M_PI/180.0))),yVel(10.0*(double)sin((double)angle*(M_PI/180.0))), dmg(32)
    {
        mCollide = {x+11, y+11, 10, 10};
        std::cout<<"XV: " <<xVel <<" YV: " <<yVel <<" A: "<< angle <<std::endl;
    };
    
    SDL_Rect getmCollide()
    {return mCollide;}
    
    int getDmg()
    {return dmg;}
    
    void collide();
    
    void move();
    
private:
    
    SDL_Rect mCollide;
    
    int dmg;
    
    double xVel;
    double yVel;
};

void Bullet::move()
{
    mCollide.x+=xVel;
    mCollide.y+=yVel;
}

#endif /* defined(__Build_1__bullet__) */
