#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include"resource.h"

using namespace std;


CONST CHAR* VALUES[] = { "THIS", "is", "my","first", "List", "Box" };

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
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
		for (int i = 0; i < sizeof(VALUES) / sizeof(VALUES[0]); i++)
		{
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)VALUES[i]);
		}

	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			/*HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			INT i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);

			const char* messages[] = {
				"You have selected 1 item - Box",
				"You have selected 2 item - Combo",
				"You have selected 3 item - first",
				"You have selected 4 item - my",
				"You have selected 5 item - is",
				"You have selected 6 item - This"
			};

			MessageBox(hwnd, messages[i], "Информация", MB_OK | MB_ICONINFORMATION);*/

			CONST INT SIZE = 256; 
			CHAR sz_buffer[SIZE] = {};
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST);

			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0); // LB_ LIST BOX GETCURSEL Get current selection
			SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
			CHAR sz_message[SIZE] = {};
			sprintf(sz_message, "You have choose #%i  with value '%s'", i + 1, sz_buffer);
			if(i != LB_ERR)MessageBox(hwnd, sz_message, "info", MB_OK | MB_ICONINFORMATION);
			else MessageBox(hwnd, "First, select the item", "info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL: EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}
