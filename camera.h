#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/gl.h>

class Camera{
  private:

  public:

  int x, y;

  Camera();
  void drawSprite(int spriteX, int spriteY, int spriteHeight, int spriteWidth, float texCoords[][8], int texIndex);
  void updatePosition(int playerX, int playerY, int mapHeight, int mapWidth);
};

#endif
