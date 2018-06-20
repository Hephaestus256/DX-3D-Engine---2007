#ifndef INC_DI80
#define INC_DI80

#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
#include "math_1d.h"

#define MAX_KEYS 256
#define MAX_MOUSE_BUTTONS 8
#define MOUSE_POS_MODE_NONE 0
#define MOUSE_POS_MODE_CLIP 1
#define MOUSE_POS_MODE_WRAP 2
#define MOUSE_DEFAULT_SENS 0.1 * (PI / 180.0)
#define MOUSE_DEFAULT_PARAMS    -PI, PI, MOUSE_DEFAULT_SENS, MOUSE_POS_MODE_WRAP,    -PI_2, PI_2, MOUSE_DEFAULT_SENS, MOUSE_POS_MODE_CLIP,    0.0, 0.0, 1.0, MOUSE_POS_MODE_NONE

// class for a physical button 
class phys_button {
	bool prev; // if key was pressed last call
public:
	bool press; // if key is currently being pressed
	bool delta; // if key has changed to 'pressed'
	bool toggle; // if key is toggled 'on'

	// initialize data for single keyboard key
	phys_button () {
		press = false;
		toggle = false;
	};

	// calculates data for single keyboard key
	void log (BYTE key)
	{
		prev = press; // saves previous key state
		press = (key & 0x80) != 0; // gets current key state
		delta = (!prev) && press; // true if newly pressed
		
		// sets the toggle flag
		if (delta)
			toggle = !toggle;
	}
};

class user_input {
	// direct input data
	LPDIRECTINPUT di;
	
	// keyboard data
	LPDIRECTINPUTDEVICE di_keyb;
	BYTE key_state[MAX_KEYS];

	// mouse data
	LPDIRECTINPUTDEVICE  di_mouse; // direct input mouse device
	DIMOUSESTATE2 mouse_state; // direct input mouse state
	int mx_raw; // raw mouse x position
	int my_raw; // raw mouse y position
	int mw_raw; // raw mouse wheel position
	int mouse_x_mode; // wrap, clip, or none to mouse x position
	int mouse_y_mode; // wrap, clip, or none to mouse y position
	int mouse_w_mode; // wrap, clip, or none to mouse wheel position
	double mouse_x_min	; // mouse x position minimum
	double mouse_x_max; // mouse x position maximum
	double mouse_y_min; // mouse y position minimum
	double mouse_y_max; // mouse y position maximum
	double mouse_w_min; // mouse wheel position minimum
	double mouse_w_max; // mouse wheel position maximum
	double mouse_x_factor; // mouse x position sensitivity
	double mouse_y_factor; // mouse y position sensitivity
	double mouse_w_factor; // mouse wheel position sensitivity
public:
	phys_button key[MAX_KEYS]; // keyboard status
	phys_button mouse_btn[MAX_MOUSE_BUTTONS]; // mouse button status
	double mouse_x; // mouse pointer x position
	double mouse_y; // mouse pointer y position
	double mouse_wheel; // mouse wheel position

	bool init (HWND wh)
	{
		// initialize direct input
		if (FAILED (DirectInput8Create (GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, NULL)))
			return false;

		// initialize the keyboard
		if (
			FAILED (di->CreateDevice (GUID_SysKeyboard, &di_keyb, NULL)) ||			
			FAILED (di_keyb->SetDataFormat (&c_dfDIKeyboard)) ||
			FAILED (di_keyb->SetCooperativeLevel (wh, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)) ||
			FAILED (di_keyb->Acquire())
		) return false;

		// initialize the mouse
		if (
			FAILED (di->CreateDevice (GUID_SysMouse, &di_mouse, NULL)) ||
			FAILED (di_mouse->SetDataFormat (&c_dfDIMouse)) ||
			FAILED (di_mouse->SetCooperativeLevel (wh, DISCL_BACKGROUND |  DISCL_NONEXCLUSIVE)) ||
			FAILED (di_mouse->Acquire ())
		) return false;
		
		// set mouse parameters/ranges to default
		mx_raw = 0;
		my_raw = 0;
		mw_raw = 0;
		set_mouse_params (MOUSE_DEFAULT_PARAMS);

		// return success
		return true;
	}

	// modes:
	// MOUSE_POS_MODE_NONE
	// MOUSE_POS_MODE_CLIP
	// MOUSE_POS_MODE_WRAP
	void set_mouse_params (
		double x_min, double x_max, double x_sens, int x_mode,
		double y_min, double y_max, double y_sens, int y_mode,
		double w_min, double w_max, double w_sens, int w_mode)
	{
		// set mouse x parameters
		mouse_x_min = x_min;
		mouse_x_max = x_max;
		mouse_x_factor = x_sens;
		mouse_x_mode = x_mode;

		// set mouse y parameters
		mouse_y_min = y_min;
		mouse_y_max = y_max;
		mouse_y_factor = y_sens;
		mouse_y_mode = y_mode;

		// set mouse wheel parameters
		mouse_w_min = w_min;
		mouse_w_max = w_max;
		mouse_w_factor = w_sens;
		mouse_w_mode = w_mode;
	}

	// constructor function
	user_input ()
	{
		di_mouse = NULL;
		di_keyb = NULL;
		di = NULL;
	}

	// destructor function that releases in reverse order
	~user_input ()
	{
		// release mouse
		if (di_mouse != NULL) {		
			di_mouse->Unacquire ();
			di_mouse->Release ();
			di_mouse = NULL;
		}

		// release keyboard
		if (di_keyb != NULL) {
			di_keyb->Unacquire ();		
			di_keyb->Release ();
			di_keyb = NULL;
		}

		// release direct input
		if (di != NULL) {
			di->Release ();
			di = NULL;
		} 
	}

	// retrieves keyb/mouse user input
	bool log_input ()
	{
		// gets keyb & mouse state from direct input
		if (
			FAILED (di_keyb->GetDeviceState (sizeof(unsigned char[MAX_KEYS]), (void*)key_state)) ||
			FAILED (di_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&mouse_state))
		) return false;

		// sets flags for all keys
		for (int k = 0; k < MAX_KEYS; k++)
			key[k].log (key_state[k]);

		// increments raw mouse values by deltas
		mx_raw += mouse_state.lX;
		my_raw += mouse_state.lY;
		mw_raw += mouse_state.lZ;
		
		// wraps, clips, or does niether to mouse x position according to mode
		if (mouse_x_mode == MOUSE_POS_MODE_WRAP)
			mx_raw = wrap_valu (double(mx_raw), mouse_x_min / mouse_x_factor, mouse_x_max / mouse_x_factor);
		else if (mouse_x_mode == MOUSE_POS_MODE_CLIP)
			mx_raw = clip_valu (double(mx_raw), mouse_x_min / mouse_x_factor, mouse_x_max / mouse_x_factor);
		
		// wraps, clips, or does niether to mouse y position according to mode
		if (mouse_y_mode == MOUSE_POS_MODE_WRAP)
			my_raw = wrap_valu (double(my_raw), mouse_y_min / mouse_y_factor, mouse_y_max / mouse_y_factor);
		else if (mouse_y_mode == MOUSE_POS_MODE_CLIP)
			my_raw = clip_valu (double(my_raw), mouse_y_min / mouse_y_factor, mouse_y_max / mouse_y_factor);

		// wraps, clips, or does niether to mouse wheel position according to mode
		if (mouse_w_mode == MOUSE_POS_MODE_WRAP)
			mw_raw = wrap_valu (double(mw_raw), mouse_w_min / mouse_w_factor, mouse_w_max / mouse_w_factor);
		else if (mouse_w_mode == MOUSE_POS_MODE_CLIP)
			mw_raw = clip_valu (double(mw_raw), mouse_w_min / mouse_w_factor, mouse_w_max / mouse_w_factor);

		tprint ("my: ", my_raw, 2);
		tprint ("min y: ", mouse_y_min / mouse_y_factor, 3);
		tprint ("max y: ", mouse_y_max / mouse_y_factor, 4);
		
		// scale raw mouse increments into custom units
		mouse_x = mx_raw * mouse_x_factor;
		mouse_y = my_raw * mouse_y_factor;
		mouse_wheel = mw_raw * mouse_w_factor;

		// sets flags for mouse buttons
		for (int b = 0; b < MAX_MOUSE_BUTTONS; b++)
			mouse_btn[b].log (mouse_state.rgbButtons[b]);

		// returns success
		return true;
	}
};


#endif // !INC_DI80