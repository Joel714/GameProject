#include "enemy.h"

Enemy::Enemy(){
  currentState = Looking;
  frameCount = 0;
  texIndex = 0;
  x = 0;
  y = 0;
  
  facingRight = false;

  bulletLifeTime = 0;
  bulletXvelocity = 0;
  bulletYvelocity = 0;
  bulletX = 0;
  bulletY = 0;
  bulletIsFired = false;
}

void Enemy::FSM(Player *player){
  switch( currentState ){
  case Looking: //0 Looking for player
          if(bulletIsFired){
            //do nothing, wait for bullet timer
            //to run out before attacking again
          }else if((player->x - x) <  160 && (player->x - x) > -160){
            //player is in range, change to attacking state
            currentState = Attacking;
          }
          texIndex = 0;
          frameCount = 0;
          if(facingRight){
            texIndex += 8;
          }
          break;
    
  case Attacking: //1 Attacking player
          frameCount++;
          //animate through firing a projectile
          if(frameCount < 10){
              texIndex = 1; //
          }else if(frameCount < 50){
            //fire projectile
            if(bulletIsFired == false && frameCount == 10){
              bulletIsFired = true;
              bulletLifeTime = 0;
              bulletY = y + 16;
              if(player->x > x){
                bulletXvelocity = 2;
                bulletX = x + 32;
                facingRight = true;
              }else{
                bulletXvelocity = -2;
                bulletX = x;
                facingRight = false;
              }
          }
              texIndex = 2;
          }else if(frameCount < 55){
              texIndex = 1;
          }else if(frameCount < 120){
              texIndex = 0;
          }else{
              currentState = Looking;
              texIndex = 0;
          }

       if(facingRight){
            texIndex += 8;
          }
          
    break;
    
  case Dying: //2 dying
    frameCount++;
    //go through death animation
    if(frameCount < 4){
      texIndex = 12;
    }else if(frameCount < 8){
      texIndex = 13;
    }else if(frameCount < 12){
      texIndex = 14;
    }else{
      texIndex = 15;
      currentState = Dead;
    }
    break;
    
  case Dead: //3 dead
    break;

  default:
  break;
  }

  //detect if hit with player's projectile
  if(player->bulletIsFired && currentState != Dead && currentState != Dying && collisionWithQuads(x, y, 32, 64, player->bulletX, player->bulletY, 16, 16)){
    currentState = Dying;
    player->bulletIsFired = false;
    frameCount = 0;
  }
  

  //if projectile fired, check if it hits
  //player and check it's life time is up
  if(bulletIsFired){
    if(bulletLifeTime < 180){
      bulletX += bulletXvelocity;
      bulletY += bulletYvelocity;
      bulletLifeTime++;
      if(collisionWithQuads(player->x, player->y, player->width, player->height, bulletX, bulletY, 16, 16) && !(player->isHit)){
        player->previousState = player->currentState;
        player->currentState = Hit;
        player->health -= 1;
        player->isHit = true;
        bulletIsFired = false;
      }
    }else{
      bulletIsFired = false;
    }
  }
}
