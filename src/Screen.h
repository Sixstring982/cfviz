#ifndef SCREEN_H
#define SCREEN_H

#include "SDL/SDL.h"

#define DEFAULT_FPS 25.0
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

typedef struct
{
  double fps;
  u16 h, w;
}ScreenParams;

#endif /* SCREEN_H */
