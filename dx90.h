// DirectX 9.0 wrapper

// rend.init_3d (camera, window, full_screen);

#ifndef DX90_INC
#define DX90_INC

#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "math_3d.h"
#include "os_3d.h"
#include "dx9_types.h"
#include "rend_3d_base.h"

char _debug_text[100][50];	
line_3d _debug_line[100];
LPDIRECT3DVERTEXBUFFER9 _line_buff;
int _debug_line_ct = 0;

#define MAX_STREAMS 32
#define CAM5ERA_3D_DEFAULT_PARAMS point_3d (0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, PI_4, 1440.0f / 900.0f, 0.5f, F_INF, 0, 0, 800 - 1, 600 - 1, NULL, NULL, NULL

dx9_tex_ptr _textr;

// convert 3d points to and from renderer vertexes
point_3d cv_point_3d (rend_p3d p)
{
	return point_3d (p.x, p.y, p.z);
}


point_3dd cv_point_3dd (rend_p3d p)
{
	return point_3dd (p.x, p.y, p.z);
}


point_2d cv_point_2d (rend_p2d p)
{
	return point_2d (p.x, p.y);
}


rend_p2d cv_rend_p2d (coord p)
{
	return rend_p2d (p.x, p.y);
}


rend_p2d cv_rend_p2d (coordd p)
{
	return rend_p2d (float(p.x), float(p.y));
}


rend_p3d cv_rend_p3d (point_3d p)
{
	return rend_p3d (p.x, p.y, p.z);
}


rend_p3d cv_rend_p3d (point_3dd p)
{
	return rend_p3d (float(p.x), float(p.y), float(p.z));
}


point_2d calc_top_left (point_2d center, dx9_tex* tex, float scale)
{
	D3DSURFACE_DESC desc;
	tex->GetLevelDesc (0, &desc);
	
	return center / scale - point_2d (desc.Width, desc.Height) * 0.5;
}



/*
class vert_buff {
public:
	LPDIRECT3DVERTEXBUFFER9 vb;
	int prim_ct;

	vert_buff (rend_class rend, D3DPOOL mode, int ct) 
	{
			if (FAILED (
				dev->CreateVertexBuffer (ct * sizeof(geo_prim), 0, 0, mode, &vb, NULL))
			) error ("Vertex Buffer");
	}
};
*/

/*
class prim_group {
	LPDIRECT3DDEVICE9 dev_3d;

	struct layer_type {
		LPDIRECT3DVERTEXBUFFER9 vb;
		int prim_ct;
	};
public:
	linked_list* layer;
	
	prim_group () {}

	prim_group (
		LPDIRECT3DDEVICE9 dev_3d, // direct 3d device
		int mode, // primitive format
		int* prim_ct, // list of primitive counts for each layer
		int layer_ct) // number of layers                
	{		
		// mode:
		//		PRIM_GROUP_MODE_SCENE_STATIC - fixed size scene
		//		PRIM_GROUP_MODE_SCENE_DYNAMIC - variable size scene
		//		PRIM_GROUP_MODE_LINE - line segments

		if (mode == PRIM_GROUP_MODE_SCENE_STATIC) {
		}
		else if (mode == PRIM_GROUP_MODE_SCENE_DYNAMIC) {
		}
	}

	// creates a quadrilateral in world
	// vert component of quad extends from 'start' in (+)y direction to 'start' + 'h'
	// x-z component of quad extends 'w' units at angle 'angle'
	// 'angle' of 0.0 PI makes quad face toward (-)z direction
	// 
	// 1----2  4
	// |    /  /|
	// |  /  /  |
	// |/  /    |
	// 0  3----5
	bool create_geo_prims (geo_prim* prim, int ct)
	{
		geo_prim* vb = NULL;
		//geo_vert* vb = NULL;


		if (FAILED (layer[0]->Lock (0, 0, (void**)&vb, 0))) {
			error ("lock");
			return false;
		}

		for (int i = 0; i < ct; i++)
			vb[i] = prim[i];
		//for (int i = 0; i < 3; i++)
		//	vb[i] = prim->vert[i];

		if (FAILED (layer[0]->Unlock ())) {
			error ("unlock");
			return false;
		}

		return true;
	}

	bool create_test_line (line_3d l, argb_valu c)
	{
		int comp = l.vect ().major_comp ();

		if (comp == POINT_3D_X)
			return create_test_line (l.func_x (-1000.0).cv_point_3d (), l.func_x (1000.0).cv_point_3d (), c);
		else if (comp == POINT_3D_Y)
			return create_test_line (l.func_y (-1000.0).cv_point_3d (), l.func_y (1000.0).cv_point_3d (), c);
		else
			return create_test_line (l.func_z (-1000.0).cv_point_3d (), l.func_z (1000.0).cv_point_3d (), c);
	}
};
*/

// rend.render (world, cam, res)

/*
// container for multiple cameras
class camera_pool {
public:
	camera_3d* start;
	camera_3d* last;

	camera_pool ()
	{
		start = last = NULL;
	}

	camera_3d* add (LPDIRECT3DSWAPCHAIN9 sc, window* win, prim_group* world)
	{
		camera_3d* cam = add ( CAMERA_3D_DEFAULT_PARAMS);
		
		cam->sc = sc;
		cam->win = win;
		cam->world = world;

		return cam;
	}

	camera_3d* add
		(point_3d pos, double xz, double yz, double xy, // position and angle
		double fov, double aspect, // scale factors (field of view, aspect ratio)
		double min_z, double max_z, // z-range
		int x1, int y1, int x2, int y2, // view rectangle
		LPDIRECT3DSWAPCHAIN9 sc, // DX swap chain
		window* win,
		prim_group* world) // target window
	{
		camera_3d* nu = new camera_3d (
			pos, xz, yz, xy, 
			fov, aspect, min_z, max_z,
			x1, y1, x2, y2,
			sc, win, world);
		
		if (last == NULL)
			start = nu;
		else 
			last->next = nu;

		last = nu;
		return nu;
	}
};
*/

/*
class draw_surface {
	rend_target m_rt;		
	bool m_wait_vr;

public:

	draw_surface ()
	{
		m_rt = NULL;
	}

	draw_surface (renderer_3d* rend, int width, int height, int bits_per_pxl, bool wait_vr)
	{
		create (rend, width, height, bits_per_pxl, wait_vr);
	}

	~draw_surface ()
	{
		m_rt = NULL;
	}

	rend_target create (renderer_3d* rend, int width, int height, int bits_per_pxl, bool wait_vr)
	{
		D3DPRESENT_PARAMETERS pp;
		D3DFORMAT format;

		// init member vars
		m_wait_vr = wait_vr;

		// init all parameters to zero
		ZeroMemory (&pp, sizeof (pp));	

		// set back buffer format based on bits-per-pixel
		if (bits_per_pxl == 16)
			format = D3DFMT_R5G6B5;
		else if (bits_per_pxl == 24)
			format = D3DFMT_R8G8B8;
		else // default to 32 bits per pixel
			format = D3DFMT_A8R8G8B8;
		
		// set presentation parameters
		pp.hDeviceWindow = NULL;
		pp.Windowed = true;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;		
		pp.BackBufferWidth = width;
		pp.BackBufferHeight = height;
		pp.BackBufferFormat = format;
		pp.EnableAutoDepthStencil = true;
		pp.AutoDepthStencilFormat = D3DFMT_D16;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		//m_d3d_dev->GetSwapChain (0, &sc);
		//rend->CreateAdditionalSwapChain (&pp, &m_rt);
	}
};
*/

// DX 3d rendering class
typedef class renderer_3d {	
public:
	rend_target sc1, sc2;
	// initilization data
	LPDIRECT3D9 m_d3d; // direct 3d function class
	LPDIRECT3DDEVICE9 m_d3d_dev; // direct 3d rendering device
	D3DPRESENT_PARAMETERS m_pars; // basic current 3d rendering parameters
	HINSTANCE m_instance; //

	// rendering data
	LPDIRECT3DVERTEXDECLARATION9 m_vertex_decl; // vertex declaration
	float m_phys_aspect; // physical aspect ratio of screen 
	float m_res_aspect; // screen resolution aspect ratio

	LPD3DXFONT m_default_font;

	// constructor functions
	renderer_3d ()
	{
		init_dat ();
	}

	// destructor function
	~renderer_3d ()
	{
	    if (m_d3d_dev != NULL)
			m_d3d_dev->Release(); // release 3d device

		if (m_d3d != NULL)
			m_d3d->Release(); // release DX class

		m_d3d = NULL;		
		m_d3d_dev = NULL;
	}

	rend_target create_draw_surface (int width, int height, int bits_per_pxl, bool wait_vert_retrace)
	{
		D3DPRESENT_PARAMETERS pp;
		D3DFORMAT format;

		// init all parameters to zero
		ZeroMemory (&pp, sizeof (pp));	

		// set back buffer format based on bits-per-pixel
		if (bits_per_pxl == 16)
			format = D3DFMT_R5G6B5;
		else if (bits_per_pxl == 24)
			format = D3DFMT_R8G8B8;
		else // default to 32 bits per pixel
			format = D3DFMT_A8R8G8B8;
		
		// set presentation parameters
		pp.hDeviceWindow = NULL;
		pp.Windowed = true;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;		
		pp.BackBufferWidth = width;
		pp.BackBufferHeight = height;
		pp.BackBufferFormat = format;
		pp.EnableAutoDepthStencil = true;
		pp.AutoDepthStencilFormat = D3DFMT_D16;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		//m_d3d_dev->GetSwapChain (0, &sc);
		rend_target rt;	
		m_d3d_dev->CreateAdditionalSwapChain (&pp, &rt);
		return rt;		
	}

	void calc_res_aspect ()
	{
		m_res_aspect = float(m_pars.BackBufferWidth) / float(m_pars.BackBufferHeight);
	}

	bool set_gfx_prefs ()
	{
		m_d3d_dev->SetRenderState (D3DRS_DITHERENABLE, TRUE);			
		m_d3d_dev->SetRenderState (D3DRS_ANTIALIASEDLINEENABLE, FALSE);		
		m_d3d_dev->SetSamplerState (0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		m_d3d_dev->SetSamplerState (0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		m_d3d_dev->SetSamplerState (0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

		return true;
	}

	bool init_device (HINSTANCE instance, window* win, bool full_screen) 
	{
		// program instance
		m_instance = instance;

		// create DX function class
		m_d3d = Direct3DCreate9 (D3D_SDK_VERSION);

		// get current windows display parameters
		D3DDISPLAYMODE dm;
		m_d3d->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &dm);

		// set parameters
		ZeroMemory (&m_pars, sizeof(m_pars));
	
		m_pars.hDeviceWindow = win->wh;
		m_pars.Windowed = !full_screen;
		m_pars.SwapEffect = D3DSWAPEFFECT_DISCARD;		
		m_pars.BackBufferWidth = 256;
		m_pars.BackBufferHeight = 256;//dm.Height;
		m_pars.BackBufferFormat = dm.Format;
		m_pars.EnableAutoDepthStencil = true;
		m_pars.AutoDepthStencilFormat = D3DFMT_D16;
		m_pars.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		calc_res_aspect ();

		// creates the DX 3d device
		HRESULT res = m_d3d->CreateDevice (
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			win->wh, 
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_pars, &m_d3d_dev);

		// test if device creation was successful
		switch (res) {
			case D3DERR_DEVICELOST:
				error ("3d device lost");
				return false;
			case D3DERR_INVALIDCALL:
				error ("Invalid call to 3d device");
				return false;
			case D3DERR_NOTAVAILABLE:
				error ("3d device not available");
				return false;
			case D3DERR_OUTOFVIDEOMEMORY:
				error ("Out of video memory");
				return false;
		}

		m_d3d_dev->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE);//CCW); // NONE
		m_d3d_dev->SetRenderState (D3DRS_LIGHTING, FALSE);
		m_d3d_dev->SetRenderState (D3DRS_ZENABLE, TRUE);
		m_d3d_dev->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	
		set_gfx_prefs ();

		m_d3d_dev->ShowCursor (FALSE);

		// a single texture layer
		D3DVERTEXELEMENT9 tex_layer = 
			{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0};
		D3DVERTEXELEMENT9 end_decl = D3DDECL_END();

		// set position and modulation (light) for first layer (geometric layer)
		D3DVERTEXELEMENT9 vert_decl[MAX_STREAMS + 30] =
		{
		  //stream  
		  //   offset     type               method                 usage        usage index       
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
			{0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			D3DDECL_END()
		};

		m_d3d_dev->GetSwapChain (0, &sc1);

		D3DPRESENT_PARAMETERS pp;

		ZeroMemory (&pp, sizeof(pp));

		pp.hDeviceWindow = NULL;//win->wh;
		pp.Windowed = true;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;		
		pp.BackBufferWidth = 512;
		pp.BackBufferHeight =512;//dm.Height;
		pp.BackBufferFormat = dm.Format;
		pp.EnableAutoDepthStencil = true;
		pp.AutoDepthStencilFormat = D3DFMT_D16;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
//		m_d3d_dev->CreateAdditionalSwapChain (&pp, &sc2);

		// declare streams for additional texture layers
		// for (int i = 1; i < MAX_STREAMS; i++) {
		//    vert_decl[i + 2] = tex_layer; 
		//    vert_decl[i + 2].Stream = i;
		// }
		//vert_decl[MAX_STREAMS + 2] = end_decl; // declare end of stream declaration
		
		// register declaration with DX
		//m_d3d_dev->CreateVertexDeclaration (vert_decl, &vertex_decl);

		return true;
	}

	void init_dat ()
	{
		m_d3d = NULL;		
		m_d3d_dev = NULL;
		m_instance = 0;
		m_phys_aspect = 1.6f;

		for (int y = 0; y < 50; y++)
			_debug_text[y][0] = '\0';
	}

	bool set_res (int x, int y)
	{
		m_pars.BackBufferWidth = x;
		m_pars.BackBufferHeight = y;
		calc_res_aspect ();

		if (FAILED (m_d3d_dev->Reset(&m_pars))) {
			error ("Could not set resolution");
			return false;
		}
		else
			return true;
	}

	bool set_full_screen ()
	{
		m_pars.Windowed = false;
		m_d3d_dev->Reset(&m_pars);

		return true;
	}

	bool set_windowed ()
	{
		m_pars.Windowed = true;
		m_d3d_dev->Reset(&m_pars);

		return true;
	}

	void clear_view ()
	{
		m_d3d_dev->Clear (0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f), 1.0f, 0);	
	}

	void create_font ()
	{
		D3DXCreateFont (
			m_d3d_dev, 25, 0, FW_BOLD, 0, FALSE, 
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_default_font
		);
	}

	void put_text (int col, int row, char* text, DWORD color)
	{
		RECT r;
		SetRect (&r, col * 20, row * 25, 0, 0);
		m_default_font->DrawText (NULL, text, -1, &r, DT_NOCLIP, color);
	}

	bool draw_sprite (coord tl, dx9_tex_ptr tex, float scale, float aspect, argb_valu shade)
	{
		LPD3DXSPRITE sprite;
		D3DXCreateSprite (m_d3d_dev, &sprite);
		D3DXMATRIX mat;
		float cor = 1.0;//(phys_aspect / res_aspect);

		mat._11 = scale; 
		mat._12 = 0.0f;
		mat._13 = 0.0f;
		mat._14 = 0.0f;

		mat._21 = 0.0f;
		mat._22 = scale * cor;
		mat._23 = 0.0f;
		mat._24 = 0.0f;

		mat._31 = 0.0f;
		mat._32 = 0.0f;
		mat._33 = scale;
		mat._34 = 0.0f;

		mat._41 = 0.0f;
		mat._42 = 0.0f;
		mat._43 = 0.0f;
		mat._44 = 1.0;

		sprite->SetTransform (&mat);
		dx9_p3d p = cv_rend_p3d (point_3d (tl.x, tl.y, 0.0));

		sprite->Begin (0);

//		d3d_dev->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
//		d3d_dev->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//		d3d_dev->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		sprite->Draw (tex, NULL, NULL, &p, shade.cv_dword ());	
		sprite->End ();

		return true;
	}

	rend_geo_bank_ptr create_geo_bank (int prim_ct, int prim_size, bool stat)
	{
		rend_geo_bank_ptr vb;    

		if (FAILED (m_d3d_dev->CreateVertexBuffer (prim_ct * prim_size, 0, 0, D3DPOOL_DEFAULT, &vb, NULL)))
			error ("Create Vertex Buffer");

		return vb;
	}

	bool modify_geo_bank (rend_geo_bank_ptr bank, void* data, int start, int len, int elem_size)
	{
		void* vb = NULL;
		
		if (FAILED (bank->Lock (0, 0, &vb, 0))) {
			error ("Lock Vertex Buffer");
			return false;
		}

		memcpy (offset (vb, start * elem_size), data, len * elem_size);

		if (FAILED (bank->Unlock ())) {
			error ("Unlock Vertex Buffer");
			return false;
		}

		return true;
	}

	void show_debug_text ()
	{
		for (int y = 0; y < 50; y++)
			put_text (0, y, &_debug_text[y][0], 0xffffffff);
	}

	/*
	bool show_seg (point_3d s1, point_3d s2, argb_valu c)
	{
		geo_vert* vb = NULL;
		
		if (FAILED (_line_buff->Lock (0, 0, (void**)&vb, 0))) {
			error ("lock");
			return false;
		}

		vb[0].vert = s1;
		vb[0].illum = c.cv_dword ();
		vb[1].vert = s2;
		vb[1].illum = c.cv_dword ();

		if (FAILED (layer[0]->Unlock ())) {
			error ("unlock");
			return false;
		}

		return true;
	}
	*/			

//		d3d_dev->SetStreamSource (0, _line_buff, 0, sizeof(geo_vert));		
//		d3d_dev->DrawPrimitive (D3DPT_LINELIST, 0, 1);
//	}
/*
	bool render ()
	{
		bool ret = true;

		for (camera_3d* cam = cam_pool.start; cam != NULL; cam = cam->next)
			if (!render (cam))
				ret = false;

		return ret;
	}
*/
	/*
	bool render (rend_geo_bank_ptr bank, os_resource res)
	{
		D3DXMATRIX proj, view, world, rot, trans;

		D3DXMatrixPerspectiveFovLH (&proj, float(PI_4), 1.5, 1.25f, 200.0f);
		d3d_dev->SetTransform (D3DTS_PROJECTION, &proj);

		LPDIRECT3DSURFACE9 back_buff = NULL;
		cam->sc->GetBackBuffer (0, D3DBACKBUFFER_TYPE_MONO, &back_buff);
		d3d_dev->SetRenderTarget (0, back_buff);

		clear_view (); 

		d3d_dev->BeginScene();

//			float s = 1.0;
//			point_2d p = cam->map_vertex (point_3d (10.0, 10.0, 125.0 + 18.0));

			//p = calc_top_left (p, _tex, s);
//			draw_sprite (p, _tex, s, 1.0, argb_valu (1.0, 1.0, 1.0, 1.0));

			show_debug_text ();

			d3d_dev->SetTexture (0, _tex);

			D3DXMatrixIdentity (&view);
			d3d_dev->SetTransform (D3DTS_VIEW, &view);

			D3DXMatrixRotationYawPitchRoll (&rot, float(0.0), float(0.0), float(0.0));
			D3DXMatrixTranslation (&trans, 0.0, 0.0, 0.0);
			world = rot * trans;
			d3d_dev->SetTransform (D3DTS_WORLD, &world);

					d3d_dev->SetVertexDeclaration (vertex_decl);

					d3d_dev->SetStreamSource (0, bank, 0, sizeof(geo_vert));
			
//			d3d_dev->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
//			d3d_dev->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
//			d3d_dev->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

//			d3d_dev->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
//			d3d_dev->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//			d3d_dev->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

//					d3d_dev->DrawPrimitive (D3DPT_TRIANGLELIST, 0, 2);
//			show_line (line_3d ());

		d3d_dev->EndScene ();

		cam->sc->Present (NULL, NULL, wh, NULL, 0);

		back_buff->Release();
		return true;
	}
	*/
	bool render (window* win, camera_3d* cam)
	{

//		D3DXMATRIX proj, view, world, rot, trans;

//		D3DXMatrixPerspectiveFovLH (&proj, float(PI_4), float(cam->aspect), 1.25f, 200.0f);
//		d3d_dev->SetTransform (D3DTS_PROJECTION, &proj);

//		LPDIRECT3DSURFACE9 back_buff = NULL;
//		cam->sc->GetBackBuffer (0, D3DBACKBUFFER_TYPE_MONO, &back_buff);
//		d3d_dev->SetRenderTarget (0, back_buff);

//		clear_view (); 

//		LPDIRECT3DSURFACE9 bb = NULL;
//		sc1->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &bb);
//		m_d3d_dev->SetRenderTarget (0, bb);
		
//		m_d3d_dev->BeginScene();
//		draw_sprite (coord (0.0, 0.0), _textr, 1.0, 1.0, argb_valu (1.0, 1.0, 0.0, 0.0));
//			float s = 1.0;
//			point_2d p = cam->map_vertex (point_3d (10.0, 10.0, 125.0 + 18.0));

			//p = calc_top_left (p, _tex, s);
///			draw_sprite (p, _tex, s, 1.0, argb_valu (1.0, 1.0, 1.0, 1.0));

//			show_debug_text ();

//			d3d_dev->SetTexture (0, _tex);

//			D3DXMatrixIdentity (&view);
//			d3d_dev->SetTransform (D3DTS_VIEW, &view);

//			D3DXMatrixRotationYawPitchRoll (&rot, float(-cam->axz), float(-cam->ayz), float(-cam->axy));
//			D3DXMatrixTranslation (&trans, -cam->pos.x, -cam->pos.y, -cam->pos.z);
//			world = rot * trans;
//			d3d_dev->SetTransform (D3DTS_WORLD, &world);

//					d3d_dev->SetVertexDeclaration (vertex_decl);

							//d3d_dev->SetStreamSource (0, cam->world->layer[0], 0, sizeof(geo_vert));
			
//			d3d_dev->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
//			d3d_dev->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
//			d3d_dev->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

//			d3d_dev->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
//			d3d_dev->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//			d3d_dev->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

//					d3d_dev->DrawPrimitive (D3DPT_TRIANGLELIST, 0, 2);
//			show_line (line_3d ());

//		m_d3d_dev->EndScene ();

//		sc1->Present (NULL, NULL, win->wh, NULL, 0);

//		back_buff->Release();
		return true;
	}
} d3d_device;

//******************************************************************************************

void tprint (char* str, int row)
{
	sprintf (&_debug_text[row][0], "%s", str);
}

void tprint (char* str, double n, int row)
{
	sprintf (&_debug_text[row][0], "%s %5.4f", str, n);
}


//point_2d calc_tex_coord (point_3d
#endif // !DX90_INC