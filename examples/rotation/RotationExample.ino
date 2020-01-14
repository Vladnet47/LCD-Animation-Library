#include <Frame.h>
#include <LCDMap.h>
#include <Queue.h>
#include <LiquidCrystal.h>

// associate any needed LCD pins
const int rs = 1, en = 2, d4 = 3, d5 = 4, d6 = 5, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// animation control
LCDMap control(5, 8);
const int cellsCount = 32;                  // total number of cell slots on the screen
const int maxCustomChars = 8;               // maximum number of customer characters (due to memory)
bool cells[cellsCount];                     // keeps track of which cells are filled with custom character

const char clockBase = 'B';
const char minuteHand = 'M';
const char hourHand = 'H';

void setup() {
  for (int i = 0; i < cellsCount; ++i) {
    cells[i] = false;
  }

  createClockBase(clockBase);
  createMinuteHand(minuteHand);
  createHourHand(hourHand);

  control.shiftSprite(clockBase, 12, -3);
  control.shiftSprite(minuteHand, 12, -3);
  control.shiftSprite(hourHand, 12, -3);
  

  // initialize LCD display
  lcd.begin(2, 16);
}

void loop() {
  control.rotateSprite(minuteHand, 45);
  if (control.getSpriteRot(minuteHand) == 0) {
    control.rotateSprite(hourHand, 45);
  }

  draw();
  
  delay(75);
}

// --------------------------------- description -----------------------------------
// reads all custom characters and draws them on the lcd
// ---------------------------------------------------------------------------------
void draw() {
  // can only go up to a maximum of 8
  short customCharCount = 0;

  for (int i = 0; i < cellsCount; ++i) {
    short row = (short) i / 16;
    short col = (short) i % 16;

    // get the bytes for a custom character at the current cell
    unsigned char* result = control.readCharacter(row, col);

    // erase current cell if a character will not be drawn again
    if (result == nullptr || customCharCount >= maxCustomChars) {
      if (cells[i]) {
        lcd.setCursor(col, row);
        lcd.print(" ");
        cells[i] = false;
      }
    }

    // if there are still custom characters available, create another one
    if (result != nullptr) {
      lcd.createChar(customCharCount, result);
      lcd.setCursor(col, row);
      lcd.write(byte(customCharCount));
      customCharCount++;
      cells[i] = true;
    }

    delete[] result;
  }
}

// --------------------------------- description -----------------------------------
// adds a 1-pixel ball to the lcd
// ---------------------------------------------------------------------------------
void createClockBase(char clockBase) {
  control.createSprite(clockBase, 15);
}

void createMinuteHand(char minuteHand) {
  control.createSprite(minuteHand, 15);
  control.addFrame(minuteHand, 'A');
  control.drawFrameH(minuteHand, 'A', 7, 7);
  control.drawFrameH(minuteHand, 'A', 7, 6);
  control.drawFrameH(minuteHand, 'A', 7, 5);
  control.drawFrameH(minuteHand, 'A', 7, 4);
  control.drawFrameH(minuteHand, 'A', 7, 3);
  control.drawFrameH(minuteHand, 'A', 7, 2);
  
  control.drawFrameD(minuteHand, 'A', 7, 7);
  control.drawFrameD(minuteHand, 'A', 8, 6);
  control.drawFrameD(minuteHand, 'A', 9, 5);
  control.drawFrameD(minuteHand, 'A', 10, 4);
  control.drawFrameD(minuteHand, 'A', 11, 3);
}

void createHourHand(char hourHand) {
  control.createSprite(hourHand, 15);
  control.addFrame(hourHand, 'A');
  control.drawFrameH(hourHand, 'A', 7, 7);
  control.drawFrameH(hourHand, 'A', 7, 6);
  control.drawFrameH(hourHand, 'A', 7, 5);
  control.drawFrameH(hourHand, 'A', 7, 4);
  
  control.drawFrameD(hourHand, 'A', 7, 7);
  control.drawFrameD(hourHand, 'A', 8, 6);
  control.drawFrameD(hourHand, 'A', 9, 5);
}
