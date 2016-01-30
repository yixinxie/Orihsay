#include "../gameplay/MonoBehaviour.h"
class CubeMesh : public MonoBehaviour{
private:
	int drawId;
public:
	void awake(void);
	void update(void);
};
