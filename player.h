#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "keys.h"
#include "collision.h"
#include "states.h"

class Player{
private:
  
public:
  int texIndex;
  States currentState;
  States previousState;
  int frameCount;
  int x; // Top left pixel position
  int y; // of the player
  int width;
  int height;
  int xVelocity;
  int yVelocity;
  int health;
  bool isHit;
  int invulnerableTime;
  int invulnerableTimeLimit;
  bool isDead;
  bool drawInvis;
  bool facingLeft;
  
  int bulletLifeTime;
  int bulletXvelocity;
  int bulletYvelocity;
  int bulletX;
  int bulletY;
  bool bulletIsFired;

  Player();
  void FSM(int* &mapData, int mapWidth, int mapHeight, Keys *key);
  void testWallCollision(int* &mapData, int mapWidth, int mapHeight, int decreaseX, int decreaseY);
};

#endif
