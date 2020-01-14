// Title: Frame
// Created by: Vlad Netrebchenko
// Start Date: June 29, 2019
// Last Modification: July 5, 2019

// Description:
// Frame stores a user-generated image as a series of bytes.
// The user is able to change the state of individual pixels on
// the image, which are either on or off. The user can also
// retrieve rows and columns from the image.

#include "Frame.h"

// initializes image to given height and width, or a default size of 1 x 1
// if height or width are too small
Frame::Frame(short sideLength) {
	length = (sideLength < 1) ? 1 : sideLength;
	
	// initialize byte array
	bytes = toIndex(length - 1, length - 1) + 1;
	pixels = new unsigned char[bytes];
	clear();
}

Frame::~Frame() {
	delete[] pixels;
}

// draws pixel at given position
// returns false if position is out of bounds
bool Frame::drawPixel(short x, short y) {
	return setPixel(x, y, true);
}

// clears pixel at given position
// returns false if position is out of bounds
bool Frame::clearPixel(short x, short y) {
	return setPixel(x, y, false);
}

// clears all pixels
void Frame::clear() {
	// set all bytes to 0
	for (int i = 0; i < bytes; i++) {
		pixels[i] = 0;
	}	
}

// returns side length of image
short Frame::size() const {
	return length;
}

// reads requested row into given byte array
// returns size of resulting byte array
// returns 0 and sets array to nullptr if row out of bounds
int Frame::getRow(short row, unsigned char*& arr) const {
	return readLine(true, row, arr);
}

// reads requested column into given byte array
// returns size of resulting byte array
// returns 0 and sets array to nullptr if column out of bounds
int Frame::getCol(short col, unsigned char*& arr) const {
	return readLine(false, col, arr);
}

// reads bytes of requested row or column into given byte array
// returns size of resulting byte array
// returns 0 and sets array to nullptr if line out of bounds
int Frame::readLine(bool horizontal, short line, unsigned char*& arr) const {
	if (!validPixel(0, line)) {
	    arr = nullptr;
		return 0;
	}

	// create empty array
	int byteNum = toIndex(length - 1, 0) + 1;
	arr = new unsigned char[byteNum];
	for (int i = 0; i < byteNum; ++i) {
		arr[i] = 0;
	}

	// copy bits from image row or column into array
	for (int i = 0; i < length; ++i) {
		short pixel = horizontal ? getPixel(i, line) : getPixel(line, i);

		if (pixel) {
			arr[toIndex(i, 0)] |= 1 << toOffset(i, 0);
		}
	}
	
	return byteNum;
}

// returns value of pixel at given position
// returns false if position is out of bounds
bool Frame::getPixel(short x, short y) const {
	if (!validPixel(x, y)) return 0;

	return (pixels[toIndex(x, y)] & (1 << toOffset(x, y))) != 0;
}

// sets pixel at given position to given value (0 or 1)
// returns false if position is out of bounds
bool Frame::setPixel(short x, short y, bool value) {
	if (!validPixel(x, y)) return false;

	int index = toIndex(x, y);
	int offset = toOffset(x, y);

	// set value at index + offset to 0 or 1
	if (value) {
		pixels[index] |= 1 << offset; 
	} else {
		pixels[index] &= ~(1 << offset);
	}

	return true;
}

// returns false if position is out of bounds, true otherwise
bool Frame::validPixel(short x, short y) const {
	return y >= 0 && y < length && x >= 0 && x < length;
}

// returns the byte offset of the given position
int Frame::toOffset(short x, short y) const {
	return 7 - (toPixel(x, y) % 8);
}

// returns the byte index of the given position
int Frame::toIndex(short x, short y) const {
	return toPixel(x, y) / 8;
}

// returns the cumulative index of the given position
// counts pixels in row-order (pixel at [1,1] = 1, pixel at [1,2] = 2...)
int Frame::toPixel(short x, short y) const {
	return y * length + x;
}