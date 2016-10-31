#include "GUID.h"
static const int* sizes = new int[20]{
	3, 7, 17, 37, 71,
		163, 353, 761, 1597, 3371,
		7013, 14591, 30293, 62851, 130363,
		270371, 560689, 1162687, 2411033, 4999559
};
OriGUIDHelper::OriGUIDHelper(){
	currentSizeIndex = 4;
	currentSize = sizes[currentSizeIndex];
	keyValuePairs = new OriGUID_kvp[currentSize];
	ZeroMemory(keyValuePairs, sizeof(OriGUID_kvp) * currentSize);
	occupied = 0;
}
OriGUIDHelper::~OriGUIDHelper(){
	delete keyValuePairs;

}
void OriGUIDHelper::clear(){
	ZeroMemory(keyValuePairs, sizeof(OriGUID_kvp) * currentSize);
	occupied = 0;

}
void OriGUIDHelper::add(const OriGUID _key, int _value){
	if (occupied == currentSize){
		// dictionary full, need to double the size;
		int oldSize = currentSize;
		currentSizeIndex++;
		currentSize = sizes[currentSizeIndex];
		OriGUID_kvp* newAlloc = new OriGUID_kvp[currentSize];
		ZeroMemory(keyValuePairs, sizeof(OriGUID_kvp) * currentSize);
		memcpy(newAlloc, keyValuePairs, sizeof(OriGUID_kvp) * oldSize);
		delete keyValuePairs;
		keyValuePairs = newAlloc;
		//rehash?

	}
	int idx = hash_func(&_key);
	while (true){
		if (keyValuePairs[idx].key.d0 == 0 &&
			keyValuePairs[idx].key.d1 == 0 ){
			keyValuePairs[idx].key.d0 = _key.d0;
			keyValuePairs[idx].key.d1 = _key.d1;
			keyValuePairs[idx].value = _value;
			occupied++;
			break;
		}
		idx++;
	}
}
bool OriGUIDHelper::tryget(const OriGUID _key, int* outVal){
	int idx = hash_func(&_key);
	int originalIdx = idx;
	*outVal = 0;
	while (true){
		if (hash_comp(&(keyValuePairs[idx].key), &_key)){

			*outVal = keyValuePairs[idx].value;
			return true;
		}
		idx++;
		if (idx == originalIdx){
			// full
			break;
		}
	}
	return false;
}
void OriGUIDHelper::genGUID(OriGUID* guid){
	GUID result;
	UuidCreate(&result);
	memcpy(guid, &result, sizeof(GUID));
}