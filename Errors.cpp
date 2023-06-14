#include "Errors.h"
#include <graphics.h>
#include <conio.h>

bool CastWarning(const char* msg)
{
	HWND hnd = GetHWnd();
	int len = MultiByteToWideChar(CP_ACP, 0, msg, -1, NULL, 0);
	wchar_t* wmsg = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, msg, -1, wmsg, len);
	int isDelete = MessageBox(hnd, wmsg, L"警告", MB_OKCANCEL);
	delete[] wmsg; 
	if (isDelete == IDCANCEL)
		return true;
	return false;
}
bool CastError(const char* msg)
{
	HWND hnd = GetHWnd();
	int len = MultiByteToWideChar(CP_ACP, 0, msg, -1, NULL, 0);
	wchar_t* wmsg = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, msg, -1, wmsg, len);
	int isDelete = MessageBox(hnd, wmsg, L"错误", MB_OK);
	delete[] wmsg; 
	return true;
}