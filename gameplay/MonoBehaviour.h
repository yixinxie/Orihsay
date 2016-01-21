class MonoBehaviour{
private:
	

public:
	MonoBehaviour();
	virtual void awake(void);
	virtual void start(void);
	virtual void update(void);
	virtual void lateUpdate(void);
	virtual void fixedUpdate(void);	
	virtual void onDestroy(void);
};