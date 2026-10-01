#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#ifdef __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#endif

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include "texture.h"
#include "bullet.h"


const int WALKING_ANIMATION_FRAMES = 3; const int SCREEN_WIDTH = 1280; const int SCREEN_HEIGHT = 720; const int LEVEL_HEIGHT = 1440; const int LEVEL_WIDTH = 2560;

bool checkCollision( SDL_Rect a, SDL_Rect b );

//The player that will move around on the screen
class Player
{
public:
    //The dimensions of the player
    static const int PLAYER_WIDTH = 32;
    static const int PLAYER_HEIGHT = 32;
    
    //Maximum axis velocity of the player
    static const int PLAYER_VEL = 5;
    
    /* PLAYER INITIALIZATION CONSTRUCTS */
        Player();
        Player(int x, int y);
        Player(int x, int y, int d, int h);

    /*  MOVING CHARACTER    */
        //Takes key presses and adjusts the player's velocity
        void handleEvent( SDL_Event& e);
    
        //Moves player and sets animation
        virtual void move(int * frame);

        virtual bool die()
        {
            return health<=0;
        }
    
        //adjust the camera
        void setCam(SDL_Rect &cam);
    
//      Useful?
        //virtual void follow(Player & target) {}

        void setController(int c)
        {
            pController = c;
        }
        int getController()
        {
            return pController;
        }
        //Knockback functions for both the x and y axis
        virtual void Xknockback();
        virtual void Yknockback();
        virtual void changeDirection();
    
        //Shows the player on the screen
        virtual void render(SDL_Rect & camera);

        virtual bool isDead() {return dead;}
    
    /* ACCESSOR FUNCTIONS */
    
        //X-position
        int getPosX() {return mPosX;}
    
        //Y-position
        int getPosY() {return mPosY;}

        SDL_Rect getmCollide() {return mCollider;}

        int getVelX() {return mVelX;}
    
        int getVelY() {return mVelY;}
    
        void setPos(int x, int y);
    
        int getHealth()
        { return health; }
        
        int getMaxHealth()
        {   return MAX_HEALTH; }
    
    
    /*  ANIMATION   */
    
        //Assign sprite sheet values for animation
        void assignSpriteSheet();
    
        //Set animation frame
        void setSprite(SDL_Rect anim[], int frame)
        {   currentClip = anim[frame];  }
    
        //USED FOR DEATH ANIMATION; CALLED FROM "menu.h"
        void setSpriteDeath(int animClip)
        {
            switch(animClip)
            {
                case 0:
                    currentClip = gSpriteClipDown[0];
                    break;
                case 1:
                    currentClip = gSpriteClipLeft[0];
                    break;
                case 2:
                    currentClip = gSpriteClipUp[0];
                    break;
                case 3:
                    currentClip = gSpriteClipDown[0];
                    break;
                    
                case 4:
                    currentClip = deathSprite;
                    break;
                    
                default:
                    break;
            }
        }
    
        //Sets initial animation frame
        void setSpritInit()
        { currentClip = gSpriteClipDown[0]; }
    
        //Get current animation frame
        SDL_Rect getFrame()
        { return currentClip; }
    /*
     
     BULLET ADDittion
     
     shoot function
     vector of bullets
     remove bullet function
     
     */
    //BULLETS ARE HERE
    
    //Shoot function to spawn bullets
    void shoot(double angle);
    
    //BULLETS from this player
    std::vector<Bullet> boomBooms;
    
    //removes indicated bullet
    int bulletHit(int indx);
    void clearBullets();
    
    void hit(int dmg);
    
    /*
     
     Pause menue addition
     
     */
    char getType()
    { return type; }
    //Sets player velocity to 0
    void stop();
    
protected:
    bool dead;
    
    char type;
    
    //The X and Y offsets of the player
    int mPosX, mPosY;
    
    //The velocity of the player
    int mVelX, mVelY;

    //health & damage
    int health;
    int MAX_HEALTH;
    int damage;
    
    //Player's collision box
    SDL_Rect mCollider;
    
    //Animation rects
    SDL_Rect gSpriteClipUp[WALKING_ANIMATION_FRAMES];
    SDL_Rect gSpriteClipDown[WALKING_ANIMATION_FRAMES];
    SDL_Rect gSpriteClipLeft[WALKING_ANIMATION_FRAMES];
    SDL_Rect gSpriteClipRight[WALKING_ANIMATION_FRAMES];
    SDL_Rect deathSprite;
    SDL_Rect currentClip;
    
    /*
    
     BULLET ADDittion
     
     added some veriables to do shooting angles
     a bool to controll speed of shooting( I hope)
     
    */
    
    //Controler shit and what ever
    //Player's Controller
    int pController;
    
    //Player's shooting angle
    double angleXcomp;
    double angleYcomp;
    double angle;
    
    //Shot controll
    bool canShoot;
};

void Player::stop()
{
    mVelX = 0;
    mVelY = 0;
}

Player::Player()
{
    std::cout<< "player constructor called"<< std::endl;
    //Initialize the offsets
    mPosX = LEVEL_WIDTH/2;
    mPosY = LEVEL_HEIGHT/2;
    currentClip = gSpriteClipDown[0];
    
    //Set collision box dimension
    mCollider.w = (PLAYER_WIDTH/2);
    mCollider.h = (PLAYER_HEIGHT/2);
    
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    MAX_HEALTH = health = 30;
    damage=2;
    type = 'p';
    dead = false;
}

Player::Player(int x, int y)
{
    std::cout<< "player constructor called"<< std::endl;
    //Initialize the offsets
    mPosX = x;
    mPosY = y;
    currentClip = gSpriteClipDown[0];
    
    //Set collision box dimension
    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;
    
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    damage=0;
    MAX_HEALTH = health = 30;
    dead = false;
}

Player::Player(int x, int y, int d, int h)
{
    std::cout<< "player constructor called"<< std::endl;
    //Initialize the offsets
    mPosX = x;
    mPosY = y;
    currentClip = gSpriteClipDown[0];
    
    //Set collision box dimension
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    
    mCollider.w = (PLAYER_WIDTH);
    mCollider.h = (PLAYER_HEIGHT);
    
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    damage=d;
    MAX_HEALTH = health = h;
    dead = false;
}
/*
 
 BULLET ADDittion
    Added shoot function (adds bullets its like a factroy)
    removebullet function
 
 */

//BULLET Functions
void Player::shoot(double angle)
{
    std::cout<<"A: " <<angle <<std::endl;
    Bullet temp(angle, mPosX, mPosY);
    boomBooms.push_back(temp);
}

void Player::clearBullets()
{
    boomBooms.clear();
}

int Player::bulletHit(int indx)
{
    int d;
    d = boomBooms[indx].getDmg();
    boomBooms.erase(boomBooms.begin()+indx);
    return d;
}

void Player::hit(int dmg)
{
    health-=dmg;
    if(die())
    {
        std::cout<< "Player Dead!" <<std::endl;
        dead=true;
    }
}

void Player::handleEvent( SDL_Event& e)
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= PLAYER_VEL; break;
            case SDLK_DOWN: mVelY += PLAYER_VEL; break;
            case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
            case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
            case SDLK_SPACE: std::cout<< mPosX << " , " << mPosY << std::endl;
            case SDLK_RETURN: shoot(M_PI/4); break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += PLAYER_VEL; break;
            case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
            case SDLK_LEFT: mVelX += PLAYER_VEL; break;
            case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
        }
    }
    
    /*
     
     BULLET ADDittion
     
        Everything below in the event handler function
     
        controler support (player movement controlled by any controller)
     
        angles calculated from right stick
     
     */
    //neither of the controller functions check for witch controller
    
    //If a axis on a controller is changed
    if(e.type == SDL_JOYAXISMOTION)
    {
        std::cout <<"p" << pController << " c" <<e.jaxis.which <<std::endl;
        if(e.jaxis.which == pController)
        {
            //checks if it was the x-axis
            if(e.jaxis.axis ==0)
            {
                //checks if it is greater or less than the hardcoded deadzone
                //dead zone can be moved later
                if(e.jaxis.value > 15000)
                {
                    mVelX = PLAYER_VEL;
                }
                else if (e.jaxis.value <-15000)
                {
                    mVelX = -PLAYER_VEL;
                }
                //when its not greater or less the nthe negative x vel is set t0 0
                else
                {
                    mVelX=0;
                }
            }
            //same as x axis but for the y axis
            else if(e.jaxis.axis ==1)
            {
                if(e.jaxis.value > 15000)
                {
                    mVelY = PLAYER_VEL;
                }
                else if (e.jaxis.value <-15000)
                {
                    mVelY = -PLAYER_VEL;
                }
                else
                {
                    mVelY=0;
                }
            }
            
            
            //checks X axis of shooting stick
            if(e.jaxis.axis == 3)
            {
                angleXcomp = e.jaxis.value;
    //                        //x component of angle
    //                        if(e.jaxis.value > 15000)
    //                        {
    //                            //angle thing hopefully
    //                            angleXcomp = 1;
    //                        }
    //                        else if (e.jaxis.value <-15000)
    //                        {
    //                            //angle thing hopefully
    //                            angleXcomp = -1;
    //                        }
    //                        //when its not greater or less the nthe negative x vel is set t0 0
    //                        else
    //                        {
    //                            //angle thing hopefully
    //                            angleXcomp = 0;
    //                        }
            }
            //same as x axis but for the y axis
            else if(e.jaxis.axis ==4)
            {
               angleYcomp = e.jaxis.value;
    //                        if(e.jaxis.value > 15000)
    //                        {
    //                            //angle thing hopefully
    //                            angleYcomp = 1;
    //                        }
    //                        else if (e.jaxis.value <-15000)
    //                        {
    //                            //angle thing hopefully
    //                            angleYcomp = -1;
    //                        }
    //                        else
    //                        {
    //                            //angle thing hopefully
    //                            angleYcomp = 0;
    //                        }
            }
            else if(e.jaxis.axis == 5)
            {
                if(e.jaxis.value > -23000)
                {
                    if(canShoot)
                    {
                        shoot(angle);
                        std::cout <<"THHHHH :" <<angle<<std::endl;
                        canShoot = false;
                    }
                }
                else
                {
                    if(!canShoot)
                    {
                        canShoot=true;
                    }
                }
            }
            angle = atan2(angleYcomp, angleXcomp)*(180.0/M_PI);
            if(angleXcomp== 0 && angleYcomp == 0)
            {
                angle = 0;
            }
        }
        //checks for button input from controllers
        else if(e.type == SDL_JOYBUTTONDOWN)
        {
            if(e.jbutton.button == 11)
            {
                shoot(M_PI/4);
            }
        }
    }

}

//Movement w/o collision detect
void Player::move(int * frame)
{
    //Move the player left or right
    mPosX += mVelX;
    mCollider.x = mPosX+mCollider.w;

    //Move the player up or down
    mPosY += mVelY;
    mCollider.y = mPosY+mCollider.h;
    
 /* ANIMATION */

        //Right
        if(mVelX > 0)
        { setSprite(gSpriteClipRight, *frame); }
    
        //Left
        else if(mVelX < 0)
        { setSprite(gSpriteClipLeft, *frame); }

        //Up
        if(mVelY > 0)
        { setSprite(gSpriteClipDown, *frame); }
        
        //Down
        else if(mVelY < 0)
        { setSprite(gSpriteClipUp, *frame); }

        
/*  WALL COLLISION (SCREEN)  */
    //If the player collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + PLAYER_WIDTH > LEVEL_WIDTH ))
    {
        //Move back
        Xknockback();
    }
    
    //If the player collided or went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + PLAYER_HEIGHT > LEVEL_HEIGHT ) )
    {
        //Move back
        Yknockback();
    }
    /*
     
     BULLET ADDittion
     
        added bullet movement call
     
     */
    
    //BULLETS move here
    if(boomBooms.size()>0)
    {
        for(int i=0; i<boomBooms.size();++i)
        {
            boomBooms[i].move();
            std::cout <<boomBooms[i].getmCollide().x <<std::endl;
        }
    }
    
}

void Player::setCam(SDL_Rect &camera)
{
    if(!checkCollision(mCollider, camera))
    {
        camera.x = mPosX - camera.w/2;
        camera.y = mPosY - camera.h/2;
    }
        if(getPosX()<camera.x)
            {
                camera.x-=(SCREEN_WIDTH/2);
            }
        if(getPosX()>camera.x+SCREEN_WIDTH)
            {
                camera.x+=(SCREEN_WIDTH/2);
            }
        if(getPosY()<camera.y)
            {
                camera.y-=(SCREEN_HEIGHT/2);
            }
        if(getPosY()>camera.y+SCREEN_HEIGHT)
            {
                camera.y+=(SCREEN_HEIGHT/2);
            }

                //Keep the camera in bounds
        if( camera.x < 0 )
            { 
                camera.x = 0;
            }
        if( camera.y < 0 )
            {
                camera.y = 0;
            }
        if( camera.x > LEVEL_WIDTH - camera.w )
            {
                camera.x = LEVEL_WIDTH - camera.w;
            }
        if( camera.y > LEVEL_HEIGHT - camera.h )
            {
                camera.y = LEVEL_HEIGHT - camera.h;
            }

}

void Player::Xknockback()
{
    if(mVelX<=0)
    {
        mPosX-=mVelX-1;
    }
    else if (mVelX>=0)
    {
        mPosX-=mVelX+1;
    }
    mCollider.x = mPosX;

}

void Player::Yknockback()
{
    if(mVelY<=0)
    {
        mPosX-=mVelX-1;
    }
    else if (mVelY>=0)
    {
        mPosX-=mVelX+1;
    }
     mPosY -= mVelY*1;
     mCollider.y = mPosY-mCollider.h;
}

void Player::changeDirection()
{
    //Player does not change direction
}

void Player::render(SDL_Rect & camera)
{
    //Show the player
    spriteSheets.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
    
    /*
     
     BULLET ADDittion
     
        added bullet render call
     
     */
    
    if(boomBooms.size()>0)
    {
        for(int i=0; i<boomBooms.size();++i)
        {
            spriteSheets.renderB(boomBooms[i].getmCollide(), camera.x, camera.y);
            //std::cout <<boomBooms[i].getmCollide().x <<" " <<boomBooms[i].getmCollide().y <<std::endl;
        }
        
    }
}

void Player::assignSpriteSheet()
{
    //Size of each sprite in sheet
    int width = 32;
    int height = 32;
    
    // Fill sprite animation arrays
    for(int i = 0; i < WALKING_ANIMATION_FRAMES; ++i)
    {
        gSpriteClipDown[i].x = width*i;
        gSpriteClipDown[i].y = height*0;
        gSpriteClipDown[i].w =  32;
        gSpriteClipDown[i].h =  32;
        
        gSpriteClipLeft[i].x = width*i;
        gSpriteClipLeft[i].y = height*1;
        gSpriteClipLeft[i].w =  32;
        gSpriteClipLeft[i].h =  32;
        
        gSpriteClipRight[i].x = width*i;
        gSpriteClipRight[i].y = height*2;
        gSpriteClipRight[i].w =  32;
        gSpriteClipRight[i].h =  32;
        
        gSpriteClipUp[i].x = width*i;
        gSpriteClipUp[i].y = height*3;
        gSpriteClipUp[i].w =  32;
        gSpriteClipUp[i].h =  32;
    }
    
    deathSprite.x = 0;
    deathSprite.y = height*4;
    deathSprite.h = 32;
    deathSprite.w = 32;
}

void Player::setPos(int x, int y)
{
    mPosX = x;
    mCollider.x = mPosX+mCollider.w;

    //Move the player up or down
    mPosY =y;
    mCollider.y = mPosY+mCollider.h;
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;
    
    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    
    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }
    
    if( topA >= bottomB )
    {
        return false;
    }
    
    if( rightA <= leftB )
    {
        return false;
    }
    
    if( leftA >= rightB )
    {
        return false;
    }
    
    //If none of the sides from A are outside B
    return true;
}

//No Player Collision
void playerCollide(Player & man1, Player & man2)
{
    if(checkCollision(man1.getmCollide(), man2.getmCollide()))
    {
        man1.Xknockback();
        man1.Yknockback();
        man2.Xknockback();
        man2.Yknockback();
        man2.Xknockback();
        man2.Yknockback();
        man2.hit(5);
    }
    for(int i=0;i<man2.boomBooms.size();++i)
    {
        if(checkCollision(man2.boomBooms[i].getmCollide(), man1.getmCollide()))
        {
            man1.hit(man2.bulletHit(i));
        }
    }
}

//POT CLASS

class Pot:public Player{
    
public:
    
    Pot(int x, int y):Player(x,y),pickedUp(false) {}
    
    void getPickedUp( SDL_Event& e , Player & man);
    
    bool amIpickedUp() {return pickedUp;}
    
    void move(int * frame);
    
    void follow()
    {
        if(pickedUp)
        {
            setPos(target->getPosX(), target->getPosY());
            mVelX=target->getVelX();
            mVelY=target->getVelY();
        }
        else if(!pickedUp)
            stop();
    }
    bool die()
    {
        //Useful?
        if(health<=0) { }
        return false;
    }
    
    void render(SDL_Rect & camera)
    {
        if(checkCollision(camera , mCollider))
        {
            pot.render(mPosX-camera.x, mPosY-camera.y, &currentClip);
        }
    }
    
private:
    
    Player* target;
    bool pickedUp;
};

std::vector<Pot> Os;

void addObject(int t, int x, int y)
{
        Pot temp(x,y);
        Os.push_back(temp);
        Os[Os.size()-1].assignSpriteSheet();
        Os[Os.size()-1].setSpritInit();
}

void clearObjects()
{
    Os.clear();
}

void Pot::getPickedUp(SDL_Event & e, Player & man)
{
    
    if(checkCollision(mCollider, man.getmCollide()))
        
    {
        
        if( (e.type == SDL_KEYDOWN && e.key.repeat == 0) || e.type == SDL_JOYBUTTONDOWN)
            
        {
            if(e.key.keysym.sym == SDLK_SPACE)
            {
                pickedUp=!pickedUp;
                if(pickedUp)
                {
                    target = &man;
                }
                else
                {
                    target = NULL;
                }
            }
            if(e.jbutton.button == 0 && e.jbutton.which == man.getController())
            {
                std::cout << e.jbutton.which <<" " <<man.getController() <<std::endl;
                pickedUp = !pickedUp;
                if(pickedUp)
                {
                    target = &man;
                }
                else
                {
                    target = NULL;
                }
            }
        }
        
    }
    
}

void Pot::move(int * frame)
{
    //Move the player left or right
    mPosX += mVelX;
    mCollider.x = mPosX;
    
    //Move the player up or down
    mPosY += mVelY;
    mCollider.y = mPosY;
    /* ANIMATION */
    
    //Right
    if(mVelX > 0)
    { setSprite(gSpriteClipRight, *frame); }
    
    //Left
    else if(mVelX < 0)
    { setSprite(gSpriteClipLeft, *frame); }
    
    //Up
    if(mVelY > 0)
    { setSprite(gSpriteClipDown, *frame); }
    
    //Down
    else if(mVelY < 0)
    { setSprite(gSpriteClipUp, *frame); }
}

//
//MOB CLASSES
//

class Mob:public Player
{
public:
    Mob(int x1, int y1, int d, int h, int s):Player(x1, y1, d, h), inRange(false), Sprite(s)
    {range={x1-160, y1-160, 320, 320};}
    
    void move(int * frame)
    {
        //Move left or right
        mPosX += mVelX;
        mCollider.x = mPosX;
        range.x=mPosX-160;
        
        //Move up or down
        mPosY += mVelY;
        mCollider.y = mPosY;
        range.y=mPosY-160;
        
        /* ANIMATION */
        
        //Right
        if(mVelX > 0)
        { setSprite(gSpriteClipRight, *frame); }
        
        //Left
        else if(mVelX < 0)
        { setSprite(gSpriteClipLeft, *frame); }
        
        //Up
        if(mVelY > 0)
        { setSprite(gSpriteClipDown, *frame); }
        
        //Down
        else if(mVelY < 0)
        { setSprite(gSpriteClipUp, *frame); }
    }
    
    void changeDirection();
    void Xknockback();
    void Yknockback();
    
    void behavior(Player & target)
    {
        mVelX = MOB_VEL;
        const int Xorig = getPosX();
        
        if(inRange)
        {
            stop();
            
            if(target.getPosX()>mPosX)
                mVelX=MOB_VEL;
            if(target.getPosX()<mPosX)
                mVelX=-MOB_VEL;
            if(target.getPosX()==mPosX)
                mVelX=0;
            if(target.getPosY()>mPosY)
                mVelY=MOB_VEL;
            if(target.getPosY()<mPosY)
                mVelY=-MOB_VEL;
            if(target.getPosY()==mPosY)
                mVelY=0;
        }
        else if (!inRange)
        {
            if (mPosX>Xorig+32*5)
            {
                mVelX = -mVelX;
            }
            
        }
    }
    
    void render(SDL_Rect & camera)
    {
        int t = Sprite;
        
        switch (t)
        {
            case 1:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    mobSprite.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
                
            case 2:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    theBoss.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
                
            default:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    spriteSheets.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
        }
        
/* ORIGINAL CODE
        if(checkCollision(camera, mCollider))
        {
            SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
            SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
            SDL_RenderFillRect(gameRenderer, &data);
            bossmobSprite.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
        }
*/
        
    }
    
    void checkRange(Player & target)
    {
        if((checkCollision(target.getmCollide(), range)))
            inRange=true;
    }
    
protected:
    static const int MOB_VEL = PLAYER_VEL/2;
    bool inRange;
    int Sprite;
    SDL_Rect range;
};

void Mob::changeDirection()
{
    mVelX = -mVelX;
}

void Mob::Xknockback()
{
    mPosX -= mVelX*1.5;
    //mVelX=mVelX*-1;
    mCollider.x = mPosX;
    std::cout <<"dieBITCH" <<std::endl;
}

void Mob::Yknockback()
{
    mPosY -= mVelY*1.5;
    //mVelY=mVelY*-1;
    mCollider.y = mPosY;
}

class staticMob:public Player
{
public:
    
    staticMob(int x1, int y1, int d, int h, int s):Player(x1, y1, d, h), Sprite(s) {}
    
    void move(int * frame)
    {}
    
    void behavior (Player & target)
    {
        stop();
    }
    
    void render(SDL_Rect & camera)
    {
        int t = Sprite;
        
        switch (t)
        {
            case 1:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    mobSprite.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
                
            case 2:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    theBoss.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
                
            default:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    spriteSheets.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
        }
    }

protected:
    static const int MOB_VEL = PLAYER_VEL/2;
    int Sprite;
};

class chaseMob:public Player
{
public:
    chaseMob(int x1, int y1, int d, int h, int s):Player(x1, y1, d, h), inRange(false), Sprite(s) {range={x1-160, y1-160, 320, 320};}
    
    void move(int * frame)
    {
        //Move left or right
        mPosX += mVelX;
        mCollider.x = mPosX;
        range.x=mPosX-160;
        
        //Move up or down
        mPosY += mVelY;
        mCollider.y = mPosY;
        range.y=mPosY-160;
        
        /* ANIMATION */
        
        //Right
        if(mVelX > 0)
        { setSprite(gSpriteClipRight, *frame); }
        
        //Left
        else if(mVelX < 0)
        { setSprite(gSpriteClipLeft, *frame); }
        
        //Up
        if(mVelY > 0)
        { setSprite(gSpriteClipDown, *frame); }
        
        //Down
        else if(mVelY < 0)
        { setSprite(gSpriteClipUp, *frame); }
    }
    
    void behavior (Player & target)
    {
        if (!inRange) {
            stop();
        }
        else
        {
            if(target.getPosX()>mPosX)
                mVelX=MOB_VEL;
            if(target.getPosX()<mPosX)
                mVelX=-MOB_VEL;
            if(target.getPosX()==mPosX)
                mVelX=0;
            if(target.getPosY()>mPosY)
                mVelY=MOB_VEL;
            if(target.getPosY()<mPosY)
                mVelY=-MOB_VEL;
            if(target.getPosY()==mPosY)
                mVelY=0;
        }
    }
    
    void render(SDL_Rect & camera)
    {
        int t = Sprite;
        
        switch (t)
        {
            case 1:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    mobSprite.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
                
            case 2:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    theBoss.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
                
            default:
                if(checkCollision(camera, mCollider))
                {
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    //SDL_Rect data={mPosX-camera.x, mPosY-camera.y, mCollider.w, mCollider.h };
                    //SDL_RenderFillRect(gameRenderer, &data);
                    spriteSheets.render( mPosX-camera.x, mPosY-camera.y, &currentClip);
                }
                break;
        }
    }
    
    void checkRange(Player & target)
    {
        if((checkCollision(target.getmCollide(), range)))
            inRange=true;
    }
    
protected:
    static const int MOB_VEL = PLAYER_VEL/2;
    bool inRange;
    int Sprite;
    SDL_Rect range;
};

//Mob Vector
std::vector<Mob> mobVector;
std::vector<staticMob> staticmobVector;
std::vector<chaseMob> chasemobVector;

std::string mPath = "maps/mobs/Mobs1.mobs";

//Add Mob
void addMob(int x, int y, int h, int d, int s)
{
    Mob temp(x, y, h, d, s);
    mobVector.push_back(temp);
    mobVector[mobVector.size()-1].assignSpriteSheet();
    mobVector[mobVector.size()-1].setSpritInit();
}

void addstaticMob(int x, int y, int h, int d, int s)
{
    staticMob temp(x, y, h, d, s);
    staticmobVector.push_back(temp);
    staticmobVector[staticmobVector.size()-1].assignSpriteSheet();
    staticmobVector[staticmobVector.size()-1].setSpritInit();
}

void addchaseMob(int x, int y, int h, int d, int s)
{
    chaseMob temp(x, y, h, d, s);
    chasemobVector.push_back(temp);
    chasemobVector[chasemobVector.size()-1].assignSpriteSheet();
    chasemobVector[chasemobVector.size()-1].setSpritInit();
}

//Extract Data from mob file
void loadMobs(std::string path)
{
    std::ifstream ifs;
    ifs.open(path);
    
    if(ifs)
    {
        int t,x,y,h,d,s;
        while(!ifs.eof())
        {
            ifs>>t;
            ifs>>x;
            ifs>>y;
            ifs>>h;
            ifs>>d;
            ifs>>s;
            
            switch (t) {
                case 1:
                    addObject(t, x, y);
                    break;
                    
                case 2:
                    addMob(x, y, h, d, s);
                    break;
                    
                case 3:
                    addstaticMob(x, y, h, d, s);
                    break;
                    
                case 4:
                    addchaseMob(x, y, h, d, s);
                    break;
                    
                default:
                    break;
            }
            
            std::cout <<"mob created at (" << x <<", " << y << ")" <<std::endl;
        }
    }
}

//Kill Mob
void killMob(int i)
{
    mobVector.erase(mobVector.begin()+i);
}

void killstaticMob(int i)
{
    staticmobVector.erase(staticmobVector.begin()+i);
}

void killchaseMob(int i)
{
    chasemobVector.erase(chasemobVector.begin()+i);
}

//Clear Mob
void clearMobs()
{
    mobVector.clear();
}

void clearstaticMobs()
{
    staticmobVector.clear();
}

void clearchaseMobs()
{
    chasemobVector.clear();
}

#endif
