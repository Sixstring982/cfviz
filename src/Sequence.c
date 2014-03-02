#include "Sequence.h"

bool NextSequenceSignal(Sequence* s)
{
  s->currentFrame++;

  if(s->currentFrame >= s->interval.y)
  {
    s->currentFrame = s->interval.x;
    if(!s->looping)
    {
      return true;
    }
  }
  return false;
}

void ResetSequence(Sequence* s)
{
  s->currentFrame = s->interval.x;
}
