#include "../gameplay/MonoBehaviour.h"
class Camera : public MonoBehaviour{
private:
	bool _isMain;
public:
	inline void setMainCamera(bool v){ _isMain = v; };
	inline bool isMain(void){ return _isMain; };
	void awake(void);
};