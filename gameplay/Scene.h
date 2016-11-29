#include <vector>
#include "GameObject.h"
#include <unordered_map>
#include "../misc/GUID.h"
using namespace std;
class Scene{
private:
	//std::unordered_map<Ori::OriGUID, int> gameObjectIndexLookup;
	//OriGUIDHelper gameObjectIndexLookup;
	std::unordered_map<unsigned int, int> gameObjectIndexLookup;
	std::unordered_map<int, GameObject*> gameObjects;
	int guidIncre;
	int state; // indicate if the scene has started. 
	
public:
	Scene(void);
	void update(void);
	void deserialize(void);
	void serialize(void);
	void onDestroy(void);
	void newGameObjectTransform(char *objName, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8);
	void newGameObjectRectTransform(char *objName, unsigned char anchorMin, unsigned char anchorMax, short offsetMinX, short offsetMinY, short offsetMaxX, short offsetMaxY, short pivotX, short pivotY);
};