#ifdef __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>


#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif


#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <iostream>
#include <vector>

using std::vector;

class HUD
{
    public:

        //Constructor
        HUD()
        {};

        //Destructor
        ~HUD()
        {};
    
        /* ACCESSORS / MUTATORS */
    
            int getMaxHealth()
            { return MAX_HEALTH; }
    
            void setMaxHealth(int max)
            { MAX_HEALTH = max;}
    
        /* RENDER */
    
            //Load Hearts
            void load();
            
            //Render Hearts
            void render(Player & player);
    
        /*  CLOSING */
    
            //Free textures
            void free();

    private:
    
        //Holds max health for heart counter
        int MAX_HEALTH;

        //Load heart textures
        LTexture hearts;
        
        //Life text
        LTexture life;
};

//Holds heart state animations
vector<SDL_Rect> heartState(4);

void HUD::free()
{
    //Free textures
    hearts.free();
    life.free();
    
}
//Load and assign heart sprite sheet and text
void HUD::load()
{
    //Load heart spritesheet
    hearts.loadFromFile("images/health.png");
    
    //Declare font color
    SDL_Color textColor = {0xFF, 0xFF, 0xFF};
    
    //Load text
    life.loadFromRenderedText(" ----- LIFE ----- " , textColor);
    
    //Assign Sprites
    for(int i = 0; i < 5; ++i)
    {
        heartState[i].x = 32*i;
        heartState[i].y = 0;
        heartState[i].h = 32;
        heartState[i].w = 32;
    }
    
}

// Render Hearts
void HUD::render(Player & player)
{
    //Set max health
    setMaxHealth(player.getMaxHealth());
    
    //Make vector of hearts with enough for max health
    vector <SDL_Rect> currentState(getMaxHealth()/4);
    
    //Find current health
    int currentHealth = player.getHealth();
    
    // Declare amount of hearts needed
    life.render( 30, 30);
    
    //Go through each heart
    for(int i = 0; i < (getMaxHealth()/4); ++i)
    {
        //Full heart
        if ((currentHealth - (4*i)) >= 4)
        {   currentState[i] = heartState[4];    }
        
        //Three-quarters heart
        else if ((currentHealth - (4*i)) == 3)
        {   currentState[i] = heartState[3];    }
        
        //Half Heart
        else if ((currentHealth - (4*i)) == 2)
        {   currentState[i] = heartState[2];    }
        
        //Quarter Heart
        else if ((currentHealth - (4*i)) == 1)
        {   currentState[i] = heartState[1];    }
        
        //Empty Heart
        else
        {   currentState[i] = heartState[0];    }

        //Render hearts using assigned states
        hearts.render( 30 + (35*i), (life.getHeight()+32), &currentState[i]);
    }
    

}

#endif


