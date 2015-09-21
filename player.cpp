#include "player.h"

Player::Player(){
  currentState = Still;
  previousState = Still;
  frameCount = 0;
  texIndex = 0;
  x = 0;
  y = 0;
  width = 32;
  height = 48;
  xVelocity = 0;
  yVelocity = 0;
  drawInvis = false;
  facingLeft = false;

  bulletLifeTime = 0;
  bulletXvelocity = 0;
  bulletYvelocity = 0;
  bulletX = 0;
  bulletY = 0;
  bulletIsFired = false;
  
  health = 3;
  isHit = false;
  invulnerableTime = 0;
  invulnerableTimeLimit = 180;
  isDead = false;
}


void Player::testWallCollision(int* &mapData, int mapWidth, int mapHeight, int decreaseX, int decreaseY){
  int xMax, xMin, yMax, yMin;
  
  //get the most possible tiles the
  //player could possibly be colliding with
  xMin = ((x + xVelocity)/32) - 1;
  xMax = xMin + 3;
  
  yMin = (y/32) - 1;
  yMax = yMin + 4;
  
  //make sure they are within bounds
  if(xMin < 0){
    xMin = 0;
  }
  if(xMax > (mapWidth - 1)){
    xMax = mapWidth - 1;
  }
  if(yMin < 0){
    yMin = 0;
  }
  if(yMax > (mapHeight - 1)){
    yMax = mapHeight - 1;
  }
  
  //loop through possible tiles player
  //will collide with
  for(int i = yMin; i <= yMax; i++){
    for(int j = xMin; j <= xMax; j++){

      if(mapData[(i * mapWidth) + j] != 0){
        while(true){
          //if there is a collision, reduce the velocity until the player
          //isn't going to hit a tile or velocity is reduced to 0 
          if(collisionWithQuads(x + xVelocity, y + yVelocity, width, height, 32 * j, 32 * i, 32, 32)){
            if(xVelocity == 0 && yVelocity == 0){
              break;
            }else{
              xVelocity += decreaseX;
              yVelocity += decreaseY;
            }
          }else{
            break;
          }
        }
      }

    }
  }

}



void Player::FSM(int* &mapData, int mapWidth, int mapHeight, Keys *key){
  switch( currentState ){
  case Still: //player is stading still on a tile
    
    if(key->a_isDown || key->d_isDown){
      currentState = Running;
      frameCount = 0;
    }

    texIndex = 0;
          
    if(key->w_isDown){
      frameCount = 0;
      currentState = Jumping;
      texIndex = 2;
    }

    if(facingLeft){
      texIndex += 13;
    }
    break;
    
  case Running: 
    //running left
    if(key->a_isDown){
      facingLeft = true;
      xVelocity = -2;
      yVelocity = 0;
      
      //loop through running animation
      if(frameCount < 8){
        texIndex = 4;
      }else if(frameCount < 16){
        texIndex = 6;
      }else if(frameCount < 24){
        texIndex = 8;
      }else if(frameCount < 32){
        texIndex = 10;
      }

      frameCount++;
      if(frameCount > 31){
        frameCount = 0;
      }

      testWallCollision(mapData, mapWidth, mapHeight, 1, 0);
      x += xVelocity;

    //running right
    }else if(key->d_isDown){
      facingLeft = false;
      xVelocity = 2;
      yVelocity = 0;

      //loop through running animation
      if(frameCount < 8){
        texIndex = 4;
      }else if(frameCount < 16){
        texIndex = 6;
      }else if(frameCount < 24){
        texIndex = 8;
      }else if(frameCount < 32){
        texIndex = 10;
      }

      frameCount++;
      if(frameCount > 31){
        frameCount = 0;
      }

      testWallCollision(mapData, mapWidth, mapHeight, -1, 0);
      x += xVelocity;

    }else{
      frameCount = 0;
      currentState = Still;
      texIndex = 0;
    }
          
    if(key->w_isDown){
      frameCount = 0;
      currentState = Jumping;
      texIndex = 2;
    }
    //check if feet are running on the ground
    xVelocity = 0;
    yVelocity = 1;
    testWallCollision(mapData, mapWidth, mapHeight, 0, -1);
    if(yVelocity != 0){
      frameCount = 0;
      currentState = Falling;
      texIndex = 2;
    }

    if(facingLeft){
      texIndex += 13;
    }

    break;
    
  case Jumping: 

    if(key->w_isDown){
      //player can only stay jumping for 60 frames
      if(frameCount < 60){
          xVelocity = 0;
          yVelocity = -2;
          testWallCollision(mapData, mapWidth, mapHeight, 0, 1);
          frameCount++;
          y += yVelocity;
      }else{
        frameCount = 0;
        currentState = Falling;
      }
    }else{
        frameCount = 0;
      currentState = Falling;
    }

    texIndex = 2;

    //check if player is still holding the run keys
    if(key->a_isDown){
      facingLeft = true;
      xVelocity = -2;
      yVelocity = 0;
      testWallCollision(mapData, mapWidth, mapHeight, 1, 0);
      x += xVelocity;
    }else if(key->d_isDown){
      facingLeft = false;
      xVelocity = 2;
      yVelocity = 0;
      testWallCollision(mapData, mapWidth, mapHeight, -1, 0);
      x += xVelocity;
    }

    if(facingLeft){
      texIndex += 13;
    }
    break;
    
  case Falling:

    texIndex = 2;
    xVelocity = 0;
    yVelocity = 2;
    
    testWallCollision(mapData, mapWidth, mapHeight, 0, -1);
          
    y += yVelocity;
    //check if player has landed on a tile
    if(yVelocity == 0){
      frameCount = 0;
      currentState = Still;
      texIndex = 0;
    }

    //check if player is still holding the run keys
    if(key->a_isDown){
      facingLeft = true;
      xVelocity = -2;
      yVelocity = 0;
      testWallCollision(mapData, mapWidth, mapHeight, 1, 0);
      x += xVelocity;
    }else if(key->d_isDown){
      facingLeft = false;
      xVelocity = 2;
      yVelocity = 0;
      testWallCollision(mapData, mapWidth, mapHeight, -1, 0);
      x += xVelocity;
    }

    if(facingLeft){
      texIndex += 13;
    }
    break;
    
  case Hit: 
    //player freezes for 30 frames
    if(frameCount < 30){
      frameCount++;
    }else{
      currentState = previousState;
      frameCount = 0;
    }

   texIndex = 12;

    if(facingLeft){
      texIndex += 13;
    }
    if(health <= 0){
       currentState = Dying;
    }
    break;

  case Dying:
    //loop through dying animation
    if(frameCount < 8){
        texIndex = 26;
      }else if(frameCount < 16){
        texIndex = 27;
      }else if(frameCount < 24){
        texIndex = 28;
      }else if(frameCount < 32){
        isDead = true;
      }
    if(!isDead){
      frameCount++;
    }
  break;
    default:
    
    break;
  }

  //check if it's ok to fire a bullet
  if(key->f_isDown && !bulletIsFired && currentState != Hit){
    bulletIsFired = true;
    bulletLifeTime = 0;
    bulletY = y + 16;
    if(facingLeft){
      bulletXvelocity = -3;
      bulletX = x - 8;
    }else{
      bulletXvelocity = 3;
      bulletX = x + 24;
    }
  }



  if(bulletIsFired){
    if(bulletLifeTime < 4 && currentState != Hit){
       texIndex++;
    }
		//time limit for bullet's lifetime is 180 frames
    if(bulletLifeTime < 180){ 
      bulletX += bulletXvelocity;
      bulletY += bulletYvelocity;
      bulletLifeTime++;
    }else{
      bulletIsFired = false;
    }
  }
  
  
  if(isHit){
    if(invulnerableTime % 10 == 0){
      drawInvis = !drawInvis;
    }

    if(drawInvis && currentState != Dying){
      texIndex = 32;
    }

    if(invulnerableTime < 90){
      invulnerableTime++;
    }else{
      invulnerableTime = 0;
      isHit = false;
    }
  }
}
