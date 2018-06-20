// Creates an generic OS-independent set of types and functions for windows
#ifndef INC_WIN_3D
#define INC_WIN_3D

#include "win.h"
typedef HWND os_resource; // defines OS resource


inline HWND get_os_resource (window* w)
{
	return w->wh;
}


#endif // !INC_WIN_3D