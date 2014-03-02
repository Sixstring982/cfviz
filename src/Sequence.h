#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "itype.h"
#include "Point.h"

typedef struct
{
  Point interval;
  u32 currentFrame;
  bool looping;
}Sequence;

/**
 * Increments the sequence's position.
 *
 * @returns true if this Sequence is finished and the next Sequence
 *          should begin executing, else false.
 */
extern bool NextSequenceSignal(Sequence* s);

extern void ResetSequence(Sequence* s);

#endif /* SEQUENCE_H */
