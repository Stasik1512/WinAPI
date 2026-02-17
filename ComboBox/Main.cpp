#include <Windows.h>
#include"resource.h"

using namespace std;


CONST CHAR* ITEMS[] = { "THIS", "is", "my","first", "Combo", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdSnow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
		for (int i = 0; i < sizeof(ITEMS)/sizeof(ITEMS[0]) ; i++)
		{
			
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)ITEMS[i]);
		}
	}
	break;
	case WM_COMMAND:
		break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}
