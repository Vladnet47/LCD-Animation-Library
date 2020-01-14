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

void setup() {
  for (int i = 0; i < cellsCount; ++i) {
    cells[i] = false;
  }

  // initialize 8 1-pixel balls with ids (A, B, C, D, E, F, G, H)
  char id = 'A';
  for (int i = 0; i < 8; ++i) {
    createBall((char) (id + i));
    randomizeBall((char) (id + i));
  }

  // initialize LCD display
  lcd.begin(2, 16);
}

void loop() {
  char id = 'A';
  for (int i = 0; i < 8; ++i) {
    // if ball reaches the edge, they will bound off
    bounceOffBounds((char) (id + i), false);

    // move ball forward by two pixels
    control.shiftSpriteForward((char) (id + i), 2);
  }

  draw();
  
  delay(100);
}

// --------------------------------- description -----------------------------------
// causes the sprite to bounce off the boundaries if they are set
// if randomize is true, the sprite turns an additional 45 degrees in a random direction
// ---------------------------------------------------------------------------------
void bounceOffBounds(char id, bool randomize) {
  if (!control.atBounds(id)) return;
  if (control.atTopBounds(id) || control.atBotBounds(id)) {
    control.flipSpriteVer(id);
  }
  if (control.atRigBounds(id) || control.atLefBounds(id)) {
    control.flipSpriteHor(id);
  }
  if (randomize) {
    control.rotateSprite(id, random(-1, 2) * 45);
  }
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
void createBall(char id) {
  control.createSprite(id, 1);
  control.addFrame(id, 'A');
  control.drawFrameH(id, 'A', 0, 0);
  control.drawFrameD(id, 'A', 0, 0);
  control.setBounds(id, 1, 76, 8, 1);
}

void randomizeBall(char id) {
  control.shiftSprite(id, random(0, 80), random(0, 16));
  control.rotateSprite(id, random(0, 8) * 45);
}
