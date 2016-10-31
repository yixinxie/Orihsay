

class Auxi{
private:
	Auxi(void);
	static Auxi* _instance;
	int guid;
public:
	static Auxi* instance(void);
	int getNewGUID(void);
};