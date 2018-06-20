#ifndef INC_CONTROLS 
#define INC_CONTROLS

#include <windows.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <mmsystem.h>
#include "math_2d.h"

#define KEYB_STAT_MASK_NOW 0x7f
#define KEYB_STAT_MASK_DELTA 0x80
#define KEYB_STAT_MAX_KEYS 256

#define MAX_X_MOUSE_NAV 480//200
#define MAX_Y_MOUSE_NAV MAX_X_MOUSE_NAV

class keyb_stats {
public:
	BYTE raw; // raw data
	bool now; // key is now being pressed
	bool toggle; // key is toggled
	bool delta; // key has gone from unpressed to pressed since last keyb check
};


// sets the cursor to be in windows mode
bool init_cursor_windows ()
{
	return ClipCursor (NULL) != 0;
}
	

// sets the cursor to be in navigation mode
bool init_cursor_nav ()
{
	// set cursor area
	RECT r;
	r.left = 0; r.top = 0;
	r.right = MAX_X_MOUSE_NAV; r.bottom = MAX_Y_MOUSE_NAV;	
	int cc = ClipCursor (&r);

	int scp = SetCursorPos (MAX_X_MOUSE_NAV / 2, MAX_Y_MOUSE_NAV / 2);

	return cc && scp;
}


disc_2d get_cursor_offset ()
{
	POINT p;	
	int gcp = GetCursorPos (&p);
	int scp = SetCursorPos (MAX_X_MOUSE_NAV / 2, MAX_Y_MOUSE_NAV / 2);

	return disc_2d (p.x, p.y);  	
}

	
coordd get_cursor_nav 
	(double min_x, double max_x, double min_y, double max_y, bool wrap_x, bool wrap_y)
{
	disc_2d o = get_cursor_offset ();

	coordd c (
		interpolate (double(o.x), 0, MAX_X_MOUSE_NAV, min_x, max_x),
		interpolate (double(o.y), 0, MAX_Y_MOUSE_NAV, min_y, max_y)
	);

/*
	if (wrap_x)
		c.x = wrap_valu (c.x, min_x, max_x);
	else
		c.x = clip_valu (c.x, min_x, max_x);

	if (wrap_y)
		c.y = wrap_valu (c.y, min_y, max_y);
	else
		c.y = clip_valu (c.y, min_y, max_y);
*/

	return c;
}

bool get_keyb_stats (keyb_stats* keyb)
{
	BYTE k[KEYB_STAT_MAX_KEYS];
	static keyb_stats prev_keyb[KEYB_STAT_MAX_KEYS];
	static bool prev_init = false;
	int i;

	if (!prev_init) {
		for (i = 0; i < KEYB_STAT_MAX_KEYS; i++) {
			prev_keyb[i].raw = 0;
			prev_keyb[i].now = false;
			prev_keyb[i].toggle = false;
			prev_keyb[i].delta = false;
		}

//		msg ("init");
		prev_init = true;
	}

	if (GetKeyboardState (k) != 0)
		for (i = 0; i < KEYB_STAT_MAX_KEYS - 1; i++) {			
			keyb[i].raw = k[i];
			keyb[i].now = (k[i] & 128) != 0;//127;//KEYB_STAT_MASK_NOW;
			keyb[i].toggle = (k[i] & 1) != 0;//KEYB_STAT_MASK_DELTA;
			keyb[i].delta = keyb[i].now && !prev_keyb[i].now;
			prev_keyb[i] = keyb[i];
		}
	else
		return false;
}


#endif // !INC_CONTROLS