#ifndef INC_EDIT_3D
#define INC_EDIT_3D



void check_controls (camera_3d& cam)
{
	keyb_stats keyb[256];
	get_keyb_stats (keyb);
	
	if (keyb[VK_LEFT].raw & 0x80)
		cam.pos.x -= 0.1f;

	if (keyb[VK_RIGHT].raw & 0x80)
		cam.pos.x += 0.1f;

	if (keyb[VK_UP].raw & 0x80)
		cam.pos.y += 0.1f;

	if (keyb[VK_DOWN].raw & 0x80)
		cam.pos.y -= 0.1f;

	if (keyb[65].now)
		cam.pos.z += 0.1f;

	if (keyb[65 + 25].now)
		cam.pos.z -= 0.1f;
}


#endif // !INC_EDIT_3D