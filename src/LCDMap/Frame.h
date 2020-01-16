#ifndef LCDIMAGE_H
#define LCDIMAGE_H

using namespace std;

class Frame {
public:
	Frame(short sideLength);
	~Frame();

	bool getPixel(short x, short y) const;
	bool drawPixel(short x, short y);
	bool clearPixel(short x, short y);
	void clear();
	short size() const;
	int getRow(short row, unsigned char*& arr) const;
	int getCol(short col, unsigned char*& arr) const;

private:
	short length;
	int bytes;
	unsigned char* pixels;

	bool setPixel(short x, short y, bool value);
	int toOffset(short x, short y) const;
	int toIndex(short x, short y) const;
	int toPixel(short x, short y) const;
	bool validPixel(short x, short y) const;
	int readLine(bool horizontal, short line, unsigned char*& arr) const;
};

#endif
