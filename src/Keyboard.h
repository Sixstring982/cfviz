#ifndef KEYSTACK_H
#define KEYSTACK_H

#include "itype.h"
#include "SDL/SDL.h"

#define KEYBOARD_SIZE 0xFF

typedef struct
{
  SDLKey field[KEYBOARD_SIZE];
  u32 heldKeys;
}KeyField;

typedef struct
{
  KeyField previous;
  KeyField current;
}Keyboard;

extern void InitKeyboard(Keyboard* kb);

extern void PressKey(Keyboard* kb, SDLKey key);

extern void ReleaseKey(Keyboard* kb, SDLKey key);

extern bool IsKeyDown(Keyboard* kb, SDLKey key);

extern bool IsKeyUp(Keyboard* kb, SDLKey key);

extern bool SemiAuto(Keyboard* kb, SDLKey key);

extern void Flip(Keyboard* kb);

#endif /* KEYSTACK_H */
