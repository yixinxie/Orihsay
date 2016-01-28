#include "../graphics/Renderer.h"
#include "../input/DXInput.h"
class G{
private:
	G(void);
	static G* _instance;
public:
	static G* instance(void);
	Renderer* renderer;
	DXInput* input;
};