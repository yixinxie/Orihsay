//struct IntIntKVPStruct{
//
//};
//class IntIntKVP{
//	const int tableSize = 10;
//
//	int currentSize;
//	int currentSizeIndex;
//	int occupied;
//	IntIntKVPStruct* keyValuePairs;
//public:
//	//static const int* sizes;
//	inline int hash_func(const OriGUID* _key){
//		return (_key->d0 + _key->d1) % currentSize;
//	}
//	inline bool hash_comp(const OriGUID* _key, const OriGUID* internalKey){
//		return _key->d0 == internalKey->d0 && _key->d1 == internalKey->d1;
//	}
//	static void genGUID(OriGUID* guid);
//	OriGUIDHelper(void);
//	~OriGUIDHelper();
//	void clear(void);
//
//	void add(const OriGUID _key, int intVal);
//	bool tryget(const OriGUID _key, int* outVal);
//};
