#include <Windows.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCndShow)
{
	MessageBox(NULL,
		"hello windows!",
		"Hello",
		MB_YESNOCANCEL |
		MB_HELP |
		MB_ICONINFORMATION | //THIS BLUE SIGNAL
		MB_SYSTEMMODAL); // MBcnopci mogno nactroiti cerez else
	return 0; // 4 BUTTON THIS MAX                                           
}