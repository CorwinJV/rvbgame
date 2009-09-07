#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>
#include "object.h"

class objectManager
{
protected:
	vector<object*> objectList;
	vector<object*> objectsToAdd;
	int				curObjects;
	int				allObjects;

public:
	objectManager();
	~objectManager();

	int  getCurrentNumObjects();
	int  getOverallNumObjects();
	template <class T>
	inline void addNewObject(int x, int y, int direction, objectEnum otype)
	{
		T* newObj = new T(x, y, direction, otype);
		objectsToAdd.push_back(newObj);
		allObjects++;
	}
    bool removeObjectsAt(int index);
    void update(); 
    void draw(); 
	void dumpTruck();
	void startOver();
};

#endif