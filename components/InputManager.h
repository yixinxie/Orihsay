#include "../gameplay/MonoBehaviour.h"
#include "../math/EasyMath.h"
class InputManager : public MonoBehaviour{
private:
	IntVector2 lastMousePos;
	bool leftButtonDown;
public:
	void awake(void);
	void update(void);
};
