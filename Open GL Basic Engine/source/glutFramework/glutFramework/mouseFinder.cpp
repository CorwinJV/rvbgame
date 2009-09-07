#include "Vector3.h"
#include "oglUtility.h"
#include "GameState.h"
#include "pixmap.h"
#include "MenuSystem.h"
#include "TestState2.h"

CVector3 GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return CVector3(posX, posY, posZ);
}

CVector3 MousePos(float x, float y, float z)
{
	POINT mouse;							// Stores The X And Y Coords For The Current Mouse Position
	GetCursorPos(&mouse);					// Gets The Current Cursor Coordinates (Mouse Coordinates)
	ScreenToClient(hWnd, &mouse);

	GLfloat winX, winY, winZ;				// Holds Our X, Y and Z Coordinates

	winX = (float)mouse.x;					// Holds The Mouse X Coordinate
	winY = (float)mouse.y;					// Holds The Mouse Y Coordinate

}