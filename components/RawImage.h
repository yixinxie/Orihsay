#include "../gameplay/MonoBehaviour.h"
#include "../gameplay/RectTransform.h"
#include "lodepng/lodepng.h"
class RawImage : public MonoBehaviour{
private:
	RectTransform* selfRectTransform;
	IntVector2 textureDim;
	
	
	int resourceHandle;
	//int drawId;
public:
	int renderable = 1;
	void awake(void);
	void loadPNG(const IntVector2& _dim, const char* fileName);
	void onRender(void);
};

