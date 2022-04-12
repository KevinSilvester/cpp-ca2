//
// Created by floodd on 23/03/2022.
//

#include "Image.h"
#include <iostream>
#include <map>
#include <string.h>
#include <tchar.h>
#include <windows.h>

int createWindow(HINSTANCE &hInstance, int &nCmdShow, HMENU &hMenubar);
void dblBuffer(HWND hWnd, HDC hdc, LPRECT rcClientRect);
void processMenu(HWND hWnd, WPARAM wParam);
string saveFilename(const char *filter = "All Files (*.ppm)\0*.*\0", HWND owner = NULL);
string openfilename(const char *filter = "All Files (*.ppm)\0*.*\0", HWND owner = NULL);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void dblBuffer(HWND hWnd, HDC hdc, LPRECT rcClientRect, Image *image);
