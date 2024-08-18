// Title Variables
int titleX = -2;
int titleY = 85;  
int titleSize = 9;

void titleSlideAnimation(){
  int startX = -220; 

  while(startX <= titleX){
    // clearScreen(); 
    drawShadowTitle(startX); 
    startX += 20; 
    drawTitle(startX, titleY, titleSize);
    Serial.print("TITLE");
    Serial.println(startX);
    delay(15); 
  }  
}

// Rewrite the text, except over the previous location of the white text to "clear"
void drawShadowTitle(int followX){
  tft.setCursor(followX, titleY);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(titleSize); 
  tft.setTextWrap(true);
  tft.print("IRIS");
}

void drawTitle(int xVal, int yVal, int size) {
  tft.setCursor(xVal, yVal);
  // tft.setFont(&FreeSansBoldOblique24pt7b);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(size); 
  tft.setTextWrap(true);
  tft.print("IRIS");
}

