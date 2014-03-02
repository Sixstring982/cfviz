#include "itype.h"
#include "Keyboard.h"
#include "Screen.h"
#include "Sequence.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/* The video needs to be in the video folder of this directory, named
   1.png, 2.png, ... n.png . We will also look for video.config, a
   file describing screen width, height, fps, and region information. */

/*         video.config FILE FORMAT
 *
 * Line 1:     screen width
 * Line 2:     screen height
 * Line 3:     screen FPS
 *
 * Next n lines: Each line describes the start of the next
 *               sequence. The first sequence is implied to be at
 *               frame 0, so the first sequence described in this
 *               section is really the second sequence to come in the
 *               vis. If one of these starts with a '*', it will loop.
 */

/**
 * Returns true if EOF was reached.
 */
u32 ReadLine(FILE* fp, char* toBuffer)
{
  char c;
  u32 charsRead = 0;
  while((c = fgetc(fp)) != EOF)
  {
    charsRead++;
    if(c == '\n')
    {
      break;
    }
    *(toBuffer++) = c;
  }
  *toBuffer = 0;
  return charsRead;
}

typedef struct
{
  ScreenParams params;
  Sequence sequences[64];
  u32 usedSequences;
}VizConfig;

void SetupFromConfig(VizConfig* cfg)
{
  FILE* fp = fopen("../video/video.config", "r");
  u32 currentSeq = 0;
  char buffer[128];
  char* bufp;
  ReadLine(fp, buffer);
  cfg->params.w = atoi(buffer);
  ReadLine(fp, buffer);
  cfg->params.h = atoi(buffer);
  ReadLine(fp, buffer);
  cfg->params.fps = atof(buffer);

  do
  {
    ReadLine(fp, buffer);
    if(*buffer)
    {
      bufp = buffer;
      cfg->sequences[currentSeq].looping = false;
      if(*bufp == '*')
      {
	cfg->sequences[currentSeq].looping = true;
	bufp++;
      }
      BufferToPoint(&cfg->sequences[currentSeq].interval, bufp);
      cfg->sequences[currentSeq].currentFrame = cfg->sequences[currentSeq].interval.x;
      currentSeq++;
    }
  }while(!feof(fp));

  cfg->usedSequences = currentSeq;

  fclose(fp);
}

SDL_Surface* InitScreen(VizConfig* cfg)
{
  return SDL_SetVideoMode(cfg->params.w, cfg->params.h, 32, SDL_FULLSCREEN);
}

void HandleKeyboard(Keyboard* kb, bool* running)
{
  if(IsKeyDown(kb, SDLK_ESCAPE))
  {
    *running = false;
  }
}

SDL_Surface* LoadPNG(char* filename)
{
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* opped = NULL;

  loadedImage = IMG_Load(filename);

  if(loadedImage)
  {
    opped = SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
  }
  return opped;
}

void BlitNextPNG(SDL_Surface* dest, VizConfig* cfg, u32 currentSequence)
{
  SDL_Surface* loadedPng;
  char fnameBuffer[32];
  SDL_Rect frameRect;

  sprintf(fnameBuffer, "../video/%02d.png", cfg->sequences[currentSequence].currentFrame);

  loadedPng = LoadPNG(fnameBuffer);

  frameRect.x = frameRect.y = 0;
  frameRect.w = cfg->params.w;
  frameRect.h = cfg->params.h;

  SDL_BlitSurface(loadedPng, NULL, dest, &frameRect);

  SDL_FreeSurface(loadedPng);
}

void RunVis()
{
  VizConfig cfg;
  bool running = true;
  SDL_Event event;
  Keyboard kb;
  SDL_Surface* screen;
  u32 currentSequence = 0;
  s32 sleepMS = 0;
  u32 lastFrame = SDL_GetTicks();
  SetupFromConfig(&cfg);

  InitKeyboard(&kb);
  screen = InitScreen(&cfg);
  SDL_ShowCursor(0);

  while(running)
  {

    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      case SDL_QUIT: running = false; break;
      case SDL_KEYDOWN:
	PressKey(&kb, event.key.keysym.sym);
	break;
      case SDL_KEYUP:
	ReleaseKey(&kb, event.key.keysym.sym);
	break;
      }
    }
    HandleKeyboard(&kb, &running);
    Flip(&kb);

    BlitNextPNG(screen, &cfg, currentSequence);
    if(NextSequenceSignal(cfg.sequences + currentSequence))
    {
      currentSequence++;
      ResetSequence(cfg.sequences + currentSequence);
    }

    sleepMS = (s32)((1000.0 / cfg.params.fps) - (SDL_GetTicks() - lastFrame));
    if(sleepMS > 0)
    {
      SDL_Delay(sleepMS);
    }
    lastFrame = SDL_GetTicks();

    SDL_Flip(screen);

  }

  SDL_FreeSurface(screen);
}

int main(int argc, char** argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  RunVis();

  SDL_Quit();
  return 0;
}
