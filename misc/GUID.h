#pragma once
#include <vector>
#include "Rpc.h"
#include "../math/EasyMath.h"
#define TABLE_SIZE 100;
#pragma comment(lib, "Rpcrt4.lib")
//namespace Ori{
	struct OriGUID{
		unsigned __int64 d0;
		unsigned __int64 d1;
	};
	struct OriGUID_kvp{
		OriGUID key;
		int value;
	};
	class OriGUIDHelper{
		const int tableSize = 10;
		
		int currentSize;
		int currentSizeIndex;
		int occupied;
		OriGUID_kvp* keyValuePairs;
	public:
		//static const int* sizes;
		inline int hash_func(const OriGUID* _key){
			return (_key->d0 + _key->d1) % currentSize ;
		}
		inline bool hash_comp(const OriGUID* _key, const OriGUID* internalKey){
			return _key->d0 == internalKey->d0 && _key->d1 == internalKey->d1;
		}
		static void genGUID(OriGUID* guid);
		OriGUIDHelper(void);
		~OriGUIDHelper();
		void clear(void);

		void add(const OriGUID _key, int intVal);
		bool tryget(const OriGUID _key, int* outVal);

	};

//}