#include <Windows.h>
#include"resource.h"


CONST CHAR g_sz_INVITE[] = "Введите имя пользователя";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
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

		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_INVITE);
	}
	break;

	case WM_COMMAND:
	{

		switch (LOWORD(wParam))
		{
		case IDC_EDIT_LOGIN:
		{
			//wPARAM - Dword
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer); // считываем текст из поля логин
			if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer, g_sz_INVITE) == 0)
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)""); // пустая строка
			if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer, "") == 0)
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_INVITE);
		}
		break;
		case IDC_BUTTON_COPY:
		{
			CONST INT SIZE = 256; //SIZE BIFFER
			CHAR sz_buffer[SIZE] = {}; // text buffer
			// ТЕКСТОВЫЕ ПОЛЯ ЧТОБЫ К НИМ МОЖНО БЫЛО ОБРАЩАТСЯ
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			// Для того чтобы обратиться к какому-либо элементу окна, ему нужно отправить смс
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer); // считываем текст поля логин
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer); // загружаем текст из буфера в пассворд
		}
		break;
		case IDOK:
			MessageBox(hwnd, "Была нажата кнопка ок", "info", MB_OK | MB_ICONINFORMATION); // первые кавычки это то что  написанно в окна, вторые кавычки это то что написано В ЗаГОЛОВКЕ
			break;
		case IDCANCEL: EndDialog(hwnd, 0);
			break;
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0); // закрытие окна с помощью крестика
	}
	return FALSE;
}