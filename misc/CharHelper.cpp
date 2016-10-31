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
void CharHelper::writeTextFile(const char* fileName, const CharBuffer& charBuffer)
{
	std::ofstream file(fileName, std::ios::out | std::ios::trunc);

	if (file.is_open())
	{
		file.write(charBuffer.buffer, charBuffer.length);
		file.close();
	}
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
	std::vector<std::string> splitString = CharHelper::split((tmp), ',');
	float x = std::stod(splitString[0]);
	float y = std::stod(splitString[1]);
	float z = std::stod(splitString[2]);
	return Vector3(x, y, z);
}
Vector2 CharHelper::charToVec2(const char* tmp){
	std::vector<std::string> splitString = CharHelper::split((tmp), ',');
	float x = std::stod(splitString[0]);
	float y = std::stod(splitString[1]);

	return Vector2(x, y);
}
float CharHelper::charToFloat(const char* tmp){
	std::string _tmp = std::string(tmp);
	return std::stod(_tmp);
}
IntVector2 CharHelper::charToIntVec2(const char* tmp){
	std::vector<std::string> splitString = CharHelper::split((tmp), ',');
	int x = std::stoi(splitString[0]);
	int y = std::stoi(splitString[1]);

	return IntVector2(x, y);
}

int CharHelper::charToInt(const char* tmp){
	std::string _tmp = std::string(tmp);
	int x = std::stoi(_tmp);

	return x;
}
unsigned __int64 CharHelper::charToUInt64(const char* tmp){
	return strtoull(tmp, nullptr, 10);
}
//void CharHelper::genGUID(OriGUID* guid){
//	GUID result;
//	UuidCreate(&result);
//	memcpy(guid, &result, sizeof(GUID));
//}