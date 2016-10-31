#include "Auxi.h"
#include "CharHelper.h"
Auxi* Auxi::_instance = nullptr;
Auxi::Auxi(){

}
Auxi* Auxi::instance(){
	if (_instance == nullptr){
		_instance = new Auxi();

		CharBuffer* guidStringCB = CharHelper::loadTextFile("guid.txt");
		_instance->guid = CharHelper::charToInt(guidStringCB->buffer);
		guidStringCB->dispose();
	}
	return _instance;
}
int Auxi::getNewGUID(){
	return guid;
}