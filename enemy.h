#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "player.h"
#include "collision.h"
#include "states.h"
class Enemy{
  private:

  public:
  int texIndex;
  States currentState;
  int frameCount;
  int x; // Top left pixel position
  int y; // of the enemy

  bool facingRight;
  
  int bulletLifeTime;
  int bulletXvelocity;
  int bulletYvelocity;
  int bulletX;
  int bulletY;
  bool bulletIsFired;
  
  Enemy();
  void FSM( Player *player);
};

#endif
