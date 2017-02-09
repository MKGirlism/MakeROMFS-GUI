#ifndef GUI_WINAPI_H
#define GUI_WINAPI_H

#include <windows.h>
#include <windowsx.h>
#include <objbase.h>
#include <shlobj.h>   
#include "romfs.h"
#include "ivfc.h"
#include "ByteBuffer.h"
#include <fstream>
#include <string.h>
#include <iostream>

class gui {
	public:
		gui(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
		~gui();
		void show();
	
	protected:
		HWND hwnd;
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif
