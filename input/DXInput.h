#pragma once
#include <windows.h>
#include <windowsx.h>
#include "dinput.h"
#include "../math/EasyMath.h"
//#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class DXInput{
private:
	IDirectInput8* directInput;
	IDirectInputDevice8* keyboardDevice;
	IDirectInputDevice8* mouseDevice;
	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;
public:
	DXInput(void);
	void init(HINSTANCE hinstance, HWND hwnd);
	void dispose(void);
	void update(void);
	IntVector2 mousePosition(void);
	bool getMouseButton(int index);
	bool getKeyDown(int index);
};
class KeyCode{
public:
	static const int ESCAPE = 0x01;
	static const int Num1 = 0x02;
	static const int Num2 = 0x03;
	static const int Num3 = 0x04;
	static const int Num4 = 0x05;
	static const int Num5 = 0x06;
	static const int Num6 = 0x07;
	static const int Num7 = 0x08;
	static const int Num8 = 0x09;
	static const int Num9 = 0x0A;
	static const int Num0 = 0x0B;
	static const int MINUS = 0x0C;    // - on main keyboard 

	static const int EQUALS = 0x0D;
	static const int BACK = 0x0E;   // backspace 
	static const int TAB = 0x0F;
	static const int Q = 0x10;
	static const int W = 0x11;
	static const int E = 0x12;
	static const int R = 0x13;
	static const int T = 0x14;
	static const int Y = 0x15;
	static const int U = 0x16;
	static const int I = 0x17;
	static const int O = 0x18;
	static const int P = 0x19;
	static const int LBRACKET = 0x1A;
	static const int RBRACKET = 0x1B;
	static const int RETURN = 0x1C;    // Enter on main keyboard 
	static const int LCONTROL = 0x1D;
	static const int A = 0x1E;
	static const int S = 0x1F;
	static const int D = 0x20;
	static const int F = 0x21;
	static const int G = 0x22;
	static const int H = 0x23;
	static const int J = 0x24;
	static const int K = 0x25;
	static const int L = 0x26;
	static const int SEMICOLON = 0x27;
	static const int APOSTROPHE = 0x28;
	static const int GRAVE = 0x29;    // accent grave 
	static const int LSHIFT = 0x2A;
	static const int BACKSLASH = 0x2B;
	static const int Z = 0x2C;
	static const int X = 0x2D;
	static const int C = 0x2E;
	static const int V = 0x2F;
	static const int B = 0x30;
	static const int N = 0x31;
	static const int M = 0x32;
	static const int COMMA = 0x33;
	static const int PERIOD = 0x34;    // . on main keyboard 
	static const int SLASH = 0x35;    // / on main keyboard 
	static const int RSHIFT = 0x36;
	static const int MULTIPLY = 0x37;    // * on numeric keypad 
	static const int LMENU = 0x38;    // left Alt 
	static const int SPACE = 0x39;
	static const int CAPITAL = 0x3A;
	static const int F1 = 0x3B;
	static const int F2 = 0x3C;
	static const int F3 = 0x3D;
	static const int F4 = 0x3E;
	static const int F5 = 0x3F;
	static const int F6 = 0x40;
	static const int F7 = 0x41;
	static const int F8 = 0x42;
	static const int F9 = 0x43;
	static const int F10 = 0x44;
	// too lazy to go through all of these!
	/*
	static const int NUMLOCK         0x45
	static const int NUMPAD7         0x47
	static const int NUMPAD8         0x48
	static const int NUMPAD9         0x49
	static const int SUBTRACT        0x4A    // - on numeric keypad
	static const int NUMPAD4         0x4B
	static const int NUMPAD5         0x4C
	static const int NUMPAD6         0x4D
	static const int ADD             0x4E    // + on numeric keypad
	static const int NUMPAD1         0x4F
	static const int NUMPAD2         0x50
	static const int NUMPAD3         0x51
	static const int NUMPAD0         0x52
	static const int DECIMAL         0x53    // . on numeric keypad 
	static const int OEM_102         0x56    // <> or \| on RT 102-key keyboard (Non-U.S.)
	*/
	static const int F11 = 0x57;
	static const int F12 = 0x58;
	/*
	static const int NUMPADEQUALS    0x8D    // = on numeric keypad (NEC PC98) 
	static const int AT              0x91    //                     (NEC PC98) 
	static const int COLON           0x92    //                     (NEC PC98) 
	static const int UNDERLINE       0x93    //                     (NEC PC98) 
	static const int NUMPADENTER     0x9C    // Enter on numeric keypad 
	static const int RCONTROL        0x9D
	static const int NUMPADCOMMA     0xB3    // , on numeric keypad (NEC PC98) 
	static const int DIVIDE          0xB5    // / on numeric keypad 
	static const int RMENU           0xB8    // right Alt 
	static const int HOME            0xC7    // Home on arrow keypad 
	static const int UP              0xC8    // UpArrow on arrow keypad 
	static const int PRIOR           0xC9    // PgUp on arrow keypad 
	static const int LEFT            0xCB    // LeftArrow on arrow keypad 
	static const int RIGHT           0xCD    // RightArrow on arrow keypad 
	static const int END             0xCF    // End on arrow keypad 
	static const int DOWN            0xD0    // DownArrow on arrow keypad 
	static const int NEXT            0xD1    // PgDn on arrow keypad 
	static const int INSERT          0xD2    // Insert on arrow keypad 
	static const int DELETE          0xD3    // Delete on arrow keypad 
	static const int LWIN            0xDB    // Left Windows key 
	static const int RWIN            0xDC    // Right Windows key 
	*/
};