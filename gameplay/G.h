#include "../graphics/Renderer.h"
#include "../input/DXInput.h"
using namespace OriGraphics;
// G is the global variable holder.
class G{
private:
	G(void);
	static G* _instance;
public:
	static G* instance(void);
	Renderer* renderer;
	DXInput* input;
};