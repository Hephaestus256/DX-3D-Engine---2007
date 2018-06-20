#ifndef INC_GFX_3D
#define INC_GFX_3D

#include "my_data.h"
#include "math_1d.h"
#include "math_2d.h"
#include "math_3d.h"
#include "dx90.h"

rend_geo_bank* _bank;

// holds set of homogeneous primitives
class prim_set
{
	linked_list* m_geo; // n layers of geometry
	renderer_3d* m_rend_3d; // 3d renderer class
	int* m_layer_len; // list of the lengths of n layers
	int m_layer_ct; // layer count
	int m_bank_size; // size of chunks of primitives
	DWORD m_prim_mode; // type of primitives
	int m_prim_size; // size of primitive in bytes

	// calculates zero-based bank and offset of zero-based nth primitive 
	void calc_loc (int n, int& bank, int& offset)
	{
		bank = n / m_bank_size;
		offset = n % m_bank_size;
	}

public:

	prim_set () {}

	prim_set (renderer_3d* rend_3d, int* layer_len, int layer_ct, int bank_size, DWORD prim_mode)
	{
		m_geo = new linked_list[layer_ct];
		m_rend_3d = rend_3d;
		m_layer_len = layer_len;
		m_layer_ct = layer_ct;
		m_bank_size = bank_size;
		m_prim_mode = prim_mode;
		m_prim_size = sizeof (geo_prim);

		bool stat = true;

		rend_geo_bank_ptr bank = m_rend_3d->create_geo_bank (100, sizeof (geo_prim), false);
		m_geo[0].append (&bank, sizeof (rend_geo_bank_ptr));
		//_bank = bank;
	}

	bool add (geo_prim* prim, int prim_ct)
	{
		return true;
	}

	bool insert (geo_prim* prim, int start, int prim_ct)
	{
		rend_geo_bank_ptr* bank = (rend_geo_bank_ptr*)m_geo[0].get_data (0, 0);
		m_rend_3d->modify_geo_bank (*bank, prim, 0, 1, sizeof(geo_prim));

		return true;
	}
};


class world_3d {
	linked_list set;
};

// calculates either u or v texture coordinates
float calc_tex_coord (xy_pair axis_m, point_3d vert, float min, double tex_dim)
{
	line_3d trace = line_3d (vert, axis_m);		
	vect_3dd v = trace.vect ();
	int mc = v.major_comp ();
	point_3dd axis_orig;

	if (mc == POINT_3D_X)
		axis_orig = trace.func_x (0.0);
	else if (mc == POINT_3D_Y)
		axis_orig = trace.func_y (0.0);
	else
		axis_orig = trace.func_z (0.0);

	if (_test[0])
		tprint ("dist ", axis_orig.dist_3d (cv_point_3dd (vert)), 7);

	//return min + float(frac (axis_orig.dist_3d (cv_point_3dd (vert)) / tex_dim));
	return float (axis_orig.dist_3d (cv_point_3dd (vert)) / 10.0);
}


// calculates texture coordinates
point_2d calc_tex_coords (xy_pair um, xy_pair vm, point_3d vert, point_2d min, xy_pair tex_dim)
{
	return point_2d (
		calc_tex_coord (um, vert, min.x, tex_dim.x),
		calc_tex_coord (vm, vert, min.y, tex_dim.y)
	);
}


void illum_poly (geo_vert* vert, int ct, D3DCOLOR illum)
{
	for (int i = 0; i < ct; i++)
		vert[i].illum = illum;
}


void skin_poly (geo_vert* vert, int ct, xy_pair tex_dim, double rot_a, coordd rot_c)
{
	plane_3d plane = plane_3d (vert[0].pos, vert[1].pos, vert[2].pos);
	m_3d u_axis_m, v_axis_m;

/*
	if (nan (plane.mx)) {
		u_axis_m = SLOPE_3D_X_AXIS;
		v_axis_m = SLOPE_3D_Z_AXIS;
	}
	else {
		m_3d m = plane.rel_slopes_3d ();
		u_axis_m = xy_pair (m.x, 0.0);
		v_axis_dir = xy_pair (
	}
*/
	plane.rel_slopes_3d (u_axis_m, v_axis_m);

	point_2d min = point_2d (0.0, 0.0);

	for (int i = 0; i < ct; i++) 
			vert[i].tex = calc_tex_coords (u_axis_m, v_axis_m, vert[i].pos, min, tex_dim);
}

// creates a quadrilateral in world
// vert component of quad extends from 'start' in (+)y direction to 'start' + 'h'
// x-z component of quad extends 'w' units at angle 'angle'
// 'angle' of 0.0 PI makes quad face toward (-)z direction
// 
// 1---2    4
// |  /    /|
// | /    / |
// |/    /  |
// 0    3---5
/*
bool create_horz_quad (prim_group& world, point_3d bot_lef, double azx, double wid, double len)
{
	geo_prim prims[2];

	prims[0].vert[0].pos = bot_lef + point_3d (0.0, 0.0, 0.0);
	prims[0].vert[1].pos = bot_lef + point_3d (0.0, len, 0.0);
	prims[0].vert[2].pos = bot_lef + point_3d (wid, len, 0.0);
	
	prims[1].vert[0].pos = prims[0].vert[0].pos;
	prims[1].vert[1].pos = prims[0].vert[2].pos;
	prims[1].vert[2].pos = bot_lef + point_3d (wid, 0.0, 0.0);

	skin_poly (prims[0].vert, 3, xy_pair (10.0, 10.0), 0.0, ORIG_2D);
	skin_poly (prims[1].vert, 3, xy_pair (10.0, 10.0), 0.0, ORIG_2D);

	illum_poly (prims[0].vert, 3, 0xffffffff);
	illum_poly (prims[1].vert, 3, 0xffffffff);

	for (int i = 0; i < 3; i++)
		prims[0].vert[i].tex = prims[0].vert[i].vert.xy () / 10.0;

	for (int i = 0; i < 3; i++)
		prims[1].vert[i].tex = prims[1].vert[i].vert.xy () / 10.0;

	tprint (prims[0].vert[0].tex.print (), 9);
	tprint (prims[0].vert[1].tex.print (), 10);
	tprint (prims[0].vert[2].tex.print (), 11);

	world.create_geo_prims (prims, 2);
//	world.create_geo_prims (&prims[1], 1);

	return true;

	geo_vert vert[4];
	point_3d d_len = cv_point_3d (vect_3dd (azx) * len);
	point_3d d_wid = cv_point_3d (vect_3dd (azx + PI_2) * wid);

	vert[0].pos = bot_lef;
	vert[1].pos = bot_lef + d_wid;
	vert[2].pos = bot_lef + d_wid + d_len;
	vert[3].pos = bot_lef + d_len;

	illum_poly (vert, 4, 0xff808080);
	skin_poly (vert, 4, xy_pair (10.0, 10.0), 0.0, coordd (0.0, 0.0));
	world.create_poly (vert, 4);

	return true;

}
*/

/*
bool create_quad (prim_group& world, point_3d start, double ax)
{
	geo_vert vert[4];

	 
	//return world.create_poly (vert, 4);
}

*/

#endif // !INC_GFX_3D