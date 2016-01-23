#include "CharHelper.h"
CharBuffer* CharHelper::loadFile(std::string File)
{
	CharBuffer* fileData = new CharBuffer;

	// open the file
	std::ifstream shaderFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	if (shaderFile.is_open())
	{
		// find the length of the file
		int Length = (int)shaderFile.tellg();

		// collect the file data
		fileData->buffer = new char[Length];
		fileData->length = Length;
		shaderFile.seekg(0, std::ios::beg);
		shaderFile.read(fileData->buffer, Length);
		shaderFile.close();

	}
	return fileData;
}