#ifdef __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#endif

#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <iostream>
#include <vector>
#include "texture.h"
#include "player.h"

using std::vector;

Mix_Music *menuMusic;
Mix_Music *pauseMusic;

Mix_Chunk *boop;
//Menu Class
class Menu
{
public:
    
    //Default menu constructor
    Menu():canMove(true)
    {};
    
    //Default menu deconstructor
    ~Menu()
    {};
    
    //Set background width
    virtual void setBackgroundWidth(int w) = 0;
    
    //Set background height
    virtual void setBackgroundHeight(int h) = 0;
    
    //Render background over game
    virtual void setBackground(int w, int h) = 0;
    
protected:
    //Screen dimensions
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    
    //controls controller input for menus
    bool canMove;
    
    //Background when menu runs
    SDL_Rect background;
    SDL_Texture *backgroundScreen_;
    Mix_Music *menuMusic;
    
};

/**************/
/* INTRO MENU */
/**************/

class IntroScreen: public Menu
{
    IntroScreen()
    {};
    
    ~IntroScreen()
    {};
    
    //Set background width
    virtual void setBackgroundWidth(int w) = 0;
    
    //Set background height
    virtual void setBackgroundHeight(int h) = 0;
    
    //Render background over game
    virtual void setBackground(int w, int h) = 0;
    
    //Render logo
    void renderIntro();
    
    //Load text prompt
    void loadText();

    //Render text prompt
    void renderText();
    
    //Have logo on screen for a few seconds
    void checkWait();
    
    //Continue prompt
    void continuePrompt();
    
    //Set advance flag
    void setFlag(bool flag);
    
    //Check for input to next menu
    void handleEvent(SDL_Event &e);
    
    //Free items
    void free();
    
private:
    
    bool advance;
    
    int wait;
    
    LTexture advanceText;
    
};



/*************/
/* MAIN MENU */
/*************/

class MainMenu : public Menu
{
public:
    
    //Constructor
    MainMenu():Menu()
    {};
    
    //Deconstructor
    ~MainMenu()
    {};
    
    /*  BACKGROUND  */
    
        //Render background over game
        void setBackground(int w, int h);
        
        //Set background width
        void setBackgroundWidth(int w)
        {   background.w = w; };
        
        //Set background height
        void setBackgroundHeight(int h)
        {   background.h = h; };
    
    /* TEXT */
    
        //Set menu text
        void loadMenuText();
        
        //Render Text
        void renderText();
        
        //Render Pause menu
        void render();
    
    /*  MENU SELECTION  */
    
        //Handle event
        void handleEvent(SDL_Event &e);
        
        //Selection Up
        void selectUp();
        
        //Selection Down
        void selectDown();
        
        //Run event
        void runEvent();
        
        //Draw outlining box
        void drawBox(int i);
    
    /*  FLAG CHECKS */
    
        //Flag to start running game
        void setRun(bool flag)
        { run = flag;}

        //Check flag to start running game
        bool getRun()
        { return run;}

        //Set quit bool
        void setQuitTrue()
        { quit = true; }

        //Get quit bool
        bool getQuit()
        { return quit; }
    
    
    /*  CLOSING  */
    
        //Free data
        void free();
    
private:
    /*  BOOL FLAGS  */
    
        //Escape flag
        bool escape;
        
        //Quit flag
        bool quit;
        
        //Run flag
        bool run;
    
    /*  TEXT TEXTURES   */
    
        LTexture startText;
        LTexture quitText;
    
    /*  IMAGE TEXTURES  */
    
        LTexture logo;
    
    /*  VARIABLES   */
    
        //Selection indicator
        int selection;
    
};


//Vector that holds main menu items
vector<LTexture* > mainMenuItems(3);

/*  MAIN MENU FUNCTIONS   */

    //Set background rect size
    void MainMenu::setBackground(int w, int h)
    {
        background.w = w;
        background.h = h;
    }

    void MainMenu::loadMenuText()
    {
        
        //Load Logo
        logo.loadFromFile("images/teamLogo.png");
        
        //Declare font color
        SDL_Color textColor = {0xFF, 0xFF, 0xFF};
        
        //Create texts
        startText.loadFromRenderedText("START NEW GAME", textColor);
        quitText.loadFromRenderedText("QUIT", textColor);
        
        //Put text into menu item
        mainMenuItems[0] = (&startText);
        mainMenuItems[1] = (&quitText);
        
        logo.setHeight(logo.getHeight());
        logo.setWidth(logo.getWidth());
        
        
    };

    void MainMenu::renderText()
    {
        
        for(int i = 0; i <=1 ; ++i)
        {
            mainMenuItems[i]->render(((SCREEN_WIDTH - mainMenuItems[i]->getWidth()) /2), (SCREEN_HEIGHT-(SCREEN_HEIGHT/3))+(64*i));
        }
        
    }

    //Free data
    void MainMenu::free()
    {
        
        //Destroy SDL Textures
        SDL_DestroyTexture(backgroundScreen_);
        backgroundScreen_ = nullptr;
        
        startText.free();
        quitText.free();
        
    }

    void MainMenu::drawBox(int i)
    {
        //Inner Box
        SDL_Rect outline;
        outline.x = ((SCREEN_WIDTH - mainMenuItems[i]->getWidth())/ 2)-10;
        outline.y = ((SCREEN_HEIGHT-(SCREEN_HEIGHT/3))+(64*i));
        outline.w = mainMenuItems[i]->getWidth() +12;
        outline.h = mainMenuItems[i]->getHeight();
        SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 200);
        SDL_RenderDrawRect(gameRenderer, &outline);
        
        //Outer Box
        outline.x = ((SCREEN_WIDTH - mainMenuItems[i]->getWidth())/ 2)-15;
        outline.y = ((SCREEN_HEIGHT-(SCREEN_HEIGHT/3))+(64*i)-5);
        outline.w = mainMenuItems[i]->getWidth()+22;
        outline.h = mainMenuItems[i]->getHeight()+10;
        SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 200);
        SDL_RenderDrawRect(gameRenderer, &outline);
    }

    //Select down
    void MainMenu::selectDown()
    {
        if(selection != 1)
        {   ++selection;   Mix_PlayChannel(-1, boop, 0); }
        
        //If greater than number of options
        //SHOULD HAVE DECLARED VALUE FOR NUMBER OF OBJECTS
        else
        { selection = 0; Mix_PlayChannel(-1, boop, 0);}
    }

    //Select Up
    void MainMenu::selectUp()
    {
        if(selection != 0)
        {   --selection;   Mix_PlayChannel(-1, boop, 0); }
        
        //If less than number of options
        //SHOULD HAVE DECLARED VALUE FOR NUMBER OF OBJECTS
        else
        { selection = 1; Mix_PlayChannel(-1, boop, 0);}
    }

    void MainMenu::handleEvent(SDL_Event &e)
{
    //If hit quit, back out
    if(e.type == SDL_QUIT)
    {
        escape = true;
    }
    
    //Escape
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
        escape = true;
    }
    
    //Up
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP)
    {   selectUp(); }
    
    //Down
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN)
    {   selectDown(); }
    
    //Enter (Return)
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
    {   runEvent(); }
    
    if(e.type == SDL_JOYAXISMOTION && e.jaxis.axis == 1 && e.jaxis.value >= 30000 && canMove)
    {
        selectDown();
        canMove=false;
    }
    else if (e.type == SDL_JOYAXISMOTION && e.jaxis.axis == 1 && e.jaxis.value <= -30000 && canMove)
    {
        selectUp();
        canMove=false;
        std::cout<<"MENU ACTION controller!";
    }
    else if (e.type == SDL_JOYAXISMOTION && e.jaxis.axis == 1 && e.jaxis.value <= 5000 && e.jaxis.value >= -5000 )
    {
        canMove = true;
    }
    
    if(e.type == SDL_JOYBUTTONDOWN && e.jbutton.button == 11)
    {
        selectUp();
    }
    if(e.type == SDL_JOYBUTTONDOWN && e.jbutton.button == 12)
    {
        selectDown();
    }
    if(e.type == SDL_JOYBUTTONDOWN && e.jbutton.button == 0)
    {
        runEvent();
    }
    
}

    //Run event based on
    void MainMenu::runEvent()
    {
        switch (selection)
        {
            //Run game
            case 0:
                setRun(true);
                escape = true;
                break;
            
            //Quit game
            //Allows to break out of loop and run shutdown function
            case 1:
                //Set quit flag
                setQuitTrue();
                escape = true;
                break;
        }
    }

    //Render Main Menu
    void MainMenu::render()
    {
        //initialize screen width and height variables
        SDL_GetRendererOutputSize(gameRenderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);
        
        //Set initial flags
        escape = false;
        run = false;
        
        //Initialize event handler
        SDL_Event b;
        
        //Declare background rect to screen size
        setBackground(SCREEN_WIDTH, SCREEN_HEIGHT);
        
        //Set blend mode for SDL
        SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
        
        //While running
        while(!escape)
        {
            //If there is an SDL_Event
            while(SDL_PollEvent(&b) != 0)
            {
                handleEvent(b);
            }
            
            //Clear screen
            SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gameRenderer);
            
            //Set draw color for transparent screen
            SDL_SetRenderDrawColor(gameRenderer, 100, 100, 100, 0xFF);
            
            //Send transparent screen to renderer
            SDL_RenderFillRect(gameRenderer, &background);
            
            //LOAD MENU LOGO
            logo.render((SCREEN_WIDTH-logo.getWidth())/2, 50);
            
            //Render pause text
            renderText();
            
            //Selection box
            drawBox(selection);
            
            //Render All
            SDL_RenderPresent(gameRenderer);
        }
        
    };


/**************/
/* PAUSE MENU */
/**************/
class Pause : public Menu
{
    
public:
    
    //Create pause overlay
    Pause()
    {};
    
    //Deconstructor
    ~Pause()
    {};
    
    /*  BACKGROUND  */
        //Save screenshot of game
        void saveScreen();
    
        //Render background over game
        void setBackground(int w, int h);

        //Set background width
        void setBackgroundWidth(int w)
        {   background.w = w; };

        //Set background height
        void setBackgroundHeight(int h)
        {   background.h = h; };
    
    /* TEXT */
    
        //Set menu text
        void loadMenuText();
        
        //Render Text
        void renderText();
        
        //Render Pause menu
        void renderPause();
    
    /*  MENU SELECTION  */
        
        //Handle event
        void handleEvent(SDL_Event &e);
        
        //Selection Up
        void selectUp();
        
        //Selection Down
        void selectDown();
        
        //Run event
        void runEvent();
        
        //Draw outlining box
        void drawBox(int i);
    
    /*  CLOSING  */
    
        //Set quit bool
        void setQuitTrue()
        { quit = true; }
    
        //Get quit bool
        bool getQuit()
        { return quit; }
    
        //Free data
        void free();
    
private:
    
    //Text Textures
    LTexture pauseText;
    LTexture resumeText;
    LTexture quitText;
    LTexture settingText;
    
    //Pointer
    SDL_Surface *pointer;
    
    //selection
    int selection;
    
    //escape flag
    bool escape;
    bool quit;
};

//Vector holding pauseMenu text textures
vector <LTexture*> menuItems(3);

//Render the pause menu
void Pause::renderPause()
{
    SDL_GetRendererOutputSize(gameRenderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    //Setvariables
    escape = false;
    SDL_Event b;
    selection = 0;
    
    //Declare background rect to screen size
    setBackground(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //Set blend mode for SDL
    SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
    
    //Save a screenshot of the screen as a BMP to load behind screen
    saveScreen();
    
    //Running
    while(!escape)
    {
        //If there is an SDL_Event
        while(SDL_PollEvent(&b) != 0)
        {
            handleEvent(b);
        }
        
        //Clear screen
        SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gameRenderer);
        
        //Send background screenshot to renderer
        SDL_RenderCopy(gameRenderer, backgroundScreen_, NULL, &background);
        
        //Set draw color for transparent screen
        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 150);
        
        //Send transparent screen to renderer
        SDL_RenderFillRect(gameRenderer, &background);
        
        //Render pause text
        renderText();
        
        //Selection box
        drawBox(selection);
        
        //Render All
        SDL_RenderPresent(gameRenderer);
    }
    
    //Destroy SDL Textures
    SDL_DestroyTexture(backgroundScreen_);
    backgroundScreen_ = nullptr;
    
};

//Set background rect size
void Pause::setBackground(int w, int h)
{
    background.w = w;
    background.h = h;
}

//Take picture of screen to place underneath
void Pause::saveScreen()
{
    // Create surface
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    
    //Read surface
    SDL_RenderReadPixels(gameRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    
    //Save surface
    //SDL_SaveBMP(sshot, "images/pauseBackground.bmp");
    //SDL_FreeSurface(sshot);
    
    //Load surface to texture
    //sshot = SDL_LoadBMP("images/pauseBackground.bmp");
    backgroundScreen_ = SDL_CreateTextureFromSurface(gameRenderer, sshot);
    
    //Free surface
    SDL_FreeSurface(sshot);
    
};

void Pause::loadMenuText()
{
    //Declare font color
    SDL_Color textColor = {0xFF, 0xFF, 0xFF};
    
    //Create texts
    pauseText.loadFromRenderedText("PAUSED", textColor);
    resumeText.loadFromRenderedText("RESUME", textColor);
    settingText.loadFromRenderedText("SETTINGS", textColor);
    quitText.loadFromRenderedText("QUIT", textColor);
    
    //Put text into menu item
    menuItems[0] = (&resumeText);
    menuItems[1] = (&settingText);
    menuItems[2] = (&quitText);

    // Pointer for menu select
    // Currently using box around selected item
    pointer = SDL_LoadBMP("images/pointer.bmp");
    
};

void Pause::renderText()
{
    //Render text "PAUSED" at top
    pauseText.render( (SCREEN_WIDTH - pauseText.getWidth())/2, SCREEN_HEIGHT/7);
    
    //Render selection menu items
    for(int i = 0; i < 3; ++i)
    {
        menuItems[i]->render(((SCREEN_WIDTH - menuItems[i]->getWidth()) /2), (SCREEN_HEIGHT/3)+(64*i));
    }

}

//Free data
void Pause::free()
{

    //Destroy SDL Textures
    SDL_DestroyTexture(backgroundScreen_);
    backgroundScreen_ = nullptr;
    
    //Free text textures
    pauseText.free();
    resumeText.free();
    quitText.free();
    
    //Free pointer
    //NOT USED, USING BOXES FOR SELECTION AT THE MOMENT
    SDL_FreeSurface(pointer);
    pointer = nullptr;
    
}

void Pause::drawBox(int i)
{
    //Inner Box
    SDL_Rect outline;
    outline.x = ((SCREEN_WIDTH - menuItems[i]->getWidth())/ 2)-10;
    outline.y = ((SCREEN_HEIGHT/3)+(64*i));
    outline.w = menuItems[i]->getWidth() +12;
    outline.h = menuItems[i]->getHeight();
    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 200);
    SDL_RenderDrawRect(gameRenderer, &outline);
    
    //Outer Box
    outline.x = ((SCREEN_WIDTH - menuItems[i]->getWidth())/ 2)-15;
    outline.y = ((SCREEN_HEIGHT/3)+(64*i)-5);
    outline.w = menuItems[i]->getWidth()+22;
    outline.h = menuItems[i]->getHeight()+10;
    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 200);
    SDL_RenderDrawRect(gameRenderer, &outline);
}

//Select down
void Pause::selectDown()
{
    if(selection != 2)
    {   ++selection;  Mix_PlayChannel(-1, boop, 0);  }
    
    //If greater than number of options
    //SHOULD HAVE DECLARED VALUE FOR NUMBER OF OBJECTS
    else
    { selection = 0; Mix_PlayChannel(-1, boop, 0);}
}

//Select Up
void Pause::selectUp()
{
    //If not last number
    if(selection != 0)
    {   --selection;    Mix_PlayChannel(-1, boop, 0);}
    
    //If less than number of options
    //SHOULD HAVE DECLARED VALUE FOR NUMBER OF OBJECTS
    else
    { selection = 2; Mix_PlayChannel(-1, boop, 0);}
}

void Pause::handleEvent(SDL_Event &e)
{
    //If hit quit, back out
    if(e.type == SDL_QUIT)
    {
        escape = true;
    }
    
    //Escape
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
        escape = true;
    }
        
    //Up
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP)
    {   selectUp(); }
    
    //Down
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN)
    {   selectDown(); }
    
    //Enter (Return)
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
    {   runEvent(); }
    
    if(e.type == SDL_JOYAXISMOTION && e.jaxis.axis == 1 && e.jaxis.value >= 30000 && canMove)
    {
        selectDown();
        canMove=false;
    }
    else if (e.type == SDL_JOYAXISMOTION && e.jaxis.axis == 1 && e.jaxis.value <= -30000 && canMove)
    {
        selectUp();
        canMove=false;
        std::cout<<"MENU ACTION controller!";
    }
    else if (e.type == SDL_JOYAXISMOTION && e.jaxis.axis == 1 && e.jaxis.value <= 5000 && e.jaxis.value >= -5000 )
    {
        canMove = true;
    }
    
    if(e.type == SDL_JOYBUTTONDOWN && e.jbutton.button == 11)
    {
        selectUp();
    }
    if(e.type == SDL_JOYBUTTONDOWN && e.jbutton.button == 12)
    {
        selectDown();
    }
    if(e.type == SDL_JOYBUTTONDOWN && e.jbutton.button == 0)
    {
        runEvent();
    }
    
}

//Run event based on
void Pause::runEvent()
{
    switch (selection)
    {
        //Return to game
        case 0:
            quit = false;
            escape = true;
            Mix_HaltMusic();
            break;
          
        //Settings
        case 1:
            //Run Setting screen
            break;
            
        //Quit to main menu
        case 2:
            //Set quit flag
            setQuitTrue();
            Mix_HaltMusic();
            //Escape
            escape = true;
            break;
            
    }
}

/**************/
/* DEATH MENU */
/**************/

class Death: public Menu
{
    
public:
    
    //Create pause overlay
    Death()
    {};
    
    //Deconstructor
    ~Death()
    {};
    
    /*  BACKGROUND  */
    
    //Render background over game
    void setBackground(int w, int h);
    
    //Set background width
    void setBackgroundWidth(int w)
    {   background.w = w; };
    
    //Set background height
    void setBackgroundHeight(int h)
    {   background.h = h; };
    
    /* Death Animation */
    
    //Run death spin animation
    void deathAnimation(Player & player, SDL_Rect * camera);
    
    /* TEXT */
    
    //Set menu text
    void loadMenuText();
    
    //Render Text
    void renderText();
    
    //Render Death menu
    void renderDeath();
    
    /*  MENU SELECTION  */
    
    //Handle event
    void handleEvent(SDL_Event &e);
    
    //Selection Up
    void selectUp();
    
    //Selection Down
    void selectDown();
    
    //Run event
    void runEvent();
    
    //Draw outlining box
    void drawBox(int i);
    
    /*  CLOSING  */
    
    //Set quit bool
    void setQuitTrue()
    { quit = true; }
    
    //Get quit bool
    bool getQuit()
    { return quit; }
    
    //Free data
    void free();
    
private:
    
    //Text Textures
    LTexture youDiedText;
    LTexture newGameText;
    LTexture quitText;
    
    //selection
    int selection;
    
    //escape flag
    bool escape;
    bool quit;
    
    //Timer
    Uint32 mStartTicks;
};

//Vector holding death screen text textures
const int LIST_SIZE = 2;
vector <LTexture*> deathScreenItems(LIST_SIZE);


//Death Animation
//Takes in Player object and camera info from main function
void Death::deathAnimation(Player & player, SDL_Rect * camera)
{
    //timer
    mStartTicks = SDL_GetTicks();
    
    int animSprite = 0;
    
    bool complete = false;
    
    while(!complete)
    {
        /* SET BACKGROUND */
            if((SDL_GetTicks() - mStartTicks)/1000.f <= 3)
            {
                
                //Red while dying
                SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0, 0, 0xFF);
            }
            
            else
            {
                //Black when dead
                SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 0xFF);
            }
    
        /* PREP RENDERER */
        
            //Clear renderer
            SDL_RenderClear(gameRenderer);
            
            //Set background to one color
            SDL_RenderFillRect(gameRenderer, &background);
        
        /*  ANIMATIONS  */
        
            //Short pause for 2 seconds
            if( (SDL_GetTicks() - mStartTicks)/1000.f < 1)
            {
                player.render(* camera);
            }
            
            //Spin for 3 seconds
            else if( (SDL_GetTicks() - mStartTicks)/1000.f >= 1 && (SDL_GetTicks() - mStartTicks)/1000.f < 3)
            {
                
                //Set sprite spin
                player.setSpriteDeath(animSprite/5);
                
                //Render character spinning
                player.render(* camera);
                
                //Change spin state
                animSprite++;
                
                //Rotate through the up, down, left, right
                if(animSprite > 20)
                {
                    animSprite = 0;
                }
                
                //PLAY SPINNING MUSIC?
            }
            
            //Show death sprite for 3 seconds
            else if((SDL_GetTicks() - mStartTicks)/1000.f >= 3 && (SDL_GetTicks() - mStartTicks)/1000.f < 5)
            {
                player.setSpriteDeath(4);
                player.render(* camera);

            }
        
        /* QUIT ANIMATION LOOP */
            else if((SDL_GetTicks() - mStartTicks)/1000.f >= 5)
            {
                complete = true;
            }
        
        SDL_RenderPresent(gameRenderer);
    }
}

//Render the death screen
void Death::renderDeath()
{
    SDL_GetRendererOutputSize(gameRenderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    
    //Setvariables
    escape = false;
    SDL_Event e;
    selection = 0;
    
    //Declare background rect to screen size
    setBackground(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //Set blend mode for SDL
    SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
    
    //Running
    while(!escape)
    {
        //If there is an SDL_Event
        while(SDL_PollEvent(&e) != 0)
        {
            handleEvent(e);
        }
        
        
        //Set draw color for death screen
        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 0xFF);
        
        //Clear Screen
        SDL_RenderClear(gameRenderer);
        
        //Send death screen to renderer
        SDL_RenderFillRect(gameRenderer, &background);
        
        //Render pause text
        renderText();
        
        //Selection box
        drawBox(selection);
        
        //Render All
        SDL_RenderPresent(gameRenderer);
    }
    
    
};

//Set background rect size
void Death::setBackground(int w, int h)
{
    background.w = w;
    background.h = h;
}

void Death::loadMenuText()
{
    //Declare font color
    SDL_Color textColor = {0xFF, 0xFF, 0xFF};
    
    //Create texts
    youDiedText.loadFromRenderedText("YOU DIED", textColor);
    newGameText.loadFromRenderedText("RETURN TO MENU", textColor);
    quitText.loadFromRenderedText("QUIT", textColor);
    
    //Put text into menu item
    deathScreenItems[0] = (&newGameText);
    deathScreenItems[1] = (&quitText);
    
};

void Death::renderText()
{
    //Render text "PAUSED" at top
    youDiedText.render( (SCREEN_WIDTH - youDiedText.getWidth())/2, SCREEN_HEIGHT/7);
    
    //Render selection menu items
    for(int i = 0; i < LIST_SIZE; ++i)
    {
        deathScreenItems[i]->render(((SCREEN_WIDTH - deathScreenItems[i]->getWidth()) /2), (SCREEN_HEIGHT/3)+(64*i));
    }
    
}

//Free data
void Death::free()
{
    
    //Destroy SDL Textures
    SDL_DestroyTexture(backgroundScreen_);
    backgroundScreen_ = nullptr;
    
    //Free text textures
    youDiedText.free();
    newGameText.free();
    quitText.free();
    
}

void Death::drawBox(int i)
{
    //Inner Box
    SDL_Rect outline;
    outline.x = ((SCREEN_WIDTH - deathScreenItems[i]->getWidth())/ 2)-10;
    outline.y = ((SCREEN_HEIGHT/3)+(64*i));
    outline.w = deathScreenItems[i]->getWidth() +12;
    outline.h = deathScreenItems[i]->getHeight();
    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 200);
    SDL_RenderDrawRect(gameRenderer, &outline);
    
    //Outer Box
    outline.x = ((SCREEN_WIDTH - deathScreenItems[i]->getWidth())/ 2)-15;
    outline.y = ((SCREEN_HEIGHT/3)+(64*i)-5);
    outline.w = deathScreenItems[i]->getWidth()+22;
    outline.h = deathScreenItems[i]->getHeight()+10;
    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 200);
    SDL_RenderDrawRect(gameRenderer, &outline);
}

//Select down
void Death::selectDown()
{
    if(selection != LIST_SIZE-1)
    {   ++selection;   Mix_PlayChannel(-1, boop, 0); }
    
    //If greater than number of options
    //SHOULD HAVE DECLARED VALUE FOR NUMBER OF OBJECTS
    else
    { selection = 0; Mix_PlayChannel(-1, boop, 0);}
}

//Select Up
void Death::selectUp()
{
    //If not last number
    if(selection != 0)
    {   --selection;    Mix_PlayChannel(-1, boop, 0);}
    
    //If less than number of options
    //SHOULD HAVE DECLARED VALUE FOR NUMBER OF OBJECTS
    else
    { selection = LIST_SIZE-1; Mix_PlayChannel(-1, boop, 0); }
}

void Death::handleEvent(SDL_Event &e)
{
    //If hit quit, back out
    if(e.type == SDL_QUIT)
    {
        escape = true;
    }
    
    //If push key
    if(e.type == SDL_KEYDOWN)
    {
        //Escape
        if(e.key.keysym.sym == SDLK_ESCAPE)
        {
            escape = true;
        }
        
        //Up
        if(e.key.keysym.sym == SDLK_UP)
        {   selectUp(); }
        
        //Down
        else if(e.key.keysym.sym == SDLK_DOWN)
        {   selectDown();   Mix_PlayChannel(-1, boop, 0); }
        
        //Enter (Return)
        else if(e.key.keysym.sym == SDLK_RETURN)
        {   runEvent(); }
    }
    if(e.type == SDL_JOYAXISMOTION)
    {
        if(e.jaxis.axis == 1)
        {
            if(e.jaxis.value >= 30000 && canMove)
            {
                selectDown();
                canMove=false;
            }
            else if (e.jaxis.value <= -30000 && canMove)
            {
                selectUp();
                canMove=false;
                std::cout<<"MENU ACTION controller!";
            }
            else if (e.jaxis.value <= 5000 && e.jaxis.value >= -5000 )
            {
                canMove = true;
            }
        }
    }
    if(e.type == SDL_JOYBUTTONDOWN)
    {
        if(e.jbutton.button == 11)
        {
            selectUp();
        }
        if(e.jbutton.button == 12)
        {
            selectDown();
        }
        if(e.jbutton.button == 0)
        {
            runEvent();
        }
    }
}

//Run event based on
void Death::runEvent()
{
    
    switch (selection)
    {
            //Return to game
        case 0:
            quit = false;
            escape = true;
            break;
            
        //Quit game
        case 1:
            //Set quit flag
            setQuitTrue();
            
            //Escape
            
            escape = true;
            break;
            
    }
}


#endif