#ifndef _MAPFILEREADER_H_
#define _MAPFILEREADER_H_
#include <cstdio>
#include <cstdlib>
#include <cstring>

int getIntValue( char *charIndex);
void startParsing(FILE *mapFile, int *mapWidth, int *mapHeight, int* &mapPointer);
void getMapData(int *mapWidth, int *mapHeight, int* &mapPointer);

#endif
