#include "Renderer.h"
G *G::_instance = nullptr;
G::G(){

}
G* G::instance(){
	if (_instance == nullptr){
		_instance = new G();
	}
	return _instance;
}