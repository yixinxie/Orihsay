#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include "../math/EasyMath.h"
#define TRACE(x)	do {std::stringstream s; s << x; OutputDebugStringA(s.str().c_str()); } while (0)

struct CharBuffer{
	char* buffer;
	int length;
	CharBuffer(void) :buffer(nullptr), length(0){

	}
	inline void dispose(void){
		if (buffer != nullptr)delete buffer;
	}
		

};
class CharHelper{
public:
	static CharBuffer* loadFile(std::string File);
	static CharBuffer* loadTextFile(std::string File);
	static void writeTextFile(const char* fileName, const CharBuffer& charBuffer);
	static std::vector<std::string> split(const std::string &s, char delim);
	static std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
	static Vector3 charToVec3(const char* chars);
	static Vector2 charToVec2(const char* chars);
	static float charToFloat(const char* chars);
};
