// -------------------------------------------------------
// Tic Tac Toe
// by Hiperblade 2012
// -------------------------------------------------------

#define B_NEXT 11
#define B_SET 10

#define sclk 4
#define mosi 5
#define cs 6
#define dc 7
#define rst 8

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#define X -1
#define O 1

#include <ST7735.h>
#include <SPI.h>

int table[] = {0, 0, 0, 0, 0, 0, 0, 0 ,0};
int current = 0;
int currentSymbol = O;
boolean run = true;

ST7735 tft = ST7735(cs, dc, mosi, sclk, rst);  

void setup(void) {
  Serial.begin(9600);
  Serial.print("hello!");
  tft.initR();               // initialize a ST7735R chip
  Serial.println("init");
  tft.writecommand(ST7735_DISPON);
  
  pinMode(B_NEXT, INPUT);
  pinMode(B_SET, INPUT);
  
  //
  newTable();
}

void loop() {
  if(run) {
    if (digitalRead(B_NEXT) == HIGH) {
      next();
      delay(200);
    }

    if (digitalRead(B_SET) == HIGH) {
      set();
      if(!run){
          printCurrent(BLACK);
      }
      delay(200);
    }
  } else {
    if ((digitalRead(B_NEXT) == HIGH) || (digitalRead(B_SET) == HIGH)){
      newTable();
      run = true;
      delay(400);
    }
  }
}

void newTable(){
  current = 0;
  for(int i = 0; i < 9; i++) {
    table[i] = 0;
  }
  printScreen();
}

void printScreen() {
  tft.fillScreen(BLACK);
     
  tft.drawLine(42, 1, 42, 127, WHITE);
  tft.drawLine(85, 1, 85, 127, WHITE);
  tft.drawLine(1, 42, 127, 42, WHITE);
  tft.drawLine(1, 85, 127, 85, WHITE);

  clearText(BLACK);
  printCurrent(YELLOW);
}

boolean set(){
  if(table[current] == 0) {
    table[current] = currentSymbol;
    if(currentSymbol == 1) {
      printO(current);
    } else {
      printX(current);
    }
    
    if(evaluate(current)) {
      if(currentSymbol == X){
        printText(1, "     X has won!", BLUE);
      } else {
        printText(1, "     O has won!", BLUE);
      }
      run = false;
    } else {
      boolean incomplete = false;
      for(int i = 0; i < 9; i++) {
        if(table[i] == 0) {
          incomplete = true;
          break;
        }
      }
      
      if(!incomplete) {
        printText(1, "        Equal!", BLUE);
        run = false;
      }
    }
    
    currentSymbol = - currentSymbol;
    return true;
  }
  return false;
}

void next(){
  printCurrent(BLACK);
  current = current + 1;
  if(current > 8) {
    current = 0;
  }
  printCurrent(YELLOW);
}

void printCurrent(uint16_t color){
  uint8_t x = getX(current);
  uint8_t y = getY(current);
  tft.drawRect(x - 16, y - 16, 33, 33, color);
}

void printText(int row, char *text, uint16_t color){
  tft.drawString(0, 128 + (row * 7), text, color);
}

void clearText(uint16_t color){
  tft.fillRect(0, 128, 128, 32, color);
}

void printO(uint8_t pos) {
   uint8_t x = getX(pos);
   uint8_t y = getY(pos);
   
   tft.drawCircle(x, y, 12, GREEN);
   tft.drawCircle(x, y, 11, GREEN);
   tft.drawCircle(x, y, 10, GREEN);
}

void printX(uint8_t pos) {
   uint8_t x = getX(pos);
   uint8_t y = getY(pos);
   tft.drawLine(x -12, y -12, x +12, y+12, RED);
   tft.drawLine(x +12, y -12, x -12, y+12, RED);

   tft.drawLine(x -12 +1, y -12, x +12, y+12 -1, RED);
   tft.drawLine(x +12 -1, y -12, x -12, y+12 -1, RED);

   tft.drawLine(x -12, y -12 +1, x +12 -1, y+12, RED);
   tft.drawLine(x +12, y -12 +1, x -12 +1, y+12, RED);
}

uint8_t getX(uint8_t pos){
  if(pos % 3 == 0){ // 0, 3, 6
    return 21;
  } else if(pos % 3 == 1){ // 1, 4, 7
    return 21 + 42;
  } else { // 2, 5, 8
    return 21 + 85;
  }
}

uint8_t getY(uint8_t pos){
  if(pos >= 6){ // 6, 7, 8
    return 21 + 85;
  } else if(pos >= 3){ // 3, 4, 5
    return 21 + 42;
  } else { // 0, 1, 2
    return 21;
  }
}

boolean evaluate(int pos){
  return (abs(table[0] + table[3] + table[6]) == 3) || (abs(table[1] + table[4] + table[7]) == 3) || (abs(table[2] + table[5] + table[8]) == 3) ||
         (abs(table[0] + table[1] + table[2]) == 3) || (abs(table[3] + table[4] + table[5]) == 3) || (abs(table[6] + table[7] + table[8]) == 3) ||
         (abs(table[0] + table[4] + table[8]) == 3) || (abs(table[2] + table[4] + table[6]) == 3);
}
