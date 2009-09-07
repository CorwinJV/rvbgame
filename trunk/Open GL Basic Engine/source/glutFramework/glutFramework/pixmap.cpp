
// pixmap.cpp

#include "pixmap.h"
#include "oglUtility.h"

#include <fstream>
#include <iostream>
#include <cstdlib>


pixmap::pixmap(const char *fname)
	: width(0), height(0), data(0), mX(0), mY(0)
{
	this->read(fname);
}

pixmap::pixmap()
	: width(0), height(0), data(0), mX(0), mY(0) {}

pixmap::~pixmap()
{
	if( data )
		delete[] data;
}

void pixmap::read(const char *fname)
{
	using namespace std;

	unsigned short planes;	// number of planes in image (must be 1) 
	unsigned short bpp;			// number of bits per pixel (must be 24)

	ifstream fin(fname, ios::in | ios::binary);
	if( !fin )
	{
		cerr << "File not found " << fname << '\n';
		exit(1);
	}
  
	fin.seekg(18, ios::cur);

	fin.read((byte *)&width, sizeof(unsigned));
	fin.read((byte *)&height, sizeof(unsigned));
	//cout << "width: " << width << " height: " << height << '\n';

	fin.read((byte *)&planes, sizeof(unsigned short));
	if( planes != 1 )
	{
		cout << "Planes from " << fname << " is not 1: " << planes << "\n";
		exit(1);
	}

	fin.read((byte *)&bpp, sizeof(unsigned short));
	if( bpp != 24 )
	{
		cout << "Bpp from " << fname << " is not 24: " << bpp << "\n";
		exit(1);
	}
	
	fin.seekg(24, ios::cur);

	unsigned size(width * height * 3);				// size of the image in bytes (3 is to RGB component).
	data = new ubyte[size];
	fin.read((byte *)data, size);

	ubyte tmp;					// temporary color storage for bgr-rgb conversion.
	for( unsigned int i(0); i < size; i += 3 )
	{
		tmp = data[i];
		data[i] = data[i+2];
		data[i+2] = tmp;
	}
	fin.close();
}

ubyte pixmap::pixel_elem(int x, int y, int elem)
{
	int pos = (y*width+x) * 3 + elem;
	return data[pos];
}

ubyte *pixmap::pixel_pos(int x, int y)
{
	int pos = (y * width + x) * 3;
	return &data[pos];
}

void pixmap::drawPixMap()
{
	// We have to change the glRasterPosition to the lower-left point
	// of our screen we'd like to start drawing. The problem is that
	// glRasterPosition takes float values from -1 to 1 which represent
	// the corners of the viewport.
    int vPort[4];
    glGetIntegerv(GL_VIEWPORT, vPort);
	
	int vpX = vPort[2];
	int vpY = vPort[3];

	// first figure out the size of a single "pixel" for x and y
	// such that if we have 1000 pixels wide, since we are using
	// a range from -1.0 to 1.0 (range of 2), we have 2/1000 for
	// a pixel width which is 0.002
	float pixelwidth  = 2/(float)vpX;
	float pixelheight = 2/(float)vpY;

	// then multiply our x and y by the pixelwidth and pixelheight
	// in order to put it in the proper position'
	// such that if we want a pixel in position 48, we multiply
	// 48 by 0.002 and endup with 0.096 for the viewport position

	float newmY = vpY - mY - height;
#ifdef pixmapWork
	std::cout << "mY = " << mY << " height = " << height << std::endl;
#endif
	float newRasterX = (float)(mX*pixelwidth) - 1.0f;
	float newRasterY = (float)(newmY*pixelheight - 1.0f);
	

	glRasterPos2f(newRasterX, newRasterY);
	glDrawPixels(this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE, data);

	glRasterPos2f(0.0f, 0.0f);
}