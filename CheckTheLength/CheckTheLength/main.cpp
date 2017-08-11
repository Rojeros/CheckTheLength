//=============================================================================
//LISTBOX CONTROLS - Copyright © 2000,2005 Ken Fitlike
//=============================================================================
//API functions used: CreateWindowEx,DefWindowProc,DispatchMessage,GetMessage,
//GetSystemMetrics,LoadImage,MessageBox,PostQuitMessage,RegisterClassEx,
//SendMessage,ShowWindow,UpdateWindow,TranslateMessage,WinMain.
//=============================================================================
//Demonstrates the creation of a listbox control.
//=============================================================================
#include <windows.h>  //include all the basics
#include <tchar.h>    //string and other mapping macros
#include <string>
#include "SearchFiles.h"
#include <algorithm>
#include <vector>
#include <atlstr.h>  
#include <shlobj.h>
#include <iostream>
#include <regex>
#pragma comment(lib, "shell32.lib")
//define an unicode string type alias
typedef std::basic_string<TCHAR> ustring;
//=============================================================================
//message processing function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int OnCreate(const HWND, CREATESTRUCT*);

//non-message function declarations
inline UINT AddString(const HWND, const wchar_t * s,int index);
HWND CreateListbox(const HWND, const HINSTANCE, DWORD, const RECT&, const int,
	const ustring&);
inline int ErrMsg(const ustring&);
HWND hList;
//setup some edit control id's
enum {
	IDCL_LISTBOX = 200
};
//=============================================================================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR pStr, int nCmd)
{

	ustring classname = _T("SIMPLEWND");
	WNDCLASSEX wcx = { 0 };  //used for storing information about the wnd 'class'

	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.lpfnWndProc = WndProc;             //wnd Procedure pointer
	wcx.hInstance = hInst;               //app instance
										 //use 'LoadImage' to load wnd class icon and cursor as it supersedes the 
										 //obsolete functions 'LoadIcon' and 'LoadCursor', although these functions will 
										 //still work. Because the icon and cursor are loaded from system resources ie 
										 //they are shared, it is not necessary to free the image resources with either 
										 //'DestroyIcon' or 'DestroyCursor'.
	wcx.hIcon = reinterpret_cast<HICON>(LoadImage(0, IDI_APPLICATION,
		IMAGE_ICON, 0, 0, LR_SHARED));
	wcx.hCursor = reinterpret_cast<HCURSOR>(LoadImage(0, IDC_ARROW,
		IMAGE_CURSOR, 0, 0, LR_SHARED));
	wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
	wcx.lpszClassName = classname.c_str();
	//the window 'class' (not c++ class) has to be registered with the system
	//before windows of that 'class' can be created
	if (!RegisterClassEx(&wcx))
	{
		ErrMsg(_T("Failed to register wnd class"));
		return -1;
	}

	int desktopwidth = GetSystemMetrics(SM_CXSCREEN);
	int desktopheight = GetSystemMetrics(SM_CYSCREEN);

	HWND hwnd = CreateWindowEx(0,                     //extended styles
		classname.c_str(),     //name: wnd 'class'
		_T("Listbox Controls"),//wnd title
		WS_OVERLAPPEDWINDOW^ WS_THICKFRAME^WS_MAXIMIZEBOX| CS_DBLCLKS,   //wnd style
		desktopwidth / 4,        //position:left
		desktopheight / 4,       //position: top
		desktopwidth / 2,        //width
		desktopheight / 2,       //height
		0,                     //parent wnd handle
		0,                     //menu handle/wnd id
		hInst,                 //app instance
		0);                    //user defined info
	if (!hwnd)
	{
		ErrMsg(_T("Failed to create wnd"));
		return -1;
	}

	ShowWindow(hwnd, nCmd);
	UpdateWindow(hwnd);
	//start message loop - windows applications are 'event driven' waiting on user,
	//application or system signals to determine what action, if any, to take. Note 
	//that an error may cause GetMessage to return a negative value so, ideally,  
	//this result should be tested for and appropriate action taken to deal with 
	//it(the approach taken here is to simply quit the application).
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}
//=============================================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return OnCreate(hwnd, reinterpret_cast<CREATESTRUCT*>(lParam));
	case WM_DESTROY:
		PostQuitMessage(0);    //signal end of application
		return 0;
	case  WM_COMMAND:
	
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				wchar_t Temp[10000];
				//HWND hwndList = GetDlgItem(hList, IDC_LISTBOX_EXAMPLE);

				// Get selected index.
				int lbItem = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);

				// Get item data.
				SendMessage(hList, LB_GETTEXT, lbItem, (LPARAM)Temp);
				SetClipboardData(CF_TEXT, Temp);
				ShellExecute(NULL, L"open", Temp, NULL, NULL, SW_SHOWDEFAULT);
				std::wstring aaa(Temp);
				
				const char* output = "Test";
				const size_t len = aaa.size() + 1;
				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
				memcpy(GlobalLock(hMem), aaa.c_str(), len);
				GlobalUnlock(hMem);
				OpenClipboard(0);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);
				CloseClipboard();
			//	std::cout<<Temp;
			}
			
		
			return 0;
	default:
		//let system deal with msg
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
//=============================================================================
inline int ErrMsg(const ustring& s)
{
	return MessageBox(0, s.c_str(), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION);
}
//=============================================================================
int OnCreate(const HWND hwnd, CREATESTRUCT *cs)
{

	
		TCHAR  my_documents[MAX_PATH];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
		if (result != S_OK){
			MessageBox(
				hwnd,
				(LPCTSTR)L"Blad lokalizacji sciezki dokumentow",
				(LPCWSTR)L"",
				MB_OK
			);
			exit(1);
		}
	
			

	//handles the WM_CREATE message of the main, parent window; return -1 to fail
	//window creation
		int desktopwidth = GetSystemMetrics(SM_CXSCREEN);
		int desktopheight = GetSystemMetrics(SM_CYSCREEN);

	RECT rc = { 10,10,(desktopwidth / 2 )- 35,desktopwidth / 4 };

	hList = CreateListbox(hwnd, cs->hInstance, 0, rc, IDCL_LISTBOX, _T(""));
	
	SearchFiles a;
	std::vector <std::wstring> * files;
	std::string name = CW2A(my_documents);
	files=a.run(name);
	
	if (files->size() == 0) {
		return 0;
	}

	//files = a.run("C:\\");
	std::vector<std::wstring>::iterator it;
	int maxVertical = 0;
	int index = 0;
	for (it = files->begin(); it != files->end(); it++) {
		
		
		const wchar_t* szName = (*it).c_str();

		AddString(hList, szName,index);
		if ((*it).size() > maxVertical)
			maxVertical = (*it).size();
		index++;
	}

	HDC hDC = GetDC(NULL);
	SIZE  size;
	GetTextExtentPoint32(hDC, LPCTSTR(L"1"), strlen("1"), &size);

	::SendMessage(hList, LB_SETHORIZONTALEXTENT, maxVertical*size.cx, 0);
	
	return 0;
}
//=============================================================================
HWND CreateListbox(const HWND hParent, const HINSTANCE hInst, DWORD dwStyle,
	const RECT& rc, const int id, const ustring& caption)
{
	
	dwStyle = WS_CHILD | WS_VISIBLE| LBS_STANDARD| WS_HSCROLL| LBS_HASSTRINGS| LBS_NOINTEGRALHEIGHT| LBS_NOTIFY;
	return CreateWindowEx(WS_EX_CLIENTEDGE,             //extended styles
		_T("listbox"),                //control 'class' name
		caption.c_str(),              //control caption
		dwStyle,                      //control style 
		rc.left,                      //position: left
		rc.top,                       //position: top
		rc.right,                     //width
		rc.bottom,                    //height
		hParent,                      //parent window handle
									  //control's ID
		reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
		hInst,                        //application instance
		0);                           //user defined info
}
//=============================================================================

inline UINT AddString(const HWND hList, const wchar_t * s,int index)
{

	return static_cast<UINT>(SendMessage(hList, LB_ADDSTRING, index, (LPARAM)s));
}
//=============================================================================