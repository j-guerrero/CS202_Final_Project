#ifndef FRAMELIMIT_H_INCLUDED
#define FRAMELIMIT_H_INCLUDED

#include <SDL2/SDL.h>

//The game logic and animations advance once per rendered frame, so the frame
//rate has to be capped or they run faster on displays above 60 Hz.
//Call once per frame, right after SDL_RenderPresent.
inline void limitFrameRate(int fps = 60)
{
    static Uint64 lastFrame = SDL_GetPerformanceCounter();
    const Uint64 freq = SDL_GetPerformanceFrequency();
    const Uint64 target = freq / fps;

    //Sleep for most of the remaining time, then spin for the sub-millisecond rest
    Uint64 elapsed = SDL_GetPerformanceCounter() - lastFrame;
    if(elapsed < target)
    {
        Uint32 ms = (Uint32)((target - elapsed) * 1000 / freq);
        if(ms > 1)
        { SDL_Delay(ms - 1); }
        while(SDL_GetPerformanceCounter() - lastFrame < target)
        {}
    }
    lastFrame = SDL_GetPerformanceCounter();
}

#endif
