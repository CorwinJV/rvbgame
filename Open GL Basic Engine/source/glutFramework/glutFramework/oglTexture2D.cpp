#include "oglTexture2D.h"

oglTexture2D::oglTexture2D() 
	: mX(0), mY(0), mWidth(0), mHeight(0),
	texture(NULL), rotationInDeg(0.0)
{

}

oglTexture2D::~oglTexture2D()
{
	ilDeleteImage(1);
}

bool oglTexture2D::loadImage(std::string filename, int dWidth, int dHeight)
{
	// Generate DevIL image and make current context
	ilInit();
	ILuint image = NULL;
	ilGenImages(1, &image);
	ilBindImage(image);

	// Load in the image
	if(!ilLoadImage((ILconst_string)filename.c_str()))
	{
		std::cout << "oglTexture2D::loadImage failed to load " 	<< filename << std::endl;
		return false;
	}
	mWidth = ilGetInteger(IL_IMAGE_WIDTH);
	mHeight = ilGetInteger(IL_IMAGE_HEIGHT);

	dX = dWidth;
	dY = dHeight;\
	
	// Copy to OpenGL texture
	if(!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
	{
		std::cout << "oglTexture2D::loadImage unable to convert image"	<< " into display friendly format." << std::endl;
		return false;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, 
		GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use nice (linear) scaling 
	glTexParameteri(GL_TEXTURE_2D, 
		GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Use nice (linear) scaling 
	glTexImage2D(GL_TEXTURE_2D, 0
		, ilGetInteger(IL_IMAGE_BPP), mWidth, mHeight, 
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, 
		ilGetData()); 
	
	// Free DevIL image since we have it in a texture now 
	//ILubyte* data = ilGetData();
	ilDeleteImages(1, &image);
	ILenum Error;
	while ((Error = ilGetError()) != IL_NO_ERROR) 
	{ 
	//	std::cout << "DevIL Loading error: " << Error; 
	} 
	return true;
}

bool oglTexture2D::drawImage(int dWidth, int dHeight)
{
	if(dWidth != 0
		&& dHeight != 0)
	{
		dX = dWidth;
		dY = dHeight;
	}

	// Bind texture to current context 
	glBindTexture(GL_TEXTURE_2D, texture); 
	// Set the alpha 
	glColor4f(1.0, 1.0, 1.0, 1.0); 

	glPushMatrix();
		glRotatef(rotationInDeg, 0.0, 0.0, 1.0);
		
		// Draw texture using a quad 
		glBegin(GL_POLYGON); 
			// Top left 
			glTexCoord2f(0.0, 0.0); 
			glVertex2i(mX, mY); 
			// Top right 
			glTexCoord2f(1.0, 0.0); 
			glVertex2i(mX + dX, mY); 
			// Bottom right 
			glTexCoord2f(1.0, 1.0); 
			glVertex2i(mX + dX, mY + dY); 
			// Bottom left 
			glTexCoord2f(0.0, 1.0); 
			glVertex2i(mX, mY + dY); 
			// Finish quad drawing 
		glEnd();
	glPopMatrix();

	return true;
}

bool oglTexture2D::drawImage(int nWidth, int nHeight, int nX, int nY)
{
	// Bind texture to current context 
	glBindTexture(GL_TEXTURE_2D, texture); 
	// Set the alpha 
	glColor4f(1.0, 1.0, 1.0, 1.0); 

	glPushMatrix();
		glRotatef(rotationInDeg, nX, nY, 1.0);

		// Draw texture using a quad 
		glBegin(GL_POLYGON); 
			// Top left 
			glTexCoord2f(0.0, 0.0); 
			glVertex2i(nX, nY); 
			// Top right 
			glTexCoord2f(1.0, 0.0); 
			glVertex2i(nX + nWidth, nY); 
			// Bottom right 
			glTexCoord2f(1.0, 1.0); 
			glVertex2i(nX + nWidth, nY + nHeight); 
			// Bottom left 
			glTexCoord2f(0.0, 1.0); 
			glVertex2i(nX, nY + nHeight); 
			// Finish quad drawing 
		glEnd();
	glPopMatrix();

	return true;
}

bool oglTexture2D::drawImageFaded(double amount)
{
	// Bind texture to current context 
	glBindTexture(GL_TEXTURE_2D, texture); 
	// Set the alpha
	//glColor4f(1.0, 1.0, 1.0, 1.0); 
	glColor4f(amount, amount, amount, amount);

	glPushMatrix();
		glRotatef(rotationInDeg, 0.0, 0.0, 1.0);

		// Draw texture using a quad 
		glBegin(GL_POLYGON); 
			// Top left 
			glTexCoord2f(0.0, 0.0); 
			glVertex2i(mX, mY); 
			// Top right 
			glTexCoord2f(1.0, 0.0); 
			glVertex2i(mX + dX, mY); 
			// Bottom right 
			glTexCoord2f(1.0, 1.0); 
			glVertex2i(mX + dX, mY + dY); 
			// Bottom left 
			glTexCoord2f(0.0, 1.0); 
			glVertex2i(mX, mY + dY); 
			// Finish quad drawing 
		glEnd();
	glPopMatrix();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

bool oglTexture2D::drawImageFaded(double amount, int dWidth, int dHeight)
{
	int tmpDX = dX;
	int tmpDY = dY;

	if(dWidth != 0
		&& dHeight != 0)
	{
		dX = dWidth;
		dY = dHeight;
	}

	// Bind texture to current context 
	glBindTexture(GL_TEXTURE_2D, texture); 
	// Set the alpha
	//glColor4f(1.0, 1.0, 1.0, 1.0); 
	glColor4f(amount, amount, amount, amount);

	glPushMatrix();
		glRotatef(rotationInDeg, 0.0, 0.0, 1.0);

		// Draw texture using a quad 
		glBegin(GL_POLYGON); 
			// Top left 
			glTexCoord2f(0.0, 0.0); 
			glVertex2i(mX, mY); 
			// Top right 
			glTexCoord2f(1.0, 0.0); 
			glVertex2i(mX + dX, mY); 
			// Bottom right 
			glTexCoord2f(1.0, 1.0); 
			glVertex2i(mX + dX, mY + dY); 
			// Bottom left 
			glTexCoord2f(0.0, 1.0); 
			glVertex2i(mX, mY + dY); 
			// Finish quad drawing 
		glEnd();
	glPopMatrix();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	dX = tmpDX;
	dY = tmpDY;
	return true;
}

bool oglTexture2D::drawImageSegment(double topLeftX, double topLeftY, 
									double topRightX, double topRightY,
									double bottomLeftX, double bottomLeftY, 
									double bottomRightX, double bottomRightY, 
									double fadeAmount = 1)
{
	// Bind texture to current context 
	glBindTexture(GL_TEXTURE_2D, texture); 
	// Set the alpha
	//glColor4f(1.0, 1.0, 1.0, 1.0); 
	glColor4f(fadeAmount, fadeAmount, fadeAmount, fadeAmount);

	glPushMatrix();
		glRotatef(rotationInDeg, 0.0, 0.0, 1.0);

		// Draw texture using a quad 
		glBegin(GL_POLYGON); 
			// Top left 
			glTexCoord2f(topLeftX, topLeftY); 
			glVertex2i(mX, mY); 
			// Top right 
			glTexCoord2f(topRightX, topRightY); 
			glVertex2i(mX + dX, mY); 
			// Bottom right 
			glTexCoord2f(bottomRightX, bottomRightY); 
			glVertex2i(mX + dX, mY + dY); 
			// Bottom left 
			glTexCoord2f(bottomLeftX, bottomLeftY); 
			glVertex2i(mX, mY + dY); 
			// Finish quad drawing 
		glEnd();
	glPopMatrix();

	return true;
}

bool oglTexture2D::drawImageSegment(double topLeftX, double topLeftY, 
										double topRightX, double topRightY,
										double bottomLeftX, double bottomLeftY, 
										double bottomRightX, double bottomRightY, 
										double fadeAmount, int dWidth, int dHeight)
{
	// Bind texture to current context 
	glBindTexture(GL_TEXTURE_2D, texture); 
	// Set the alpha
	//glColor4f(1.0, 1.0, 1.0, 1.0); 
	glColor4f(fadeAmount, fadeAmount, fadeAmount, fadeAmount);

	glPushMatrix();
		glRotatef(rotationInDeg, 0.0, 0.0, 1.0);

		// Draw texture using a quad 
		glBegin(GL_POLYGON); 
			// Top left 
			glTexCoord2f(topLeftX, topLeftY); 
			glVertex2i(mX, mY); 
			// Top right 
			glTexCoord2f(topRightX, topRightY); 
			glVertex2i(mX + dWidth, mY); 
			// Bottom right 
			glTexCoord2f(bottomRightX, bottomRightY); 
			glVertex2i(mX + dWidth, mY + dHeight); 
			// Bottom left 
			glTexCoord2f(bottomLeftX, bottomLeftY); 
			glVertex2i(mX, mY + dHeight); 
			// Finish quad drawing 
		glEnd();
	glPopMatrix();

	return true;
}

bool oglTexture2D::drawImageSegment(double topLeftX, double topLeftY, 
										double topRightX, double topRightY,
										double bottomLeftX, double bottomLeftY, 
										double bottomRightX, double bottomRightY, 
										double fadeAmount, int dWidth, int dHeight,
										float r, float g, float b)
{
	// Bind texture to current context 
	glBindTexture(GL_TEXTURE_2D, texture); 
	// Set the alpha
	//glColor4f(1.0, 1.0, 1.0, 1.0); 
	glColor4f(fadeAmount, fadeAmount, fadeAmount, fadeAmount);

	glPushMatrix();
		glRotatef(rotationInDeg, 0.0, 0.0, 1.0);

		// Draw texture using a quad 
		glBegin(GL_POLYGON); 
			// Top left 
			glTexCoord2f(topLeftX, topLeftY); 
			glVertex2i(mX, mY); 
			// Top right 
			glTexCoord2f(topRightX, topRightY); 
			glVertex2i(mX + dWidth, mY); 
			// Bottom right 
			glTexCoord2f(bottomRightX, bottomRightY); 
			glVertex2i(mX + dWidth, mY + dHeight); 
			// Bottom left 
			glTexCoord2f(bottomLeftX, bottomLeftY); 
			glVertex2i(mX, mY + dHeight); 
			// Finish quad drawing 
		glEnd();
	glPopMatrix();

	return true;
}
