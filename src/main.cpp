#ifdef __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>


#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#endif

#include <iostream>
#include <string>
#include <vector>

#include "texture.h"
#include "player.h"
#include "tiles.h"
#include "menu.h"
#include "interface.h"


/*
 
 menue added
 
*/

Mix_Chunk *boom;
Mix_Chunk *deathScream;

    //Window
    SDL_Window *gameWindow = NULL;

/*  MAIN FUNCTIONS  */
    bool init();

    bool loadMedia();

    void close();

    void drawBox(int x, int y, int w, int h);

    void drawBox(SDL_Rect outliner);

    int prevTime = 0;
    int currentTime = 0;
    float deltaTime = 0;

/* !!!!!!!!!!!!!!!!!!!!!! */

int static speed = 0;
int static frame = 0;




//Initialize player
int players = 0;
Player one;
Player two;

Pot cheech(875, 720);

std::vector<Player> Ps;

void clearPlayers()
{
    Ps.clear();
}

void addPlayer(int i=0)
{
    if(i==0)
    {
        Ps.insert(Ps.begin(), one);
        Ps[0].setController(i);
        players++;
    }
    
    else
    {
        Ps.push_back(two);
        Ps[i].setController(i);
        players++;
    }
}

void removePlayer(int i)
{
    Ps.erase(Ps.begin()+i);
    --players;
}


/*
 
 pause menue
 
*/

Pause pauseMenu;
MainMenu mainMenu;
Death deathScreen;
HUD health;

void renderGame();


/*  Render  */

void renderGame(SDL_Rect &cam)
{
    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gameRenderer);
    
    
    //Render background first
    
    //Render player with current animation
    Ps[0].render(cam);
    
    //std::cout << Ps[0].getPosX() << std::endl;
    
    //Render text
    gTextTexture.render( (SCREEN_WIDTH - gTextTexture.getWidth())/2, (SCREEN_HEIGHT - gTextTexture.getHeight()) );
    
    //Display all in order
    SDL_RenderPresent(gameRenderer);
}

//void renderGamePaus

/*
 
 Bullet added
 
    added controller support
 
*/
//Game controllers
//init function will add all that are connected on startup
std::vector<SDL_Joystick*> gControllers;
std::vector<SDL_Haptic*> gHap;
//adds a controllers to the list
void addController(SDL_Event& e)
{
    gControllers.push_back(SDL_JoystickOpen(e.jdevice.which));
    SDL_Haptic* temp;
    temp = SDL_HapticOpenFromJoystick(gControllers[e.jdevice.which]);
    SDL_HapticRumbleInit(temp);
    gHap.push_back(temp);
    
}

void addController(int i)
{
    gControllers.push_back(SDL_JoystickOpen(i));
    SDL_Haptic* temp;
    temp = SDL_HapticOpenFromJoystick(gControllers[i]);
    SDL_HapticRumbleInit(temp);
    gHap.push_back(temp);
    std::cout << i <<std::endl;
}

//removes a controllers from vector
void removeController(SDL_Event& e)
{
    SDL_HapticClose(gHap[e.jdevice.which]);
    SDL_JoystickClose(gControllers[e.jdevice.which]);
    
    gHap.erase(gHap.begin() + e.jdevice.which);
    gControllers.erase(gControllers.begin() + e.jdevice.which);
    std::cout << e.jdevice.which <<std::endl;
}






/*  MAIN FUNCTIONS  */
    void close()
    {
        //Close font functions
        gTextTexture.free();
        TTF_CloseFont( gFont );
        gFont = NULL;
        
        //Free textures
        spriteSheets.free();
        tileSheet.free();
        pot.free();
        theBoss.free();
        health.free();
        
        //Destroy renderer
        SDL_DestroyRenderer(gameRenderer);
        gameRenderer = NULL;
        
        //Destroy window, free window pointer
        SDL_DestroyWindow(gameWindow);
        gameWindow = NULL;
        
        //Quit SDL Functions
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    bool init()
    {
        //Initialization flag
        bool success = true;
        
        //Initialize SDL
        if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_AUDIO) < 0 )
        {
            std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            std::cout << "SDL Video successfully loaded!" << std::endl;
            std::cout << "SDL Joystick successfully loaded!" << std::endl;
            
            //Set texture filtering to linear
            if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            {
                std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
            }
            
            //Create window
            gameWindow = SDL_CreateWindow("VIDEO GAME: THE MOVIE: THE GAME 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED);
            if( gameWindow == NULL )
            {
                std:: cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                std::cout << "Window successfully created!" << std::endl;
                
                //Create vsynced renderer for window
                gameRenderer = SDL_CreateRenderer( gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                if( gameRenderer == NULL )
                {
                    std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
                    success = false;
                }
                else
                {
                    std::cout << "Renderer successfully created!" << std::endl;
                    //Initialize renderer color
                    SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    
                    //Initialize PNG loading
                    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
                    if( !( IMG_Init( imgFlags ) & imgFlags ) )
                    {
                        std::cout << "SDL_image could not initialize! SDL_image Error: " <<  IMG_GetError() << std::endl;;
                        success = false;
                    }
                    
                    //Initialize SDL_tff
                    if (TTF_Init() == -1)
                    {
                        std::cout << "SDL_tff could not initialize! SDL_tff Error: " << TTF_GetError() << std::endl;
                        success = false;
                    }

                    //Initialize phat beats
                    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0)
                    {
                        std::cout << "SDL_mixer failed to initialize SDL_mixer Error: "<< TTF_GetError() << std::endl;
                        success = false;
                    }
                    
                    else
                    {
                        std::cout << "SDL Image and Text successfully loaded!" << std::endl;
                        std::cout << "INITIAL STARTUP SUCCESSFUL!" << std::endl;
                    }
                }
            }
        }
        
        if(SDL_NumJoysticks() <1)
        {
            std::cout <<"No Joystick" <<std::endl;
        }
        else
        {
            //adds connected controllers to vector
            for(int ii=0; ii<SDL_NumJoysticks(); ++ii)
            {
                addController(ii);
                
            }
        }
        
        return success;
    }

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Load/assign Tile Sheet
    if(!loadTileSet("images/tilesheet.png"))
    {
        std::cout << "Failed to load tileset\n" << std::endl;
        success = false;
    }
    //Load Music
    menuMusic = Mix_LoadMUS("sound/WWtheme.wav");
    if(menuMusic== NULL)
    {
        std::cout << "Failed to load menu music" << std::endl;
        success = false;
    }

    overworldTheme = Mix_LoadMUS("sound/overworld.wav");
    if(overworldTheme==NULL)
    {
        std::cout << "failed to load overworld theme" << std::endl;
        success = false;
    }

    flowerTheme= Mix_LoadMUS("sound/snakeEater.wav");
    if(flowerTheme == NULL)
    {
        std::cout <<"failed to load snake eater theme from file"<<std::endl;
        success=false;
    }

    pauseMusic= Mix_LoadMUS("sound/sabotage.wav");
    if(pauseMusic==NULL)
    {
        std::cout <<"faild to load sabotage.wav" <<std::endl;
        success = false;
    }
    if(!mobSprite.loadFromFile("images/sheet1.png"))
    {
        std::cout << "Failed loading mob texture\n"<<std::endl;
        success = false;
    }
    //Load sound effects
    boom= Mix_LoadWAV("sound/gun.wav");
    if(boom==NULL)
    {
        std::cout << "failed to load gun sound, it's late i'm out of witty shit to type" << std::endl;
        success =false;
    }
    boop= Mix_LoadWAV("sound/boop.wav");
    if(boop==NULL)
    {
        std::cout << "failed to boop, get bopped son" << std::endl;
        success =false;
    }
    deathScream= Mix_LoadWAV("sound/death.wav");
    if(deathScream==NULL)
    {
        std::cout << "failed to open death sound" << std::endl;
        success=false;
    }

    
    //Load dot texture
    if( !spriteSheets.loadFromFile("images/sheet2.png" ) )
    {
        std::cout << "Failed to load character texture!\n" << std::endl;
        success = false;
    }

    if(!pot.loadFromFile("images/maryjane.png"))
    {
        std::cout << "man getchu some pot textures maaaaaan\n"<< std::endl;
        success = false;
    }

    if(!theBoss.loadFromFile("images/theBoss.png"))
    {
        std::cout << "snake? snake? snaaaaaaaaaaaaaaaaaaaaaaaaake? failed loading boss texture\n"<<std::endl;
        success = false;
    }
    
     if(!fireball.loadFromFile("images/bullet.png"))
    {
        std::cout << "click, click, no bullets mang, go get some more and put them in your images file\n"<<std::endl;
        success =false;
    }
    
    //Load text
    gFont = TTF_OpenFont("fonts/RetGanon.ttf", 40);
    if( gFont == NULL)
    {
        std::cout << "Failed to load text font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        success = false;
    }
    
    else
    {
        //Render text
        SDL_Color textColor = { 0, 0, 0 };
        if(!gTextTexture.loadFromRenderedText("#HeartBreak Gorillas, 2015", textColor))
        {
            std::cout << "Failed to render text texture!" << std::endl;
            success = false;
        }
    }
    
    mainMenu.loadMenuText();
    pauseMenu.loadMenuText();
    deathScreen.loadMenuText();
    health.load();
    std::cout<<"all textures loaded successfully" << std::endl;
    return success;
}


//start function

void start()
{
    clearPlayers();
    //Load Map
    map = "maps/1.map";
    mPath = "maps/mobs/Mobs1.mobs";
    if(!loadMap(map.c_str()))
    {std::cout <<"failed loading map\n" << std::endl;}
    if(SDL_NumJoysticks() < 1)
    {
        addPlayer();
    }
    else
    {
        for(int i=0; i<SDL_NumJoysticks(); ++i)
        {
            addPlayer(i);
        }
    }
    
}

/* MAIN FUNCTION */
    int main(int argc, char* argv[])
    {
        //Initialize SDL
        if(!init())
        { std::cout << "UNABLE TO LOAD SDL SYSTEMS! ABANDON SHIP!!!!" << std::endl; }
        
        else
        {   //Load media and textures
            if(!loadMedia())
            { std::cout << "UNABLE TO LOAD GRAPHICS! NOTHING TO SEE HERE!" << std::endl; }
            
            // Initialize spritesheet and first animation
            else
            {
                one.assignSpriteSheet();
                one.setSpritInit();
                
                two.assignSpriteSheet();
                two.setSpritInit();
//
//                cheech.assignSpriteSheet();
//                cheech.setSpritInit();
                }
        }
        
        //Set quit flag
        bool quit = false;

        SDL_Rect camera = { ((LEVEL_WIDTH-SCREEN_WIDTH)/2), ((LEVEL_HEIGHT-SCREEN_HEIGHT)/2), SCREEN_WIDTH, SCREEN_HEIGHT };

        //Create event handler
        SDL_Event e;        
        
        
        //RUN PROGRAM
        while(!quit)
        {
            Mix_HaltMusic();
            
            //Run menu first
            while(mainMenu.getQuit() != true)
            {
                Mix_PlayMusic(menuMusic, -1);
                mainMenu.render();
                Mix_HaltMusic();
                    if(map=="maps/2.map")
                        {
                            Mix_HaltMusic();
                            Mix_PlayMusic(flowerTheme, -1);
                        }
                        else
                        {
                            Mix_PlayMusic(overworldTheme, -1);
                        }
                
                //if START GAME selected
                if(mainMenu.getRun() == true)
                {
                    //     }
                    //start game function
                    
                    std::cout<<"game ON" <<std::endl;
                    start();
                    //end start game function
                    
                    
                while(!quit)
                {
                    while(SDL_PollEvent(&e) != 0)
                    {
                        //If close window button is hit
                        if(e.type == SDL_QUIT)
                        {
                            quit = true;
                        }
                        else if(e.type == SDL_JOYDEVICEADDED)
                        {
                            addController(e);
                            std::cout <<e.jdevice.which <<" Contreoller added"<< std::endl;
                            if(e.jdevice.which != 0)
                            {
                                addPlayer(SDL_JoystickInstanceID(gControllers[gControllers.size()-1]));
                            }
                            
                        }
                        else if(e.type == SDL_JOYDEVICEREMOVED)
                        {
                            removeController(e);
                            removePlayer(e.jdevice.which);
                        }
                        else if(e.type == SDL_KEYDOWN || e.type == SDL_JOYBUTTONDOWN)
                        {
                            std::cout <<e.jbutton.which <<std::endl;
                            if(e.key.keysym.sym == SDLK_ESCAPE || e.jbutton.button == 10)
                            {
                                for(int i=0; i<Ps.size(); ++i)
                                {
                                   Ps[i].stop();
                                }
                                Mix_HaltMusic();
                                Mix_PlayMusic(pauseMusic, -1);
                                //Run Pause Menu
                                pauseMenu.renderPause();
                                quit = pauseMenu.getQuit();
                                Mix_HaltMusic();
                                if(map=="maps/2.map")
                                {
                                    Mix_HaltMusic();
                                    Mix_PlayMusic(flowerTheme, -1);
                                }
                                else
                                {
                                    Mix_PlayMusic(overworldTheme, -1);
                                }

                            }
                        }
                        else{
                        }
                        //Handle main character input
                        for(int i=0; i<Ps.size(); ++i)
                        {
                            Ps[i].handleEvent(e);
                            for(int j=0; j<Os.size(); ++j)
                            {
                                Os[j].getPickedUp(e, Ps[i]);
                            }
                        }
                        
                        
                    //Handle main character input
                    }
                    
                    for(int i=0; i<Ps.size(); ++i)
                    {
                        if(Ps[i].die())
                        {
                            Mix_PlayChannel(-1, deathScream, 0);
                            //Run dying animation
                            deathScreen.deathAnimation(Ps[i], &camera);
                            
                            //CRASHES IF NO CONTROLLER IS CONNECTED
                                    //FIXED
                            if(gHap.size()> i)
                            {
                               SDL_HapticRumblePlay(gHap[i], 10, 500);
                            }
                            
                            //Erase player data and push character vector
                            Ps.erase(Ps.begin()+i);
                            
                            
                            if(Ps.size()<1)
                            {
                                quit = true;
                                deathScreen.renderDeath();
                                clearchaseMobs();
                                clearMobs();
                                clearObjects();
                                clearstaticMobs();
                            }
                            
                            // LOAD INTO DEATH MENU?
                        }
                    }
                    
                    for(int i=0; i<mobVector.size(); ++i)
                    {
                        if(mobVector[i].isDead())
                        {
                            killMob(i);
                        }
                    }
                    //STATIC MOBS
                    for(int i=0; i<staticmobVector.size(); ++i)
                    {
                        if(staticmobVector[i].isDead())
                        {
                            killstaticMob(i);
                        }
                    }
                    //CHASE MOBS
                    for(int i=0; i<chasemobVector.size(); ++i)
                    {
                        if(chasemobVector[i].isDead())
                        {
                            killchaseMob(i);
                        }
                    }
                    
                    ++frame;
                    speed = frame/8;
                    
                    if(speed >= 3)
                    {
                        frame = 0;
                        speed = 0;
                    }
                   
                    //Move main character
                    for(int i=0; i<Ps.size(); ++i)
                    {
                        Ps[i].move(&speed);
                        checkAll(Ps[i]);
                        Ps[i].setCam(camera);
                    }


                    for(int i=0; i<Os.size();++i)
                    {
                        Os[i].follow();
                    

                        Os[i].move(&speed);
                    }
                
                    //MOBS
                    for(int i=0; i<mobVector.size(); ++i)
                    {
                        
                        mobVector[i].move(&speed);
                        for(int j=0; j<Ps.size(); ++j)
                        {
                            playerCollide(mobVector[i], Ps[j]);
                            mobVector[i].behavior(Ps[j]);
                            mobVector[i].checkRange(Ps[j]);
                        }
                        checkAll(mobVector[i]);
                    }
                    
                    //STATIC MOBS
                    for(int i=0; i<staticmobVector.size(); ++i)
                    {
                        
                        staticmobVector[i].move(&speed);
                        for(int j=0; j<Ps.size(); ++j)
                        {
                            playerCollide(staticmobVector[i], Ps[j]);
                            staticmobVector[i].behavior(Ps[j]);
                        }
                        checkAll(staticmobVector[i]);
                    }
                    
                    //CHASE MOBS
                    for(int i=0; i<chasemobVector.size(); ++i)
                    {
                        
                        chasemobVector[i].move(&speed);
                        for(int j=0; j<Ps.size(); ++j)
                        {
                            playerCollide(chasemobVector[i], Ps[j]);
                            chasemobVector[i].behavior(Ps[j]);
                            chasemobVector[i].checkRange(Ps[j]);
                        }
                        checkAll(chasemobVector[i]);
                    }
                    //Clear screen
                    SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gameRenderer);
                    
                    //Tile collision checks
                    
                    
                    //Render background first
                    BuildWorld(camera);
                    


                    

                    //Render Player Objects Mobs
                    for(int i=0; i<Ps.size(); ++i)
                    {
                        Ps[i].render(camera);
                    }
                        
                    for(int i=0;i<Os.size();++i)
                    {
                        Os[i].render(camera);
                    }
                    
                    //MOBS
                    for(int i=0; i<mobVector.size(); ++i)
                    {
                        mobVector[i].render(camera);
                    }
                    
                    //STATIC MOBS
                    for(int i=0; i<staticmobVector.size(); ++i)
                    {
                        staticmobVector[i].render(camera);
                    }
                    
                    //CHASE MOBS
                    for(int i=0; i<chasemobVector.size(); ++i)
                    {
                        chasemobVector[i].render(camera);
                    }

                    
                    health.render(Ps[0]);
                    
                    //Render text
                    gTextTexture.render( (SCREEN_WIDTH - gTextTexture.getWidth())/2, (SCREEN_HEIGHT - gTextTexture.getHeight()) );
                    
                    //Display all in order
                    SDL_RenderPresent(gameRenderer);
                    
                    }
                }
                
                //Quit back to main menu
                quit = false;
                mainMenu.setRun(false);
                if(deathScreen.getQuit())
                {
                    mainMenu.setQuitTrue();
                }
                std::cout << "Quit Game" << std::endl;
            }

        /*  END GAME    */
    
        close();
        
        return 0;
        
        }
    }
