#include <stdlib.h>
#include "Point.h"

void BufferToPoint(Point* toFill, char* buffer)
{
  toFill->x = atoi(buffer);

  while(*(buffer++) != ',');

  toFill->y = atoi(buffer);
}
