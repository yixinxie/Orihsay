#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include "graphics/Renderer.h"
#include "dx/directx.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "gameplay/SceneManager.h"
#include "misc/ClassFactory.h"

using namespace rapidjson;
ClassFactory classFactory;
// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);
HWND systemInit(const HINSTANCE instance, const int cmdShow, const int width, const int height);
MSG gameLoop(Renderer* renderer);
void testjson(void){
	const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
	Document d;
	d.Parse(json);

	// 2. Modify it by DOM.
	Value& s = d["stars"];
	s.SetInt(s.GetInt() + 1);

	// 3. Stringify the DOM
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	// Output {"project":"rapidjson","stars":11}
	std::cout << buffer.GetString() << std::endl;
}
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	testjson();
	// the handle for the window, filled by a function
	HWND window = systemInit(hInstance, nCmdShow, 1024, 768);
	Renderer* renderer = new DirectX11();
	G::instance()->renderer = renderer;
	renderer->init(window, 1024, 768);
	
	MSG msg = gameLoop(renderer);
	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
HWND systemInit(const HINSTANCE instance, const int cmdShow, const int width, const int height){
	HWND hWnd;

	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"Editor";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"Editor",    // name of the window class
		L"Orihsay",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		width,    // width of the window
		height,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		instance,    // application handle
		NULL);    // used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, cmdShow);
	return hWnd;
}
MSG gameLoop(Renderer* renderer){
	MSG msg;
	registerComponentClasses();
	SceneManager sceneManager;
	//renderer->initQuadBuffer();
	renderer->initInstancing();
	
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
		sceneManager.update();
		//renderer->renderFrame();
		renderer->render();
		Sleep(16); // going to fix the gameloop a bit later...
	}
	//renderer->disposeQuadBuffer();
	renderer->disposeInstancing();
	renderer->close();
	delete renderer;
	return msg;
}