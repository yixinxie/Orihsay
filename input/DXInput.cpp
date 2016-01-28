#include "DXInput.h"
#include "../misc/Macros.h"
#include "../misc/CharHelper.h"
DXInput::DXInput(){

}

void DXInput::init(HINSTANCE hinstance, HWND hwnd){
	HRESULT result;

		// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(result)){
	}

		// Initialize the direct input interface for the keyboard.
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboardDevice, NULL);
	if (FAILED(result)){
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)){
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = keyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)){
	}

	result = keyboardDevice->Acquire();
	if (FAILED(result)){
		TRACE("keyboard acquire failed");
	}

	// Initialize the direct input interface for the mouse.
	result = directInput->CreateDevice(GUID_SysMouse, &mouseDevice, NULL);
	if (FAILED(result)){
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = mouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)){
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = mouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)){
	}

	// Acquire the mouse.
	result = mouseDevice->Acquire();
	if (FAILED(result)){
		TRACE("mouse acquire failed");
	}
}
void DXInput::dispose(){
	mouseDevice->Unacquire();
	SAFE_RELEASE(mouseDevice);

	keyboardDevice->Unacquire();
	SAFE_RELEASE(keyboardDevice);

	SAFE_RELEASE(directInput);
}
void DXInput::update(){
	HRESULT result;
	result = keyboardDevice->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result)){
		//TRACE("keyboard acquire failed");
		return;
	}
	result = mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result)){
		//TRACE("mouse acquire failed");
		return;
	}
}
IntVector2 DXInput::mousePosition(){
	
	return IntVector2(mouseState.lX, mouseState.lY);
}
bool DXInput::getMouseButton(int index){
	if (index < 0 || index >= 4)return false;
	return mouseState.rgbButtons[index];
}
bool DXInput::getKeyDown(int index){
	return keyboardState[index] & 0x80;
}