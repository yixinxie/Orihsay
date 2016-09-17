#include "G.h"
G* G::_instance = nullptr;
G::G(){

}
G* G::instance(){
	if (_instance == nullptr){
		_instance = new G();
		_instance->input = nullptr;
		_instance->renderer = nullptr;
	}
	return _instance;
}