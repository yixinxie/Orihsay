#include <vector>
#include "Scene.h"
using namespace std;
class SceneManager{
private:
	vector<Scene*> scenes;
	int currentSceneIndex;
public:
	SceneManager(void);
	void update(void);
	void onDestroy(void);
};