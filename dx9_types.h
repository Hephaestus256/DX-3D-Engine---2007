#ifndef INC_DX9_TYPES
#define INC_DX9_TYPES

//	 define types for DirectX 9.0
typedef point_3d vert_3d; // defines the type of vertex
typedef point_2d tex_coord; // defines the type of texture coordinate
typedef DWORD color_3d; // defines the type of color
typedef DWORD illum_3d; // defines the type of illumination
typedef D3DXVECTOR3 rend_p3d, dx9_p3d; // defines the type of 3d vertex for renderer
typedef D3DXVECTOR2 rend_p2d, dx9_p2d; // defines the type of 2d vertex for renderer
typedef rend_p2d rend_tc, dx9_tc; // defines the type of texture coordinates for renderer
typedef DWORD rend_ill, dx9_ill; // defines the type of vertex illumination for renderer
typedef DWORD rend_color, dx9_color; // defines the type of vertex color for renderer
typedef IDirect3DDevice9 rend_class, dx9_dev; // defines the renderer class
typedef IDirect3DTexture9 rend_tex, dx9_tex; // defines the texture class for renderer
typedef IDirect3DTexture9 *rend_tex_ptr, *dx9_tex_ptr; // defines pointer to texture class for renderer
typedef IDirect3DVertexBuffer9 rend_geo_bank, dx9_vb; // defines class for allocating of renderer resources for geometry
typedef IDirect3DVertexBuffer9* rend_geo_bank_ptr, dx9_vb_ptr; // defines pointer to above class
typedef LPDIRECT3DSWAPCHAIN9 rend_target, dx9_sc; // defines the render target

#endif // !INC_DX9_TYPES