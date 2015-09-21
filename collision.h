#ifndef _COLLISION_H_
#define _COLLISION_H_

int vecDot2i(int *a, int *b);
int whichSide(int x2, int y2, int width2, int height2, int *P, int *D);
bool collisionWithQuads(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

#endif
