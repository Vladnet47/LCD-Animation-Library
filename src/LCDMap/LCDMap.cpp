// Title: LCDMap
// Created by: Vlad Netrebchenko
// Start Date: June 28, 2019
// Last Modification: July 8, 2019

// Description:
// LCDMap acts as an overlay to the Arduino LCD screen
// that provides functionality for creating moving images.
// The user is able to create sprites which move in the x
// and y axis and take on multiple frames, which can
// be read directly into LiquidCrystal createChar() and
// promptly displayed on the screen

#include "LCDMap.h"

// takes height and width (in pixels) of LCD character
LCDMap::LCDMap(short charWidth, short charHeight) {
	charWdth = charWidth;
	charHght = charHeight;

	removeBounds();
}

LCDMap::~LCDMap() {
	sprites.clear();
}

// sets bounds that no sprite can step past
// count is number of pixels away from the first character
void LCDMap::setBounds(int top, int right, int bottom, int left) {
    topBound = top;
    rightBound = right;
    bottomBound = bottom;
    leftBound = left;
}

// sets bounds to infinity
void LCDMap::removeBounds() {
    topBound = MAX;
    rightBound = MAX;
    bottomBound = MAX;
    leftBound = MAX;
}

// returns true if sprite is touching the bounds
bool LCDMap::atBounds(char id) const {
    return atTopBounds(id) || atRigBounds(id) || atBotBounds(id) || atLefBounds(id);
}

// returns true if sprite touching top bounds
bool LCDMap::atTopBounds(char id) const {
    Sprite* sprite = sprites.get(id);

    return (sprite == nullptr) ? false : sprite->y == -topBound;
}

// returns true if sprite touching right bounds
bool LCDMap::atRigBounds(char id) const {
    Sprite* sprite = sprites.get(id);

    return (sprite == nullptr) ? false : sprite->x + sprite->size == rightBound + charWdth;
}

// returns true if sprite touching bottom bounds
bool LCDMap::atBotBounds(char id) const {
    Sprite* sprite = sprites.get(id);

    return (sprite == nullptr) ? false : sprite->y + sprite->size == bottomBound + charHght;
}

// returns true if sprite touching left bounds
bool LCDMap::atLefBounds(char id) const {
    Sprite* sprite = sprites.get(id);

    return (sprite == nullptr) ? false : sprite->x == -leftBound;
}

// returns x position of sprite with given id
int LCDMap::getSpriteX(char id) const {
	Sprite* sprite = sprites.get(id);

	return (sprite == nullptr) ? ERROR : sprite->x;
}

// returns y position of sprite with given id
int LCDMap::getSpriteY(char id) const {
	Sprite* sprite = sprites.get(id);

	return (sprite == nullptr) ? ERROR : sprite->y;
}

// returns rotation of sprite with given id
short LCDMap::getSpriteRot(char id) const {
	Sprite* sprite = sprites.get(id);

	return (sprite == nullptr) ? ERROR : sprite->rotation;
}

// moves sprite with given id by given amount
void LCDMap::shiftSprite(char id, int x, int y) {
	Sprite* sprite = sprites.get(id);
	
	if (sprite != nullptr) {
	    int shiftLeft = -leftBound - sprite->x;
	    int shiftTop = -topBound - sprite->y;
	    int shiftRight = (charWdth + rightBound) - (sprite->x + sprite->size);
	    int shiftBottom = (charHght + bottomBound) - (sprite->y + sprite->size);

	    sprite->x += (shiftLeft > x) ? shiftLeft : (shiftRight < x) ? shiftRight : x;
	    sprite->y += (shiftTop > y) ? shiftTop : (shiftBottom < y) ? shiftBottom : y;
	}
}

// rotates sprite with given id by the given rotation
// rotaion must be a multiple of 45 degrees
void LCDMap::rotateSprite(char id, short degrees) {
	if (degrees % 45 != 0) return;

	Sprite* sprite = sprites.get(id);
	
	if (sprite != nullptr) {
		sprite->rotation += (360 + (degrees % 360));
		sprite->rotation %= 360;
	}
}

// creates new sprite at position (0, 0)
bool LCDMap::createSprite(char id, short sideLength) {
	if (sprites.contains(id)) return false;

	Sprite* sprite = new Sprite();
	sprite->x = 0;
	sprite->y = 0;
	sprite->rotation = 0;
	sprite->size = sideLength;
	return sprites.add(id, sprite);
}

// removes sprite with given id
bool LCDMap::removeSprite(char id) {
	return sprites.remove(id);
}

// adds horizontal and diagonal frame to sprite with given id
bool LCDMap::addFrame(char id, char frameId) {
	Sprite* sprite = sprites.get(id);
	if (sprite == nullptr) return false;

    return sprite->framesH.add(frameId, new Frame(sprite->size)) && sprite->framesD.add(frameId, new Frame(sprite->size));
}

// draws a pixel on the horizontal frame
bool LCDMap::drawFrameH(char id, char frameId, short x, short y) {
    Sprite* sprite = sprites.get(id);
    if (sprite == nullptr) return false;

    Frame* frame = sprite->framesH.get(frameId);
    if (frame == nullptr) return false;

    return frame->drawPixel(x, y);
}

// draws a pixel on the diagonal frame
bool LCDMap::drawFrameD(char id, char frameId, short x, short y) {
    Sprite* sprite = sprites.get(id);
    if (sprite == nullptr) return false;

    Frame* frame = sprite->framesD.get(frameId);
    if (frame == nullptr) return false;

    return frame->drawPixel(x, y);
}

// erases a pixel from the horizontal frame
bool LCDMap::eraseFrameH(char id, char frameId, short x, short y) {
    Sprite* sprite = sprites.get(id);
    if (sprite == nullptr) return false;

    Frame* frame = sprite->framesH.get(frameId);
    if (frame == nullptr) return false;

    return frame->clearPixel(x, y);
}

// erases a pixel from the diagonal frame
bool LCDMap::eraseFrameD(char id, char frameId, short x, short y) {
    Sprite* sprite = sprites.get(id);
    if (sprite == nullptr) return false;

    Frame* frame = sprite->framesD.get(frameId);
    if (frame == nullptr) return false;

    return frame->clearPixel(x, y);
}

// rotates the horizontal and diagonal frame of sprite with given id
bool LCDMap::nextFrame(char id) {
	Sprite* sprite = sprites.get(id);
	if (sprite == nullptr) return false;

	sprite->framesH.rotate();
	sprite->framesD.rotate();

	return true;
}

// reads all sprites in character at given row and column (start from 0)
// returns array of bytes, each one representing a row in the custom character
unsigned char* LCDMap::readCharacter(short row, short col) {
    // initialize character array
	unsigned char* character = new unsigned char[charHght];
	for (int i = 0; i < charHght; ++i) {
		character[i] = 0;
	}

	// get character position in pixels
    int charX = col * charWdth;
    int charY = row * charHght;

    // for every sprite
	for (int i = 0; i < sprites.size(); ++i) {
	    char id = sprites.id();

	    // copy the pixels that overlap with the character
	    for (int i = 0; i < charHght; ++i) {
	        character[i] |= readSprite(id, charX, charY + i);
	    }

		sprites.rotate();
	}

	// if character is all zeroes (no sprite overlap), return a nullptr
	for (int i = 0; i < charHght; ++i) {
		if (character[i] != 0) return character;
	}

	delete[] character;
	return nullptr;
}

// returns the row bits of a custom character at given position, as it overlaps
// with the sprite with the given id
unsigned char LCDMap::readSprite(char id, int posX, int posY) const {
	Sprite* sprite = sprites.get(id);
	if (sprite == nullptr) return 0;

    // get correct frame and determine reading direction
    bool readLine;          // true = rows, false = columns
    bool readDirection;     // true = right, false = left along the line
	Frame* frame = selectFrame(id, readLine, readDirection);
	if (frame == nullptr) return 0;

	// get the correct row or column
    unsigned char* frameLine = nullptr;
    int lineNum = getLineNumber(readLine, readDirection, sprite->y, posY, sprite->size);
	if (readLine) {
        frame->getRow(lineNum, frameLine);
	} else {
	    frame->getCol(lineNum, frameLine);
	}
    if (frameLine == nullptr) return 0;

	// determine reading start index
    int readStart = getStartPosition(readDirection, sprite->x, posX, sprite->size);

    // read 5 bits from start index, to the right or to the left
	unsigned char result = readBytePiece(readDirection, readStart, frameLine, sprite->size);

	delete[] frameLine;
	return result;
}

// based on rotation of sprite,
// determines whether to use horizontal or diagonal frame
// determines whether to read rows or columns
// determines whether to read right or left along the row or column
// returns true if frame was successfully selected
Frame* LCDMap::selectFrame(char id, bool& readLine, bool& readDirection) const {
	Sprite* sprite = sprites.get(id);
	if (sprite == nullptr) return nullptr;

	switch(sprite->rotation) {
		case 0:
            readLine = true;
            readDirection = true;
            return sprite->framesH.get();
		case 45:
            readLine = true;
            readDirection = true;
			return sprite->framesD.get();
		case 90:
            readLine = false;
            readDirection = false;
            return sprite->framesH.get();
		case 135:
            readLine = false;
            readDirection = false;
            return sprite->framesD.get();
		case 180:
            readLine = true;
            readDirection = false;
            return sprite->framesH.get();
		case 225:
            readLine = true;
            readDirection = false;
            return sprite->framesD.get();
		case 270:
            readLine = false;
            readDirection = true;
            return sprite->framesH.get();
		case 315:
            readLine = false;
            readDirection = true;
            return sprite->framesD.get();
	}

	return nullptr;
}

// returns the number of the row or column to read, as it relates to the character and sprite position
int LCDMap::getLineNumber(bool line, bool direction, int spriteY, int charY, short spriteSize) const {
    if (line && direction || !line && !direction) {
        return charY - spriteY;
    } else {
        return spriteY + spriteSize - 1 - charY;
    }
}

// returns the reading start index, as it relates to the character and sprite position
int LCDMap::getStartPosition(bool direction, int spriteX, int charX, short spriteSize) const {
    if (direction) {
        return charX - spriteX;
    } else {
        return spriteX + spriteSize - 1 - charX;
    }
}

// reads cellWdth bits from startPos, inclusive
// direction true = right, false = left
// any empty slots are filled by zeroes
unsigned char LCDMap::readBytePiece(bool direction, int startPos, unsigned char* arr, int length) const {
	if (arr == nullptr) return 0;

	short dir = direction ? 1 : -1;
	unsigned char charRow = 0;
	
	// read 5 bytes into beginning of new byte [000XXXXX]
	for (int i = 0; i < charWdth; ++i) {
		int from = startPos + dir * i;

		// if from index exists in arr, copy bit at index
		// if it does not, leave a 0
		if (from >= 0 && from < length) {
			int to = charWdth - i - 1;
			int index = from / 8;
			int shift = 7 - (from % 8);

			charRow |= ((arr[index] >> shift) & 1) << to;
		}	
	}

	return charRow;
}

// returns size of sprite with given id
short LCDMap::size(char id) const {
    Sprite* sprite = sprites.get(id);
    if (sprite == nullptr) return ERROR;

    return sprite->size;
}

// returns true if sprite with given id exists, false otherwise
bool LCDMap::contains(char id) const {
	return sprites.contains(id);
}

// returns number of frames for sprite with given id
int LCDMap::frames(char id) const {
    if (sprites.get(id)->framesH.size() == sprites.get(id)->framesD.size()) {
        return sprites.get(id)->framesH.size();
    }

    return ERROR;
}