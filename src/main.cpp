#include "UsefulFunctions.h"
#include <iostream>
#include <map>
#include <string.h>
#include <windows.h>

using std::string;

// Global variables
#define IDM_FILE_SAVE 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

#define IDM_EDIT_Greyscale 4
#define IDM_EDIT_FlipHorizontal 5
#define IDM_EDIT_FlipVertical 6
#define IDM_EDIT_FilterRed 7
#define IDM_EDIT_FilterGreen 8
#define IDM_EDIT_FilterBlue 9
#define IDM_EDIT_Reset 10
#define IDM_FILE_LOAD_RAW 11
#define IDM_EDIT_AD1 12
#define IDM_EDIT_AD2_INC 13
#define IDM_EDIT_AD2_DEC 14
#define IDM_EDIT_AD3_HORIZONTAL 15
#define IDM_EDIT_AD3_VERTICAL 16
#define IDM_EDIT_AD3_BOTH 17
#define IDM_EDIT_GAMMA 18
#define IDM_EDIT_ADV_MEAN 19
#define IDM_EDIT_ADV_GAUSS 20

string current_file;
string fileType;
// The main window class name.
Image *image;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
   image = new Image();

   HMENU hMenubar = CreateMenu(); // The main menu bar
   HMENU hMenu = CreateMenu();    // the file menu
   HMENU hAlter = CreateMenu();    // the edit menu
   HMENU hBrightnessSubMenu = CreatePopupMenu();  // the submenu for brightness
   HMENU hMirrorSubMenu = CreatePopupMenu();  // the submenu for mirroring
   HMENU hBlurSubMenu = CreatePopupMenu();  // the submenu for blur

   // File menu
   AppendMenuW(hMenu, MF_STRING, 2, L"&Open ppm");
   AppendMenuW(hMenu, MF_STRING, IDM_FILE_SAVE, L"&Save As");
   AppendMenuW(hMenu, MF_STRING, IDM_FILE_LOAD_RAW, L"&Load Raw");
   AppendMenuW(hMenu, MF_STRING, IDM_EDIT_Reset, L"&Reload");
   AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
   AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

   // Brightness menu
   AppendMenuW(hBrightnessSubMenu, MF_STRING, IDM_EDIT_AD2_INC, L"&Increase Brightness");
   AppendMenuW(hBrightnessSubMenu, MF_STRING, IDM_EDIT_AD2_DEC, L"&Decrease Brightness");

   // Mirror menu
   AppendMenuW(hMirrorSubMenu, MF_STRING, IDM_EDIT_AD3_HORIZONTAL, L"&Horizontal");
   AppendMenuW(hMirrorSubMenu, MF_STRING, IDM_EDIT_AD3_VERTICAL, L"&Vertical");
   AppendMenuW(hMirrorSubMenu, MF_STRING, IDM_EDIT_AD3_BOTH, L"&Both");

   // Blur menu
   AppendMenuW(hBlurSubMenu, MF_STRING, IDM_EDIT_ADV_MEAN, L"&Mean Blur");
   AppendMenuW(hBlurSubMenu, MF_STRING, IDM_EDIT_ADV_GAUSS, L"&Gaussian Blur");

   // Edit menu
   AppendMenuW(hAlter, MF_STRING, IDM_EDIT_Greyscale, L"&Greyscale");
   AppendMenuW(hAlter, MF_STRING, IDM_EDIT_FlipHorizontal, L"&Flip Horizontal");
   AppendMenuW(hAlter, MF_STRING, IDM_EDIT_FlipVertical, L"&Flip Vertical");
   AppendMenuW(hAlter, MF_STRING, IDM_EDIT_FilterRed, L"&Show Only Red");
   AppendMenuW(hAlter, MF_STRING, IDM_EDIT_FilterGreen, L"&Show Only Green");
   AppendMenuW(hAlter, MF_STRING, IDM_EDIT_FilterBlue, L"&Show Only Blue");

   AppendMenuW(hAlter, MF_SEPARATOR, 0, NULL);

   AppendMenuW(hAlter, MF_STRING, IDM_EDIT_AD1, L"&Additional Function 1 - Negative");
   AppendMenuW(hAlter, MF_STRING | MF_POPUP, (UINT_PTR)hBrightnessSubMenu, L"&Additional Function 2 - Brightness");
   AppendMenuW(hAlter, MF_STRING | MF_POPUP, (UINT_PTR)hMirrorSubMenu, L"&Additional Function 3 - Mirror");

   AppendMenuW(hAlter, MF_SEPARATOR, 0, NULL);

   AppendMenuW(hAlter, MF_STRING, IDM_EDIT_GAMMA, L"&Gamma Correction");
   AppendMenuW(hAlter, MF_STRING | MF_POPUP, (UINT_PTR)hBlurSubMenu, L"&Advanced Feature - Blur");

   // Attach the menus to the menubar
   AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
   AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hAlter, L"&Edit");

   int ret = createWindow(hInstance, nCmdShow, hMenubar);

   delete image;
   return ret;
}

void processMenu(HWND hWnd, WPARAM wParam)
{
   switch (LOWORD(wParam))
   {
   case IDM_FILE_OPEN:
      current_file = openfilename("Image (*.ppm)\0*.ppm\0\0", hWnd);
      fileType = "ppm";

      if (!image->load(current_file))
         MessageBox(NULL, _T("Error Loading image! Please try again"), _T("Load Error"), 0);
      break;
   case IDM_FILE_LOAD_RAW:
      current_file = openfilename("Image (*.raw)\0*.raw\0\0", hWnd);
      fileType = "raw";

      if (!image->loadRaw(current_file))
         MessageBox(NULL, _T("Error Loading image! Please try again"), _T("Load Error"), 0);
      break;
   case IDM_FILE_SAVE: {
      string f = saveFilename("Image (*.ppm)\0*.ppm\0\0", hWnd);
      if (!image->savePPM(f))
         MessageBox(NULL, _T("Error saving image! Please try again"), _T("Save Error"), 0);
      break;
   }
   case IDM_EDIT_Greyscale:
      image->load(current_file);
      image->greyScale(); // comment
      break;
   case IDM_EDIT_FlipHorizontal:
      // image.load(current_file);
      image->flipHorizontal();
      break;
   case IDM_EDIT_FlipVertical:
      image->flipVertically();
      break;
   case IDM_EDIT_FilterRed:
      image->load(current_file);
      image->filterRed();
      break;
   case IDM_EDIT_FilterGreen:
      image->load(current_file);
      image->filterGreen();
      break;
   case IDM_EDIT_FilterBlue:
      image->load(current_file);
      image->filterBlue();
      break;
   case IDM_EDIT_AD1:
      image->AdditionalFunction1();
      break;
   case IDM_EDIT_AD2_INC:
      image->AdditionalFunction2(image->brightness::increase);
      break;
   case IDM_EDIT_AD2_DEC:
      image->AdditionalFunction2(image->brightness::decrease);
      break;
   case IDM_EDIT_AD3_HORIZONTAL:
      image->AdditionalFunction3(image->mirror::horizontal);
      break;
   case IDM_EDIT_AD3_VERTICAL:
      image->AdditionalFunction3(image->mirror::vertical);
      break;
   case IDM_EDIT_AD3_BOTH:
      image->AdditionalFunction3(image->mirror::both);
      break;
   case IDM_EDIT_GAMMA:
      image->gammaCorrection();
      break;
   case IDM_EDIT_ADV_MEAN:
      image->AdvancedFunction(image->blur::mean);
      break;
   case IDM_EDIT_ADV_GAUSS:
      image->AdvancedFunction(image->blur::gaussian);
      break;
   case IDM_EDIT_Reset:
      if (fileType == "ppm")
         image->load(current_file);
      else
         image->loadRaw(current_file);
      break;
   case IDM_FILE_QUIT:
      SendMessage(hWnd, WM_CLOSE, 0, 0);
      break;
   }
   SetWindowPos(hWnd, NULL, 0, 0, image->getWidth() + 20, image->getHeight() + 60, SWP_NOMOVE);
   RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   PAINTSTRUCT ps;
   HDC hdc;

   RECT *rect = new RECT();

   switch (message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      rect->top = 0;
      rect->left = 0;
      rect->bottom = image->getHeight();
      rect->right = image->getWidth();
      dblBuffer(hWnd, hdc, rect, image);

      EndPaint(hWnd, &ps);
      break;
   case WM_MENUCOMMAND:
   case WM_COMMAND:

      processMenu(hWnd, wParam);

      break;
   case WM_DESTROY:
      PostQuitMessage(0);
      break;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0;
}
