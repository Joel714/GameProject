#include "collision.h"

//dot product for 2d vectors
int vecDot2i(int *a, int *b){
  int accumulate = 0;
  int i;
  for(i = 0; i < 2; i++){
    accumulate += a[i] * b[i];
  }
  return( accumulate );
}

//determines if an edge is on the positive side of
//one quad, if it is then no collision has taken place.
int whichSide(int x2, int y2, int width2, int height2, int *P, int *D){
  int posCount = 0;
  int negCount = 0;
  int zeroCount = 0;
  int t = 0;
  int vectorDiff[2];
  
  //bottom right
  vectorDiff[0] = x2 + width2 - 1 - P[0];
  vectorDiff[1] = y2 + height2 - 1 - P[1];
  
  t = vecDot2i(D, vectorDiff);
  
  if(t > 0){
    posCount++;
  }else if(t < 0){
    negCount++;
  }else{
    zeroCount++;
  }
  
  //top right
  vectorDiff[0] = x2 + width2 - 1 - P[0];
  vectorDiff[1] = y2 - P[1];
  
  t = vecDot2i(D, vectorDiff);
  
  if(t > 0){
    posCount++;
  }else if(t < 0){
    negCount++;
  }else{
    zeroCount++;
  }
  
  //bottom left
  vectorDiff[0] = x2 - P[0];
  vectorDiff[1] = y2 + height2 - 1  - P[1];
  
  t = vecDot2i(D, vectorDiff);
  
  if(t > 0){
    posCount++;
  }else if(t < 0){
    negCount++;
  }else{
    zeroCount++;
  }
  
  //top left
  vectorDiff[0] = x2 - P[0];
  vectorDiff[1] = y2 - P[1];
  
  t = vecDot2i(D, vectorDiff);
  
  if(t > 0){
    posCount++;
  }else if(t < 0){
    negCount++;
  }else{
    zeroCount++;
  }
    
  if((posCount > 0 && negCount > 0) || zeroCount > 0){
    return 0;
  }
  
  if(posCount == 0){
    return -1;
  }else{
    return 1;
  }
}

//Takes in two quadrilaterals and returns true if
//they are colliding with each other.
//Uses simpler separation of axis method since
//both quads are axis aligned
bool collisionWithQuads(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2){
  int P[] = {0, 0};
  int D[] = {0, 0};
  
  P[0] = x1 + width1 - 1;
  P[1] = y1 + height1 - 1;
  D[0] = 0;
  D[1] = 1;

  if(whichSide(x2, y2, width2, height2, P, D) > 0){
    return false;
  }
  
  P[0] = x1 + width1 - 1;
  P[1] = y1;
  D[0] = 1;
  D[1] = 0;

  if(whichSide(x2, y2, width2, height2, P, D) > 0){
    return false;
  }
  
  P[0] = x1;
  P[1] = y1;
  D[0] = 0;
  D[1] = -1;

  if(whichSide(x2, y2, width2, height2, P, D) > 0){
    return false;
  }
  
  P[0] = x1;
  P[1] = y1 + height1 - 1;
  D[0] = -1;
  D[1] = 0;

  if(whichSide(x2, y2, width2, height2, P, D) > 0){
    return false;
  }
  return true;
}

