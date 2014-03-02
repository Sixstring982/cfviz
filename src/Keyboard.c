#include "Keyboard.h"

void InitKeyboard(Keyboard* kb)
{
  u32 i;
  for(i = 0; i < KEYBOARD_SIZE; i++)
  {
    kb->previous.field[i] = (SDLKey)0;
    kb->current.field[i] = (SDLKey)0;
  }
  kb->previous.heldKeys =
    kb->current.heldKeys = 0;
}

void PressKey(Keyboard* kb, SDLKey key)
{
  u32 keysFound = 0;
  u32 i;
  for(i = 0; i < KEYBOARD_SIZE; i++)
  {
    if(!kb->current.field[i] && keysFound == kb->current.heldKeys)
    {
      kb->current.field[i] = key;
      break;
    }
    else if(kb->current.field[i])
    {
      keysFound++;
    }
  }
  kb->current.heldKeys++;
}

void ReleaseKey(Keyboard* kb, SDLKey key)
{
  u32 i;
  for(i = 0; i < KEYBOARD_SIZE; i++)
  {
    if(kb->current.field[i] == key)
    {
      kb->current.field[i] = (SDLKey)0;
    }
  }
  kb->current.heldKeys--;
}

static bool KeyDownInField(KeyField* kf, SDLKey key)
{
  u32 foundKeys = 0;
  u32 i;
  for(i = 0; i < KEYBOARD_SIZE; i++)
  {
    if(foundKeys == kf->heldKeys)
    {
      return false;
    }
    if(kf->field[i] == key)
    {
      return true;
    }
    else if(kf->field[i])
    {
      foundKeys++;
    }
  }
  return false;
}

bool IsKeyDown(Keyboard* kb, SDLKey key)
{
  return KeyDownInField(&kb->current, key);
}

extern bool IsKeyUp(Keyboard* kb, SDLKey key)
{
  return !IsKeyDown(kb, key);
}

extern bool SemiAuto(Keyboard* kb, SDLKey key)
{
  return KeyDownInField(&kb->current, key) &&
    !KeyDownInField(&kb->previous, key);
}

extern void Flip(Keyboard* kb)
{
  u32 i;
  kb->previous.heldKeys = kb->current.heldKeys;
  for(i = 0; i < KEYBOARD_SIZE; i++)
  {
    kb->previous.field[i] = kb->current.field[i];
  }
}
