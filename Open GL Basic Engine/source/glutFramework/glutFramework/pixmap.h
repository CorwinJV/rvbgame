#ifndef PIXMAP_H
#define PIXMAP_H

#include "consoleDefines.h"
// pixmap.h
typedef char byte;
typedef unsigned char ubyte;

struct pixmap
{
	unsigned width, height; 
	unsigned int mX, mY;
	
	ubyte *data;

	enum {RED = 0, GREEN, BLUE};

	pixmap(const char *fname);
	pixmap();
	~pixmap();

	void read(const char *fname);
	ubyte pixel_elem(int x, int y, int elem);
	ubyte *pixel_pos(int x, int y);

	void drawPixMap();
};

#endif