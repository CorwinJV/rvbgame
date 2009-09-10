#ifndef OGLTEXTURE2D_H
#define OGLTEXTURE2D_H

#include <IL\il.h>
#include <IL\ilu.h>
#include <iostream>
#include <string>
#include "oglUtility.h"


class oglTexture2D
{
public:
	oglTexture2D();
	~oglTexture2D();
	// Copy Constructor:
	oglTexture2D(const oglTexture2D& p) 
	{
		mX = p.mX;
		mY = p.mY;
		dX = p.dX;
		dY = p.dY;
		texture = p.texture;
		mWidth = p.mWidth;
		mHeight = p.mHeight;
	}

	bool loadImage(std::string, int dWidth, int dHeight);
	bool drawImage(int dWidth = 0, int dHeight = 0);
	bool drawImage(int nWidth, int nHeight, int nX, int nY);
	bool drawImageFaded(double amount);
	bool drawImageFaded(double amount, int dWidth, int dHeight);
	bool drawImageFaded(double amount, int dWidth, int dHeight, int nX, int nY);
	bool oglTexture2D::drawImageSegment(double topLeftX, double topLeftY, 
										double topRightX, double topRightY,
										double bottomLeftX, double bottomLeftY, 
										double bottomRightX, double bottomRightY, 
										double fadeAmount);

	bool oglTexture2D::drawImageSegment(double topLeftX, double topLeftY, 
										double topRightX, double topRightY,
										double bottomLeftX, double bottomLeftY, 
										double bottomRightX, double bottomRightY, 
										double fadeAmount, int dWidth, int dHeight);

	bool oglTexture2D::drawImageSegment(double topLeftX, double topLeftY, 
										double topRightX, double topRightY,
										double bottomLeftX, double bottomLeftY, 
										double bottomRightX, double bottomRightY, 
										double fadeAmount, int dWidth, int dHeight,
										float r, float g, float b);

	int mX;
	int mY;
	int dX;
	int dY;

	float rotationInDeg;

private:
	GLuint texture;
	int mWidth;
	int mHeight;
};

#endif