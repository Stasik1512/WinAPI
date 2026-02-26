#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include "resource.h"

using namespace std;

CONST CHAR* VALUES[] = { "THIS", "is", "my", "first", "List", "Box" };

BOOL CALLBACK AddDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // Обьявление дочернего окна
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ОБбьявление гл окна

BOOL CALLBACK EditDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


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
	{
		switch (LOWORD(wParam))
		{
		case IDC_LIST:
			if (HIWORD(wParam) == LBN_DBLCLK)
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, EditDialogProc, 0);
			break;
		case IDC_BUTTON_ADD:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), // Открываем дочернее окно
				hwnd,
				AddDialogProc, (LPARAM)hwnd);  // указатель на главное окно
		}
		break;
		case IDC_BUTTON_DELET:
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST);

			// индекс выбранного элемента
			INT iSelected = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

			if (iSelected != LB_ERR)
			{
				CONST INT SIZE = 256;
				CHAR sz_buffer[SIZE] = {};
				SendMessage(hListBox, LB_GETTEXT, iSelected, (LPARAM)sz_buffer);

				SendMessage(hListBox, LB_DELETESTRING, iSelected, 0);
			}
			else
			{
				MessageBox(hwnd, "Сначала выберите элемент для удаления",
					"info", MB_OK | MB_ICONINFORMATION);
			}
		}
		break;
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST);

			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
			CHAR sz_message[SIZE] = {};
			sprintf(sz_message, "You have choose #%i  with value '%s'", i + 1, sz_buffer);
			if (i != LB_ERR)
				MessageBox(hwnd, sz_message, "info", MB_OK | MB_ICONINFORMATION);
			else
				MessageBox(hwnd, "First, select the item", "info", MB_OK | MB_ICONINFORMATION);
		}
		break;

		case IDCANCEL:
		{
			EndDialog(hwnd, 0);
		}
		break;
		}
	}
	break;

	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
	}
	break;
	}
	return FALSE;
}

// Дочернее окно 
BOOL CALLBACK AddDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hParentWnd;  // указатель на главное окно

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hParentWnd = (HWND)lParam;

		// иконка
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		// фокус
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
		SetFocus(hEdit);
		return FALSE;
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			// Получаем текст из поля
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
			GetWindowText(hEdit, sz_buffer, SIZE);

			// Проверка, что текст не пустой
			if (strlen(sz_buffer) > 0)
			{
				// Добавляем текст в ListBox
				HWND hListBox = GetDlgItem(hParentWnd, IDC_LIST);
				if (SendMessage(hListBox, LB_FINDSTRING, 0, (LPARAM)sz_buffer) == LB_ERR)
					SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			}
			EndDialog(hwnd, IDOK);
		}
		break;

		case IDCANCEL:
		{
			EndDialog(hwnd, 0);
		}
		break;
		}
	}
	break;

	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
	}
	break;
	}
	return FALSE;
}

BOOL CALLBACK EditDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CONST INT SIZE = 256;
	CHAR sz_buffer[SIZE] = {};
	HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD);
	HWND hParent = GetParent(hwnd);
	HWND hListBox = GetDlgItem(hParent, IDC_LIST);
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Изменить выбранный элемент");

		INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_ADD));
		SendMessage(hEdit, EM_SETSEL, 0, -1);
		SendMessage(hEdit, EM_SETSEL, -1, -1);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (strlen(sz_buffer) > 0 && SendMessage(hListBox, LB_FINDSTRING, 0, (LPARAM)sz_buffer) == LB_ERR)
			{
				INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				SendMessage(hListBox, LB_DELETESTRING, i, 0);
				SendMessage(hListBox, LB_INSERTSTRING, i, (LPARAM)sz_buffer);
			}
		}
		case IDCANCEL:EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}