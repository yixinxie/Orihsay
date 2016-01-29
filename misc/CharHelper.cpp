#include "CharHelper.h"

CharBuffer* CharHelper::loadFile(std::string File)
{
	CharBuffer* fileData = new CharBuffer;

	// open the file
	std::ifstream file(File, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	if (file.is_open())
	{
		// find the length of the file
		int Length = (int)file.tellg();

		// collect the file data
		fileData->buffer = new char[Length];
		fileData->length = Length;
		file.seekg(0, std::ios::beg);
		file.read(fileData->buffer, Length);
		file.close();

	}
	return fileData;
}
CharBuffer* CharHelper::loadTextFile(std::string File)
{
	CharBuffer* fileData = new CharBuffer;

	// open the file

	std::ifstream file(File, std::ios::in | std::ios::ate);

	// if open was successful
	if (file.is_open())
	{
		// find the length of the file
		int Length = (int)file.tellg();

		// collect the file data
		fileData->buffer = new char[Length + 1];
		fileData->length = Length + 1;
		file.seekg(0, std::ios::beg);
		file.read(fileData->buffer, Length);
		fileData->buffer[Length] = 0;
		file.close();

	}
	return fileData;
}
std::vector<std::string>& CharHelper::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> CharHelper::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}
Vector3 CharHelper::charToVec3(const char* tmp){
	vector<string> splitString = CharHelper::split((tmp), ',');
	//go->transform->position = Vector3()
	float x = std::stod(splitString[0]);
	float y = std::stod(splitString[1]);
	float z = std::stod(splitString[2]);
	return Vector3(x, y, z);
}
float CharHelper::charToFloat(const char* tmp){
	std::string _tmp = string(tmp);
	return std::stod(_tmp);
}