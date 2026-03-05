#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include <string>
#include "resource.h"

using namespace std;

BOOL CALLBACK AddDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EditDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void LoadFileListToLB(HWND hListBox, LPCSTR szFolder = ".")  // текущий каталог
{
    // все файлы
    char szSearchPath[MAX_PATH] = {};
    GetCurrentDirectory(MAX_PATH, szSearchPath);
    strcat(szSearchPath, "\\*.*");

    WIN32_FIND_DATA FindData;
    HANDLE hFind = FindFirstFile(szSearchPath, &FindData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // текущий и родительский каталоги
            if (strcmp(FindData.cFileName, ".") != 0 &&
                strcmp(FindData.cFileName, "..") != 0)
            {
                
                if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)FindData.cFileName);
                }
            }
        } while (FindNextFile(hFind, &FindData));

        FindClose(hFind);
    }
}

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

        // Очиcтка
        SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

        // Загружаем список файлов
        LoadFileListToLB(hListBox);
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
            DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD),
                hwnd, AddDialogProc, (LPARAM)hwnd);
        }
        break;

        case IDC_BUTTON_DELET:
        {
            HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
            INT iSelected = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

            if (iSelected != LB_ERR)
            {
                // Спрашиваем подтверждение перед удалением
                if (MessageBox(hwnd, "Удалить выбранный элемент?", "Подтверждение",
                    MB_YESNO | MB_ICONQUESTION) == IDYES)
                {
                    SendMessage(hListBox, LB_DELETESTRING, iSelected, 0);
                }
            }
            else
            {
                MessageBox(hwnd, "Сначала выберите элемент для удаления",
                    "Информация", MB_OK | MB_ICONINFORMATION);
            }
        }
        break;

        case IDOK:
        {
            CONST INT SIZE = 256;
            CHAR sz_buffer[SIZE] = {};
            HWND hListBox = GetDlgItem(hwnd, IDC_LIST);

            INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
            if (i != LB_ERR)
            {
                SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
                CHAR sz_message[SIZE] = {};
                sprintf(sz_message, "Вы выбрали файл: %s", sz_buffer);
                MessageBox(hwnd, sz_message, "Информация", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBox(hwnd, "Сначала выберите файл", "Информация",
                    MB_OK | MB_ICONINFORMATION);
            }
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

// окно для добавления
BOOL CALLBACK AddDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hParentWnd;

    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        hParentWnd = (HWND)lParam;

        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

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
            CONST INT SIZE = 256;
            CHAR sz_buffer[SIZE] = {};
            HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
            GetWindowText(hEdit, sz_buffer, SIZE);

            if (strlen(sz_buffer) > 0)
            {
                HWND hListBox = GetDlgItem(hParentWnd, IDC_LIST);
                if (SendMessage(hListBox, LB_FINDSTRING, 0, (LPARAM)sz_buffer) == LB_ERR)
                    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
                else
                    MessageBox(hwnd, "Такой элемент уже существует", "Ошибка",
                        MB_OK | MB_ICONWARNING);
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

// окно для редактирования
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
        if (i != LB_ERR)
        {
            SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
            SetFocus(hEdit);
            SendMessage(hEdit, EM_SETSEL, 0, -1);
        }
    }
    break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
            if (strlen(sz_buffer) > 0)
            {
                INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                if (i != LB_ERR)
                {
                    // Проверяем, нет ли уже такого элемента
                    if (SendMessage(hListBox, LB_FINDSTRING, 0, (LPARAM)sz_buffer) == LB_ERR)
                    {
                        SendMessage(hListBox, LB_DELETESTRING, i, 0);
                        SendMessage(hListBox, LB_INSERTSTRING, i, (LPARAM)sz_buffer);
                    }
                    else
                    {
                        MessageBox(hwnd, "Такой элемент уже существует", "Ошибка",
                            MB_OK | MB_ICONWARNING);
                    }
                }
            }
            EndDialog(hwnd, 0);
            break;
        }
        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;
    }
    return FALSE;
}