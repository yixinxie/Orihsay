#pragma once
#include <fstream>
#include <sstream>
#define TRACE(x)	do {std::stringstream s; s << x; OutputDebugStringA(s.str().c_str()); } while (0)
struct CharBuffer{
	char* buffer;
	int length;
	CharBuffer(void) :buffer(nullptr), length(0){

	}
		

};
class CharHelper{
public:
	static CharBuffer* loadFile(std::string File);
};