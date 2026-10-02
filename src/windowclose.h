#ifndef WINDOWCLOSE_H_INCLUDED
#define WINDOWCLOSE_H_INCLUDED

//Set when the window's close button is pressed so every screen
//(game, pause, menus) can unwind and exit the whole program.
inline bool & windowCloseRequested()
{
    static bool requested = false;
    return requested;
}

#endif
