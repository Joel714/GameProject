#include "mapfileReader.h"

//this function is only for getting
//the width and height of the map
int getIntValue( char *charIndex){
  int i = 0;
  int j = 0;
  bool startSavingInt = false;
  char intString[10];
  
  while(true){
    if(charIndex[i] == '\0'){
      printf("terminated char reached");
      return 0;
    }
    
    if(startSavingInt && j < 9){
      intString[j] = charIndex[i];
      intString[j + 1] = '\0';
      j++;
    }
    
    if(charIndex[i] == '"'){
      if(startSavingInt == false){
	startSavingInt = true;
      }else{
	return atoi(intString);
      }
    }
    
    i++;
  }
  return 0;
}

void startParsing(FILE *mapFile, int *mapWidth, int *mapHeight, int* &mapPointer){
  char lineString[500];
  char *charIndex;
  
  //look for layer tag, then get width
  //and height of map
  while(fgets(lineString, 500, mapFile) != NULL){
    charIndex = strstr(lineString, "<layer");
    if(charIndex != NULL){
      charIndex = strstr(lineString, "width");
      *mapWidth = getIntValue(charIndex);
      printf("width in file: %d\n", *mapWidth);
      charIndex = strstr(lineString, "height");
      *mapHeight = getIntValue(charIndex);
      printf("height in file: %d\n", *mapHeight);
      break;
    }
  }
  
  //need this extra fgets to ignore one
  //line in the file and go to the map data
  fgets(lineString, 500, mapFile);
  
  mapPointer = new int[(*mapHeight) * (*mapWidth)];
  
  int linestringIndex = 0;
  int mapdataIndex = 0;
  int intstringIndex = 0;
  char intString[10];
  
  for(int i = 0; fgets(lineString, 500, mapFile) != NULL && i < (*mapHeight); i++){
    while(lineString[linestringIndex] != '\n'){
      if(lineString[linestringIndex] == ','){
	mapPointer[mapdataIndex] = atoi(intString);
	mapdataIndex++;
	intstringIndex = 0;
      }else{
	intString[intstringIndex] = lineString[linestringIndex];
	intString[intstringIndex + 1] = '\0';
	intstringIndex++;
      }
      linestringIndex++;
    }
    linestringIndex = 0;
  }
}



void getMapData(int *mapWidth, int *mapHeight, int* &mapPointer){
  FILE * xmlFile;
 
  xmlFile = fopen("data/firstlevel.tmx", "r");
  if(xmlFile == NULL){
    perror("error opening file\n");
  } else {
    startParsing(xmlFile, mapWidth, mapHeight, mapPointer);
    fclose(xmlFile);
    printf("end of file reached\n");
  }
}

