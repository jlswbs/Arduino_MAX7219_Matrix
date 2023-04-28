// Wolfram 2D cellular automata //

#include "LedMatrix.h"

#define LM_CS_PIN 10

LedMatrix lm(LM_CS_PIN, false);

#define WIDTH   8
#define HEIGHT  8
#define SCR (WIDTH * HEIGHT)

  bool state[SCR];
  bool newst[SCR];
  bool rules[10] = {0,0,1,1,1,1,0,0,0,0};

uint8_t neighbors(uint16_t x, uint16_t y){
  
  uint8_t result = 0;

  if(y > 0 && state[x+(y-1)*WIDTH] == 1) result = result + 1;
  if(x > 0 && state[(x-1)+y*WIDTH] == 1) result = result + 1;
  if(x < WIDTH-1 && state[(x+1)+y*WIDTH] == 1) result = result + 1;
  if(y < HEIGHT-1 && state[x+(y+1)*WIDTH] == 1) result = result + 1;
  
  return result;
 
}

void setup(){

  lm.begin();

  state[(3)+(3)*WIDTH] = 1;
  state[(4)+(3)*WIDTH] = 1;
  state[(3)+(4)*WIDTH] = 1;
  state[(4)+(4)*WIDTH] = 1;


}

void loop(){

  for(int y = 0; y < HEIGHT; y++){
    
    for(int x = 0; x < WIDTH; x++){
           
      uint8_t totalNeighbors = neighbors(x, y);
            
      if(state[x+y*WIDTH] == 0 && totalNeighbors == 0) {newst[x+y*WIDTH] = rules[0]; lm.clearBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 0) {newst[x+y*WIDTH] = rules[1]; lm.setBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 0 && totalNeighbors == 1) {newst[x+y*WIDTH] = rules[2]; lm.clearBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 1) {newst[x+y*WIDTH] = rules[3]; lm.setBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 0 && totalNeighbors == 2) {newst[x+y*WIDTH] = rules[4]; lm.clearBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 2) {newst[x+y*WIDTH] = rules[5]; lm.setBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 0 && totalNeighbors == 3) {newst[x+y*WIDTH] = rules[6]; lm.clearBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 3) {newst[x+y*WIDTH] = rules[7]; lm.setBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 0 && totalNeighbors == 4) {newst[x+y*WIDTH] = rules[8]; lm.clearBufferPixel(x, y);}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 4) {newst[x+y*WIDTH] = rules[9]; lm.setBufferPixel(x, y);}

    }
  }
 
  memcpy(state, newst, SCR);

  lm.sendBuffer();
  delay(250);
      
      
      
}
