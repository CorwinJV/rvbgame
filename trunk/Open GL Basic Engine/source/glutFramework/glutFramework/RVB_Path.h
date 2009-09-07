#ifndef RVB_PATH_H
#define RVB_PATH_H

#include "RVB_aStarNode.h"
#include <vector>
#include <string>
#include <sstream>
#include "oglGameVars.h"

#define STRAIGHT_COST 10
#define DIAG_COST 14

using namespace std;

class RVB_Map;
class rvbPath
{
private:
	vector<vector<aStarNode*>>		nodeList; // 2 Dim map of tile nodes
	vector<aStarNode*>				openList; // List of tiles that need checking

	vector<aStarNode*>				calculatedPath; // End result, path

	vector<aStarNode*>::iterator	iCalcPath; // An iterator pointing to the current index in the path

	int startX;
	int startY;
	int endX;
	int endY;
	int boardWidth;
	int boardHeight;
	RVB_Map *board;

	bool nodeCheck(int offX, int offY, int moveCost, aStarNode* potentialParentNode);
	void generatePath();

public:
	rvbPath(int startX_n, int startY_n, int endX_n, int endY_n, int width, int height, RVB_Map *newBoard);
	~rvbPath();
	
	void processPath();

	aStarNode* getCurrentPathNode(); 
	aStarNode* advancePathNode();

	bool isPathStillValid();
	void recalcPath(int startX_n, int startY_n, int endX_n, int endY_n);

	bool drawPath(double scaleFactor, int offX, int offY, int width, entityType type);				// draws the calculated path
	bool drawAll(double scaleFactor);					// draws the board and color in open/closed list
};

#endif