#include "camera.h"

Camera::Camera(){
  x = 160;
  y = 160;
}

void Camera::drawSprite(int spriteX, int spriteY, int spriteHeight, int spriteWidth, float texCoords[][8], int texIndex){
 float leftX = (float(spriteX) - float(x))/160.0;
 float topY = (float(y) - float(spriteY))/160.0;
 float rightX = (float(spriteX + spriteWidth) - float(x))/160.0;
 float bottomY = (float(y) - float(spriteY + spriteHeight))/160.0;
 glBegin(GL_QUADS);
 glTexCoord2f(texCoords[texIndex][0], texCoords[texIndex][1]);
  glVertex3f(leftX, topY, 0.0);
  
  glTexCoord2f(texCoords[texIndex][2], texCoords[texIndex][3]);
  glVertex3f(rightX, topY, 0.0);
  
  glTexCoord2f(texCoords[texIndex][4], texCoords[texIndex][5]);
  glVertex3f(rightX, bottomY, 0.0);
  
  glTexCoord2f(texCoords[texIndex][6], texCoords[texIndex][7]);
  glVertex3f(leftX, bottomY, 0.0);
  glEnd();
}

void Camera::updatePosition(int playerX, int playerY, int mapHeight, int mapWidth){

  //camera is 48 pixels in front of player
  //so that you can see enemies ahead of you
  x = playerX + 48;
  y = playerY;

  //keep camera's within the maps boundries
  if(x > (mapWidth * 32) - 160){
    x = (mapWidth * 32) - 160;
  } else if(x < 160){
    x = 160;
  }
  
  if(y > (mapHeight * 32) - 160){
    y = (mapHeight * 32) - 160;
  } else if(y < 160){
    y = 160;
  }
}
