// Renderer-independent base for 3D graphics rendering
// Supports the renderer functions

#ifndef INC_REND_3D_BASE
#define INC_REND_3D_BASE

#include "my_data.h"
#include "math_3d.h"

#define PRIM_GROUP_MODE_SCENE_STATIC 0
#define PRIM_GROUP_MODE_SCENE_DYNAMIC 1
#define PRIM_GROUP_MODE_LINE 2


// class for creating Alpha, Red, Green, Blue color
class argb_valu {
public:
	float a, r, g, b;

	argb_valu () {}

	argb_valu (float ap, float rp, float gp, float bp)
	{
		a = ap;
		r = rp;
		g = gp;
		b = bp;
	}

	DWORD cv_dword ()
	{
		return 
			(int(clip_max_valu (a * 255.0f, 255)) << 24) +
			(int(clip_max_valu (r * 255.0f, 255)) << 16) +
			(int(clip_max_valu (g * 255.0f, 255)) << 8) +
			(int(clip_max_valu (b * 255.0f, 255)) << 0);
	}
};


// holds data for texture layers
typedef tex_coord tex_layer;


// holds geometry vertex for 3d world
class geo_vert { // geometry layer
public:
	vert_3d pos; // spatial 3d position
	tex_layer tex; // base texture coordinates
	illum_3d illum; // illumination

	geo_vert () {}

	geo_vert (vert_3d p, tex_coord t, illum_3d i) 
	{
		pos = p;
		tex = t;
		illum = i;
	}
};


// holds data for vertices without texture data
class color_vert {      
public:
	vert_3d pos; // vertex
	illum_3d color; // illumination

	color_vert () {}

	color_vert (vert_3d p, illum_3d c)
	{
		pos = p;
		color = c;
	}
};


// geometric primitive
class geo_prim {
public:
	geo_vert vert[3];

	geo_prim () {}

	geo_prim (geo_vert v1, geo_vert v2, geo_vert v3)
	{
		vert[0] = v1;
		vert[1] = v2;
		vert[2] = v3;
	}
};


// colored line segment
class color_seg {
public:
	color_vert p[2];

	color_seg () {}

	color_seg (color_vert p1, color_vert p2)
	{
		p[0] = p1;
		p[1] = p2;
	}
};


// perspective to render scene from
class camera_3d { 
	friend class renderer_3d;
	
	rend_target pm_rt; // render target
	
	camera_3d (
		renderer_3d* rend, 
		ray_3d view, double axy, 
		double fov, double aspect, double min_z, double max_z)//, rend_target rt)
	{
		m_pos = view.orig;
		m_axz = view.angle.x;
		m_ayz = view.angle.y;
		m_axy = axy;
		m_fov = fov;
		m_aspect = aspect;
		m_min_z = min_z;
		m_max_z = max_z;
		//pm_rt = rend->create_draw_surface (
	}

public:
	point_3dd m_pos; // view position 
	double m_axz, m_ayz, m_axy; // view angles
	int x_res, y_res; // resolution
	double m_fov; // field of view
	double m_aspect; // aspect ratio
	double m_min_z, m_max_z; // z-range

	camera_3d () {}
};


#endif // !INC_REND_3D_BASE