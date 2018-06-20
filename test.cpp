// edit_3d
//		gfx_3d
//	
// win_gfx
//		win.h 
#define WINDOWS_LEAN_AND_MEAN 
#define _CRT_SECURE_NO_WARNINGS

bool _test[5] = {false, false, false, false, false};

#include "win.h"
#include "dx90.h"
#include "di80.h"
#include "gfx_3d.h"

//#include "controls.h"
//#include "edit_3d.h"
//#include "debug_3d.h"
//#include "mmsystem.h"
// getasckey

class device_ {
	int dev;
	friend class cam_;
};

class cam_ {
	double zoom;

	int render (device_ d)
	{
		d.dev = 1;
	}
};
// the camera class should have access to the dev in device_3d


int WINAPI WinMain (HINSTANCE inst, HINSTANCE prev_inst, LPSTR cmd_lin, int cmd_show)
{
	user_input keyb;
	d3d_device dev;// (inst, false);

	window win;
	if (win.create (inst, "exper", -1, -1, 1441, 901, WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE))
		//1 * WS_OVERLAPPED | 1 * WS_MINIMIZE | 1 * WS_VISIBLE))
		;//msg ("ok");
	else
		;//msg ("crap");

	dev.init_device (inst, &win, false); // WS_OVERLAPPEDWINDOW 

	//ShowWindow (win.wh, SW_HIDE);
	//UpdateWindow (win.wh);
	
	window e (inst, "exp", 500, 200, 1000, 700, WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE);
	//	int len[3] = {115, 50, 21};
//	prim_set set (&dev, len, 3, 10, 0);
//	set.insert (
//		&geo_prim (
//			geo_vert (point_3d (0.0, 0.0, 25.0), tex_coord (0.0, 0.0), 0xffffffff),
//			geo_vert (point_3d (0.0, 10.0, 25.0), tex_coord (0.0, 0.0), 0xffffffff),
//			geo_vert (point_3d (20.0, 0.0, 25.0), tex_coord (0.0, 0.0), 0xffffffff)
//		), 
//		0, 1
//	);

	if (FAILED (D3DXCreateTextureFromFile (dev.m_d3d_dev, "\\programs\\metal4.dds", &_textr)))
		error ("crap");

//			prim_group world (dev.d3d_dev, 1000);

//	world.create_test_line (line_3d (ORIG_3D, point_3d (0.0, 1.0, 1.0)), argb_valu (1.0, 1.0, 1.0, 1.0));
//	world.create_quad (point_3d (0.0, 0.0, 0.0), 10.0, 5.0, 0.0);
//			dev.cam_3d->world = &world;
	
	//	point_3d v (0.0);
//	point_3d v2 = point_3d (0.0, 10.0, 0.0) * 2.0 + v * 10.0;
//	msg (v2.print ());
//			create_horz_quad (world, point_3d (1.0, 2.0, 50.0), 0.0, 10.0, 10.0);
	// world.create_poly (NULL, 0);

//	msg (cv_string_bin (15));

	// init_cursor_nav ();
//	RECT r;
//	r.left = 0; r.top = 0;
//	r.right = 200; r.bottom = 200;	
//	ClipCursor (&r);
  
//	msg ("wrap ", wrap_valu (0.0, -500.0, -100.0));
//wrap_valu (0.0, 5.0, 100.0)
//	SetCursorPos (100, 100);
//	dev.create_font ();

//	keyb.init (dev.win_3d->wh);

//	if (!dev.win_3d->wait_create ())
//		msg ("what");

	//while (check_win_msg ());
	camera_3d cam;

	while (e.check_command () != WIN_CMD_CLOSE) { 
		if (!check_win_msg ()) {
		}
		else {
			dev.render (&e, &cam);
		}
	}
//		if (!check_win_msg ()) {
//			keyb.log_input ();			
//			dev.render ();

//			if (keyb.mouse_state.a > 30)

//			dev.cam_3d->axz = keyb.mouse_x;
//			dev.cam_3d->ayz = keyb.mouse_y;
			
//			if (keyb.key[DIK_RIGHT].press)
//				dev.cam_3d->pos.x += .1f;

//			if (keyb.key[DIK_LEFT].press)
//				dev.cam_3d->pos.x -= .1f;

//			if (keyb.key[DIK_UP].press)
//				dev.cam_3d->pos.y += .1f;

//			if (keyb.key[DIK_DOWN].press)
//				dev.cam_3d->pos.y -= .1f;

//			if (keyb.key[DIK_A].press)
//				dev.cam_3d->pos.z += .1f;

//			if (keyb.key[DIK_Z].press)
//				dev.cam_3d->pos.z -= .1f;
//		}
//	}

	return 0;
}
