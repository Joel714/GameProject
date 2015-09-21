
#include <cstdlib>
#include <cstdio>
#include <cassert>

#include <GL/gl.h>
#include <GL/glut.h>


#ifdef FREEGLUT
#include <GL/freeglut_ext.h>
#endif

#ifndef NDEBUG
#define msglError( ) _msglError( stderr, __FILE__, __LINE__ )
#else
#define msglError( )
#endif

#include <FreeImage.h>

#include "keys.h"
#include "player.h"
#include "enemy.h"
#include "mapfileReader.h"
#include "camera.h"
#include "states.h"

Keys keys;

Player player;
GLuint playerTextureId;
float playerTexCoords[33][8] = {
  { 0.0, 1.0, 0.0625, 1.0, 0.0625, 0.90625, 0.0, 0.90625},//0 stand still :Right
  {0.125, 1.0, 0.1875, 1.0, 0.1875, 0.90625, 0.125, 0.90625},
  {0.5, 0.90625, 0.5625, 0.90625, 0.5625, 0.8125, 0.5, 0.8125},//2 jump/fall :Right
  {0.5, 0.8125, 0.5625, 0.8125, 0.5625, 0.71875, 0.5, 0.71875},
  {0.0, 0.90625, 0.0625, 0.90625, 0.0625, 0.8125, 0.0, 0.8125},//4 walking 1 :Right
  {0.0, 0.8125, 0.0625, 0.8125, 0.0625, 0.71875, 0.0, 0.71875},
  {0.0625, 0.90625, 0.125, 0.90625, 0.125, 0.8125, 0.0625, 0.8125},//6 walking 2 :Right
  {0.0625, 0.8125, 0.125, 0.8125, 0.125, 0.71875, 0.0625, 0.71875},
  {0.125, 0.90625, 0.1875, 0.90625, 0.1875, 0.8125, 0.125, 0.8125},//8 walking 3 :Right
  {0.125, 0.8125, 0.1875, 0.8125, 0.1875, 0.71875, 0.125, 0.71875},
  {0.1875, 0.90625, 0.25, 0.90625, 0.25, 0.8125, 0.1875, 0.8125},//10 walking 4 :Right
  {0.1875, 0.8125, 0.25, 0.8125, 0.25, 0.71875, 0.1875, 0.71875},
  {0.25, 1.0, 0.3125, 1.0, 0.3125, 0.90625, 0.25, 0.90625},//12 hit :Right
  {0.0625, 1.0, 0.125, 1.0, 0.125, 0.90625, 0.0625, 0.90625},//13 stand still :Left
  {0.1875, 1.0, 0.25, 1.0, 0.25, 0.90625, 0.1875, 0.90625},
  { 0.5625, 0.90625, 0.625, 0.90625, 0.625, 0.8125, 0.5625, 0.8125},//15 jump/fall :Left
  {0.5625, 0.8125, 0.625, 0.8125, 0.625, 0.71875, 0.5625, 0.71875},
  {0.25, 0.90625, 0.3125, 0.90625, 0.3125, 0.8125, 0.25, 0.8125},//17 walking 1 :Left
  {0.25, 0.8125, 0.3125, 0.8125, 0.3125, 0.71875, 0.25, 0.71875},
  {0.3125, 0.90625, 0.375, 0.90625, 0.375, 0.8125, 0.3125, 0.8125},//19 walking 2 :Left
  {0.3125, 0.8125, 0.375, 0.8125, 0.375, 0.71875, 0.3125, 0.71875},
  {0.375, 0.90625, 0.4375, 0.90625, 0.4375, 0.8125, 0.375, 0.8125},//21 walking 3 :Left
  {0.375, 0.8125, 0.4375, 0.8125, 0.4375, 0.71875, 0.375, 0.71875},
  {0.4375, 0.90625, 0.5, 0.90625, 0.5, 0.8125, 0.4375, 0.8125},//23 walking 4 :Left
  {0.4375, 0.8125, 0.5, 0.8125, 0.5, 0.71875, 0.4375, 0.71875},
  {0.3125, 1.0, 0.375, 1.0, 0.375, 0.90625, 0.3125, 0.90625},//25 hit :Left
  {0.375, 1.0, 0.4375, 1.0, 0.4375, 0.90625, 0.375, 0.90625},//26 chunk 1
  {0.4375, 1.0, 0.5, 1.0, 0.5, 0.90625, 0.4375, 0.90625},//27 chunk 2
  {0.5, 1.0, 0.5625, 1.0, 0.5625, 0.90625, 0.5, 0.90625},//28 chunk 3
  {0.90625, 1.0, 1.0, 1.0, 1.0, 0.96875, 0.90625, 0.96875},//29 bar
  {0.9375, 0.96875, 0.96875, 0.96875, 0.96875, 0.9375, 0.9375, 0.9375},//30 red
  {0.96875, 0.96875, 1.0, 0.96875, 1.0, 0.9375, 0.96875, 0.9375},//31 bullet
  {0.5625 , 1.0, 0.625, 1.0, 0.625, 0.90625, 0.5625, 0.90625}//32 invisible
};

int mapWidth = 0;
int mapHeight = 0;
int *mapData;
GLuint mapTexureId;
float mapTexCoords[4][8] = {
  {0.0, 1.0,    0.5, 1.0,    0.5, 0.5,    0.0, 0.5},
  {0.5, 1.0,    1.0, 1.0,    1.0, 0.5,    0.5, 0.5},
  {0.0, 0.5,    0.5, 0.5,    0.5, 0.0,    0.0, 0.0},
  {0.5, 0.5,    1.0, 0.5,    1.0, 0.0,    0.5, 0.0}
};

const int enemyArraySize = 3;
Enemy enemyArray[enemyArraySize];
GLuint enemyTextureId;
float etexCoords[16][8] = {
  {0.0, 1.0,    0.25, 1.0,    0.25, 0.5,    0.0, 0.5},//0 Looking/standing stil : Left
  {0.25, 1.0,    0.5, 1.0,    0.5, 0.5,    0.25, 0.5},//1 arm half way up : Left
  {0.5, 1.0,    0.75, 1.0,    0.75, 0.5,    0.5, 0.5},//2 arm up/firing : Left
  {0.75, 1.0,    0.875, 1.0,    0.875, 0.875,    0.75, 0.875},//3 bullet
  {0.0, 0.5,    0.25, 0.5,    0.25, 0.0,    0.0, 0.0},//4 dying animation 1 : Left
  {0.25, 0.5,    0.5, 0.5,    0.5, 0.0,    0.25, 0.0},//5 dying animation 2 : Left
  {0.5, 0.5,    0.75, 0.5,    0.75, 0.0,    0.5, 0.0},//6 dying animation 3 : Left
  {0.75, 0.5,    1.0, 0.5,    1.0, 0.0,    0.75, 0.0},//7 dead : Left
  {0.25, 1.0,    0.0, 1.0,    0.0, 0.5,    0.25, 0.5},//8 Looking/standing stil : Right
  {0.5, 1.0,    0.25, 1.0,    0.25, 0.5,    0.5, 0.5},//9 arm half way up : Right
  {0.75, 1.0,    0.5, 1.0,    0.5, 0.5,    0.75, 0.5},//10 arm up/firing : Right
  {0.875, 1.0,    0.75, 1.0,    0.75, 0.875,    0.875, 0.875},//bullet
  {0.25, 0.5,    0.0, 0.5,    0.0, 0.0,    0.25, 0.0},//12 dying animation 1 : Right
  {0.5, 0.5,    0.25, 0.5,    0.25, 0.0,    0.5, 0.0},//13 dying animation 2 : Right
  {0.75, 0.5,    0.5, 0.5,    0.5, 0.0,    0.75, 0.0},//14 dying animation 3 : Right
  {1.0, 0.5,    0.75, 0.5,    0.75, 0.0,    1.0, 0.0}//15 dead : Right
};

bool gameStarted = false;
bool gameEnded = false;

Camera myCamera;

bool _msglError( FILE *out, const char *filename, int line ){
  bool ret = false;
  GLenum err = glGetError( );
  while( err != GL_NO_ERROR ) {
    ret = true;
    fprintf( out, "GL ERROR:%s:%d: %s\n", filename, line, (char*)gluErrorString( err ) );
    err = glGetError( );
  }
  return( ret );
}


void initOpenGL( ){
  glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthRange(0.0, 1.0);
  glDepthFunc(GL_LEQUAL);
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  msglError( );
}


void initKeys(){
  keys.a_isDown = false;
  keys.d_isDown = false;
  keys.w_isDown = false;
  keys.f_isDown = false;
}


//uses FreeImage to load a texture
void textureLoader(const char *filename,  GLuint *textureid){
    FIBITMAP* bitmap = NULL;
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
    if( fif == FIF_UNKNOWN ){
        fif = FreeImage_GetFIFFromFilename(filename);
    }
    if( (fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)){
        bitmap = FreeImage_Load(fif, filename, 0);
        assert(bitmap);
    }
    
    assert(bitmap);
    FREE_IMAGE_COLOR_TYPE fic = FreeImage_GetColorType(bitmap);
    assert( fic == FIC_RGB || fic == FIC_RGBALPHA );
    unsigned int bpp = FreeImage_GetBPP(bitmap);
    assert(bpp == 24 || bpp == 32);
    GLenum pixelFormat;
    if(bpp == 24){
        pixelFormat = GL_BGR;
    }else{
        pixelFormat = GL_BGRA;
    }
    
    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    
    glGenTextures(1, textureid);
    glBindTexture(GL_TEXTURE_2D, *textureid);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));
}


void resetLevel(){
  player.currentState = Still;
  player.previousState = Still;
  player.frameCount = 0;
  player.texIndex = 0;
  player.xVelocity = 0;
  player.yVelocity = 0;

  player.facingLeft = false;

  player.bulletLifeTime = 0;
  player.bulletIsFired = false;
  
  player.health = 3;
  player.isHit = false;
  player.invulnerableTime = 0;
  player.invulnerableTimeLimit = 180;
  player.isDead = false;
  player.x = 64;
  player.y = 240;
  for(int i = 0; i < enemyArraySize; i++){
    enemyArray[i].currentState = Looking;
    enemyArray[i].frameCount = 0;
    enemyArray[i].texIndex = 0;
  
    enemyArray[i].facingRight = false;

    enemyArray[i].bulletLifeTime = 0;
    enemyArray[i].bulletIsFired = false;
  }
}


void keyboardDown( unsigned char key, int x, int y ){
  switch( key ){
  case 27: // The 'esc' key
  case 'q':
    delete [] mapData;
#ifdef FREEGLUT
    glutLeaveMainLoop( );
#else
    exit( 0 );
#endif
    break;
  case 'a':
    keys.a_isDown = true;
    break;
  case 'd':
    keys.d_isDown = true;
    break;
  case 'w':
    keys.w_isDown = true;
    break;   
  case 'f':
    keys.f_isDown = true;
    break;
  case 's':
    gameStarted = true;
    break;
  case 'r':
    if(gameEnded){
      gameEnded = false;
      resetLevel();
    }
    break;
  default:
    break;
  }
}


void keyboardUp( unsigned char key, int x, int y ){
  switch( key ){
    case 'a':
     keys.a_isDown = false;
     break;
  case 'd':
     keys.d_isDown = false;
     break;   
  case 'w':
     keys.w_isDown = false;
     break;
  case 'f':
     keys.f_isDown = false;
     break;
  default:
     break;
  }
}


void display( ){
  msglError( );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, mapTexureId);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if(gameEnded){
    //show ending menu
    glBindTexture(GL_TEXTURE_2D, playerTextureId);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 0.4375);
    glVertex3f(-1.0, 1.0, 0.0);
  
    glTexCoord2f(0.25, 0.4375);
    glVertex3f(1.0, 1.0, 0.0);
  
    glTexCoord2f(0.25, 0.15625);
    glVertex3f(1.0, -1.0, 0.0);
  
    glTexCoord2f(0.0, 0.15625);
    glVertex3f(-1.0, -1.0, 0.0);
    
    glEnd();
  }else if(gameStarted){
    //the maximum and minumum amount of
    //tiles that are viewed on screen
    int minXview = 0;
    int maxXview = 0;
    int minYview = 0;
    int maxYview = 0;

    //get max and min of viewable tiles
    if(myCamera.x % 32 == 0){
      minXview = myCamera.x/32 - 5;
      maxXview = myCamera.x/32 + 4;
    }else{
      minXview = myCamera.x/32 - 5;
      maxXview = myCamera.x/32 + 5;
    }

    if(myCamera.y % 32 == 0){
      minYview = myCamera.y/32 - 5;
      maxYview = myCamera.y/32 + 4;
    }else{
      minYview = myCamera.y/32 - 5;
      maxYview = myCamera.y/32 + 5;
    }

    //check if it's within possible values
    if(minXview < 0 || minXview > (mapWidth - 1)){
      minXview = 0;
    }

    if(maxXview > (mapWidth - 1) || maxXview < 0 ){
      maxXview = 0;
    }

    if(minYview < 0 || minYview > (mapHeight - 1)){
      minYview = 0;
    }

    if(maxYview > (mapHeight - 1) || maxYview < 0 ){
      maxYview = 0;
    }

    //loop through map data and draw the tiles
    for(int i = minYview; i <= maxYview; i++){
      for(int j = minXview; j <= maxXview; j++){
        if(mapData[(i * mapWidth) + j] == 0){
  	
        }else if(mapData[(i * mapWidth) + j] == 1){
          myCamera.drawSprite(j * 32, i * 32, 32, 32, mapTexCoords, 1);
        }
      }
    }
  
    glBindTexture(GL_TEXTURE_2D, playerTextureId);
    //draw player sprite
    myCamera.drawSprite(player.x, player.y, 48, 32, playerTexCoords, player.texIndex);

    //draw red healthbar
    myCamera.drawSprite(myCamera.x - 160, myCamera.y - 160, 16, 16 * player.health, playerTexCoords, 30);

    //draw healthbar outline
    myCamera.drawSprite(myCamera.x - 160, myCamera.y - 160, 16, 48, playerTexCoords, 29);

    //draw player bullet if it's fired
    if(player.bulletIsFired){
      myCamera.drawSprite(player.bulletX, player.bulletY, 16, 16, playerTexCoords, 31);
    }
    
    //loop through enemy array and draw 
    //enemy sprite and bullet
    glBindTexture(GL_TEXTURE_2D, enemyTextureId);
    for(int eIndex = 0; eIndex < enemyArraySize; eIndex++){
      myCamera.drawSprite(enemyArray[eIndex].x, enemyArray[eIndex].y, 64, 32, etexCoords, enemyArray[eIndex].texIndex);

      if(enemyArray[eIndex].bulletIsFired){
        myCamera.drawSprite(enemyArray[eIndex].bulletX, enemyArray[eIndex].bulletY, 16, 16, etexCoords, 3);
      }
    }
    
  }else if(!gameStarted){
    //show strating menu
    glBindTexture(GL_TEXTURE_2D, playerTextureId);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.71875);
    glVertex3f(-1.0, 1.0, 0.0);
  
    glTexCoord2f(0.25, 0.71875);
    glVertex3f(1.0, 1.0, 0.0);
  
    glTexCoord2f(0.25, 0.4375);
    glVertex3f(1.0, -1.0, 0.0);
  
    glTexCoord2f(0.0, 0.4375);
    glVertex3f(-1.0, -1.0, 0.0);

    glEnd();
  }

  glDisable(GL_TEXTURE_2D);
  glutSwapBuffers( );
  msglError( );
}

void reshape( int width, int height ){
  if (height == 0){
    height = 1;
  }
  glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
}

void update(int x){
  //end game when player reaches
  //the end of the map
  if(player.x >= (mapWidth - 2) * 32){
    gameEnded = true;
  }

  //reset level if player dies
  if(player.isDead || player.y > (mapHeight * 32)){
    resetLevel();
  }

  //if the game has started then
  //go through the player and enemy fsm
  if(gameStarted && !gameEnded){
    player.FSM(mapData, mapWidth, mapHeight, &keys);
    myCamera.updatePosition(player.x, player.y, mapHeight, mapWidth);
    for(int i = 0; i < enemyArraySize; i++){
      enemyArray[i].FSM(&player);
    }
  }

  glutTimerFunc(16, update, 0);
  glutPostRedisplay();  
}

int main(int argc, const char* argv[]){

  glutInit( &argc, (char**)argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( 640, 640);
  glutCreateWindow( "Demo" );

  initOpenGL( );
  initKeys();

  getMapData(&mapWidth, &mapHeight, mapData);

  player.x = 64;
  player.y = 240;

  enemyArray[0].x = 640;
  enemyArray[0].y = 224;
  enemyArray[1].x = 1120;
  enemyArray[1].y = 160;
  enemyArray[2].x = 1600;
  enemyArray[2].y = 224;

  textureLoader("data/playerSprites.png", &playerTextureId);
  textureLoader("data/enemySprites.png", &enemyTextureId);
  textureLoader("data/tileSprites.png", &mapTexureId);

  glutKeyboardFunc( keyboardDown );
  glutKeyboardUpFunc( keyboardUp );
  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutTimerFunc(16, update, 0);
  glutMainLoop( );
  return(0);
}

