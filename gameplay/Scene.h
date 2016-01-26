#include <vector>
#include "GameObject.h"
using namespace std;
class Scene{
private:
	vector<GameObject*> gameObjects;
	int state;
public:
	Scene(void);
	void update(void);
	void deserialize(void);
	void serialize(void);
	void onDestroy(void);
};