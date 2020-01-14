#ifndef LCDMAP_H
#define LCDMAP_H

#include "Frame.h"
#include "Queue.h"

struct Sprite {
	~Sprite() { framesH.clear(); framesD.clear(); }
	int x;
	int y;
	short rotation;
	short size;
	Queue<Frame> framesH;
    Queue<Frame> framesD;
};

class LCDMap {
public:
	LCDMap(short charWidth, short charHeight);
	~LCDMap();

	void setBounds(int top, int right, int bottom, int left);
	void removeBounds();
	bool atBounds(char id) const;
	bool atTopBounds(char id) const;
	bool atRigBounds(char id) const;
	bool atBotBounds(char id) const;
	bool atLefBounds(char id) const;

	int getSpriteX(char id) const;
	int getSpriteY(char id) const;
	short getSpriteRot(char id) const;
	void shiftSprite(char id, int x, int y);
	void rotateSprite(char id, short degrees);
	bool createSprite(char id, short sideLength);
	bool removeSprite(char id);

	bool addFrame(char id, char frameId);
	bool drawFrameH(char id, char frameId, short x, short y);
	bool drawFrameD(char id, char frameId, short x, short y);
	bool eraseFrameH(char id, char frameId, short x, short y);
    bool eraseFrameD(char id, char frameId, short x, short y);
	bool nextFrame(char id);

	unsigned char* readCharacter(short row, short col);
	short size(char id) const;
	bool contains(char id) const;
	int frames(char id) const;

private:
	const int ERROR = -1;
	const int MAX = 10000;
	short charHght;
	short charWdth;
	int topBound;
	int rightBound;
	int bottomBound;
	int leftBound;
	Queue<Sprite> sprites;

	unsigned char readSprite(char id, int posX, int posY) const;
	Frame* selectFrame(char id, bool& row, bool& right) const;
    int getLineNumber(bool line, bool direction, int spriteY, int charY, short spriteSize) const;
    int getStartPosition(bool direction, int spriteX, int charX, short spriteSize) const;
	unsigned char readBytePiece(bool direction, int startPos, unsigned char* arr, int length) const;
};

#endif
