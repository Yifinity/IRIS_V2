/*
 * Screen Methods: Manage Screen Drawing Logic 
 */


// Window Initialization
Window testWindow = Window(); 
WindowReminder reminderWindow = WindowReminder(); 
WindowClock clockWindow = WindowClock();
WindowWeather weatherWindow = WindowWeather();
Window* listWindows[] = {&reminderWindow, &clockWindow, &weatherWindow};

// Module Initialization
Module reminderModule = Module(green, iconReminder);
Module clockModule = Module(blue, iconClock);
Module weatherModule = Module(purple, iconWeather);

// Index of leftmost item
int selectionIdx = 0;  // Index of view's leftmost item.
int windowIdx = 1; // Idx of the selected window. 

Module listModules[] = {reminderModule, clockModule, weatherModule}; 
int numElements = sizeof(listModules) / sizeof(listModules[0]); // Get number of elements in listModules

// Organization for the three-module view. 
Module displayModules[3] = {
  listModules[(selectionIdx) % numElements],
  listModules[(selectionIdx + 1) % numElements],
  listModules[(selectionIdx + 2) % numElements]  
};


void screenStart(){
  tft.init(240, 320);
  tft.setRotation(1);  // 320x240 (Rotated)
  clearScreen();
}

void clearScreen(){
  tft.fillRect(0, 0, 320, 240, ST77XX_BLACK);
  // tft.fillRect(ST77XX_BLACK)
  // tft.fillScreen(ST77XX_BLACK); 
}

// Called upon initialization.
void drawFullMenu(){
  drawTopOutline();
  drawModules();   // Draw the three selected modules
  drawPointer();   // Show selected
  drawBluetoothText(); 
  drawBluetoothStatus(false); 
}

void drawTopOutline(){
  // Border
  tft.drawRoundRect(9, 35, 305, 35, 15, ST77XX_WHITE); 
  drawText("V0.1.0", 24, 45, 2); 
}

void drawBluetoothText(){
  drawText("Bluetooth: ", 150, 45, 2); 
}

void drawBluetoothStatus(bool isConnected){
  if(!retracted){
    if(isConnected){
      tft.fillRoundRect(270, 45, 25, 15, 3, ST77XX_GREEN);
    } else {
      tft.fillRoundRect(270, 45, 25, 15, 3, ST77XX_RED);    
    }
  }
}

void drawText(String text, int x, int y, int size) {
  tft.setCursor(x, y);
  tft.setTextSize(size);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.print(text);
}

// Used for fast erasing
void drawShadowText(String text, int x, int y, int size) {
  tft.setCursor(x, y);
  tft.setTextSize(size);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextWrap(true);
  tft.print(text);
}


void drawModules(){
  // Draw the three items in display modules
  for(int i = 0; i < 3; i++){ 
    displayModules[i].draw(i);
  }
}

void clearModules(){
  tft.fillRoundRect(9, 75, 303, 110, 15, ST77XX_BLACK);          
}

void drawPointer(){
  tft.fillTriangle(148, 200,
                   160, 192,
                   172, 200, ST77XX_WHITE);
}

// Read the CURRENT App State and update the screen as nessisary. 
void updateScreen(){
  switch (SELECTSTATE){
    case STATE_NEUTRAL:
      break; 
    case STATE_LEFT: 
      updateModules(false); 
      break;
    case STATE_RIGHT:
      updateModules(true); 
      break;
    case STATE_SELECT:
      drawSelectedWindow(windowIdx);
      // clearModules();
      // reminderWindow.drawWindow();
      break;
    case STATE_BACK:
      clearSelectedWindow(windowIdx);
      // reminderWindow.clearWindow(); 
      // drawModules(); 
      break;
  }
  SELECTSTATE = STATE_NEUTRAL; // RESET state to current
}

void drawSelectedWindow(int selected){
  clearModules();
  listWindows[selected]->drawWindow();
  
}

void clearSelectedWindow(int selected){
  listWindows[selected]->clearWindow(); 
  drawModules(); 
}

void updateModules(bool forward){
  if(forward){
    selectionIdx++; 
    windowIdx = (selectionIdx + 1) % numElements; // Updated the selected window idx

    displayModules[0] = listModules[(selectionIdx) % numElements];
    displayModules[1] = listModules[(selectionIdx + 1) % numElements];
    displayModules[2] = listModules[(selectionIdx + 2) % numElements];
  
  } else {
    selectionIdx--; 
    if(selectionIdx < 0){ // If out of bounds set selection to rightmost item.
      selectionIdx = numElements - 1; 
    }
    windowIdx = (selectionIdx + 1) % numElements; // Updated the selected window idx

    displayModules[0] = listModules[(selectionIdx) % numElements];
    displayModules[1] = listModules[(selectionIdx + 1) % numElements];
    displayModules[2] = listModules[(selectionIdx + 2) % numElements];
  };
  drawModules(); 
}
