// Simplified windows interface

#ifndef INC_WIN
#define INC_WIN

#include <windows.h>
#include <stdio.h>
#include "my_data.h"

#define MAX_PATH_LEN 256 * 8
#define MAX_CLASS_NAME_LEN 64
#define CREATE_WIN_TRY_MAX 1000000
#define MAX_MENU_ITEM_LEN 64
#define MAX_MENU_DEPTH 32
#define DEFAULT_WIN_PARAMS WS_VISIBLE | WS_OVERLAPPEDWINDOW

#define WIN_CMD_NONE 0
#define WIN_CMD_CLOSE -1

LRESULT CALLBACK wnd_proc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// struct for windows messages
struct {
	HWND wh; // window handler
	bool created; // if window is finished being created
	int command; // last menu command
} _win_msg; // init


// displays windows exclamation box 
void error (LPSTR text)
{
	MessageBox (NULL, text, "Error!", MB_ICONEXCLAMATION | MB_OK);
}


// displays windows pop-up box 
void msg (LPSTR text)
{
	MessageBox (NULL, text, "Info", MB_OK);
}


// displays windows pop-up box with caption
void msg (LPSTR caption, LPSTR text)
{
	MessageBox (NULL, text, caption, MB_OK);
}


// displays windows pop-up box with number
void msg (LPSTR text, double n)
{
	char buff[256];

	sprintf_s (buff, "%s %f", text, n);
	MessageBox (NULL, buff, "Info", MB_OK);
}


void clear_win_msg ()
{
		_win_msg.created = false; 
		_win_msg.command =  WIN_CMD_NONE;
		_win_msg.wh = NULL;
}


// checks windows messages and returns if there is one
bool check_win_msg ()
{
	MSG msg;
	
	clear_win_msg ();

	if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
		return true;
	}
	else 
		return false;
}


// class for a 'windows' window
class window {
public:
	HWND wh; // 'windows' window handle
	WNDCLASSEX wc; // windows class
	HINSTANCE instance; // instance of the application
	char class_name[MAX_CLASS_NAME_LEN]; // windows class name
	window* next; // pointer for a linked list
	
	// constructor functions
	window () {}

	window (HINSTANCE inst, LPSTR title, int x1, int y1, int x2, int y2)
	{
		create (inst, title, x1, y1, x2, y2);
	}

	window (HINSTANCE inst, LPSTR title, LPSTR menu, int x1, int y1, int x2, int y2)
	{
		if (create (inst, title, x1, y1, x2, y2))
			create_menu (menu);
	}

	window (HINSTANCE inst, LPSTR title, int x1, int y1, int x2, int y2, int params)
	{
		create (inst, title, x1, y1, x2, y2, params);
	}

	bool create (HINSTANCE inst, LPSTR title, int x1, int y1, int x2, int y2)
	{
		return create (inst, title, x1, y1, x2, y2, DEFAULT_WIN_PARAMS);
	}

	// creates a Windows window
	bool create 
		(HINSTANCE inst, LPSTR title, int x1, int y1, int x2, int y2, DWORD params)
	{
		// set class vars
		instance = inst; 
		strcpy_s (class_name, title);

		// create class for window
		wc.cbSize = sizeof (WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = wnd_proc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor (NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = class_name;
		wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);

		if (!RegisterClassEx (&wc)) {
			error ("class 'Window' creation failed!");
			return false;
		}

		// create the window
		wh = CreateWindowEx (
			WS_EX_CLIENTEDGE,
			class_name,	title, params,			
			x1, y1, x2 - x1 + 1, y2 - y1 + 1,
			NULL, NULL, instance, NULL);

		if (wh == NULL) {
			error ("Window Creation Failed!");
			return false;
		}
		else
			return true;
	}

	// window destructor
	~window ()
	{
		UnregisterClass (class_name, instance);
	}

	// waits for window to finish being created
	bool wait_create ()
	{
		for (int i = 0; i < CREATE_WIN_TRY_MAX; i++)
			if (_win_msg.created && (_win_msg.wh == wh))
				return true;

		return false;
	}

	void show () 
	{
		ShowWindow(wh, SW_SHOWDEFAULT);
		UpdateWindow (wh);
	}

	void hide () 
	{
		ShowWindow(wh, SW_HIDE);
		UpdateWindow (wh);
	}

	// opens windows "open file" box and returns success/failure of user-selected file
	bool open_file (LPSTR filter, LPSTR def_ext, LPSTR fn)
	{
		char filename[MAX_PATH] = "";		
		OPENFILENAME ofn;		

		ZeroMemory (&ofn, sizeof (ofn));		
		ofn.lStructSize = sizeof (ofn);
		ofn.hwndOwner = wh;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH_LEN;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = def_ext;

		strcpy (fn, filename);
		return GetOpenFileName (&ofn) != 0;
	}

	// creates a menu for the window
	bool create_menu (LPSTR list)
	{
		int p = 0;
		int next_p = 0;
		int cmd = 1;
		HMENU menu[MAX_MENU_DEPTH];			
		menu[0] = CreateMenu ();
		
		while (p != -1) {
			char item[MAX_MENU_ITEM_LEN];
			char next_item[MAX_MENU_ITEM_LEN];			

			p = read_line (list, p, item);
			next_p = read_line (list, p, next_item);

			if (p != -1) {
				char res[MAX_MENU_ITEM_LEN];
				char next_res[MAX_MENU_ITEM_LEN];
				
				int lev = no_white_lef (item, res) / 2 + 1;
				int next_lev = no_white_lef (next_item, next_res) / 2 + 1;
				int diff = next_lev - lev;

				menu[lev] = CreatePopupMenu ();
				
				if (diff > 0) 
					AppendMenu (menu[lev - 1], MF_STRING | MF_POPUP, (UINT)menu[lev], res);
				else 
					AppendMenu (menu[lev - 1], MF_STRING | MF_POPUP, cmd++, res);
			}
		}
		
		return SetMenu (wh, menu[0]) != 0;
	}

	// see if user clicked a menu option
	int check_command ()
	{
		if (_win_msg.wh == wh) 
			return _win_msg.command;
		else 
			return WIN_CMD_NONE;
	}

	// clears the command buffer so that it does not register the command repeatedly
	void release_command ()
	{
		_win_msg.created = false; 
		_win_msg.command = WIN_CMD_NONE;
		_win_msg.wh = NULL;
	}
};


class window_pool
{
public:
	window* start; // start of linked list
	window* last; // end of linked list
	HINSTANCE instance; // instance of the application

	// constructor funtions
	window_pool ()
	{
		start = last = NULL;
	}

	window_pool (HINSTANCE inst)
	{
		init (inst);
	}

	// initialize class vars
	void init (HINSTANCE inst)
	{
		start = last = NULL;
		instance = inst;
	}

	void log_window (window* nu)
	{
		if (last == NULL)
			start = nu;
		else
			last->next = nu;

		last = nu;
	}
	
	// add a window with params to pool without a menu
	window* add (LPSTR title, int x1, int y1, int x2, int y2, int params)
	{
		window* nu = new window (instance, title, x1, y1, x2, y2, params);
		log_window (nu);
		return nu;
	}

	// add a window to pool without a menu
	window* add (LPSTR title, int x1, int y1, int x2, int y2)
	{
		window* nu = new window (instance, title, x1, y1, x2, y2);
		log_window (nu);
		return nu;
	}

	// add a window to pool with a menu
	window* add (LPSTR title, LPSTR menu, int x1, int y1, int x2, int y2)
	{
		window* nu = new window (instance, title, menu, x1, y1, x2, y2);
		log_window (nu);
		return nu;
	}
};


// checks windows messages
LRESULT CALLBACK wnd_proc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE:
			_win_msg.wh = hwnd;
			_win_msg.created = true;
			break;
		case WM_COMMAND:
			_win_msg.wh = hwnd;
			_win_msg.command = LOWORD(wParam);
			break;
		case WM_CLOSE:
			_win_msg.wh = hwnd;			
			_win_msg.command = WIN_CMD_CLOSE;
			break;
		case WM_DESTROY:
			_win_msg.wh = hwnd;
			_win_msg.command = WIN_CMD_CLOSE;
			break;
		default:
		    return DefWindowProc (hwnd, msg, wParam, lParam);
	}
//	*/
	return 0;
}


#endif // !INC_WIN

