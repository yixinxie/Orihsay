#include "../gameplay/MonoBehaviour.h"
class LightSource : public MonoBehaviour{
private:
public:
	int drawId;
	void awake(void);
	void update(void);
};
