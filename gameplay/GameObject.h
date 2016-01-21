#include "MonoBehaviour.h"
#include <vector>
using namespace std;
class GameObject{
private:
	GameObject(void);
	
	vector<MonoBehaviour*> components;
	void dispose(void);
	
public:

	static GameObject* instantiate(void);
	static GameObject* create(void);
	void addComponent(MonoBehaviour* mono);

	void awake(void);
	void start(void);
	void update(void);
	void fixedUpdate(void);
};