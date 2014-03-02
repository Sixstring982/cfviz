#ifndef POINT_H
#define POINT_H

#include "itype.h"

typedef struct
{
  u32 x, y;
}Point;

extern void BufferToPoint(Point* toFill, char* buffer);

#endif /* POINT_H */
