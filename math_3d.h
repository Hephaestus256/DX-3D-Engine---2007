// ================================================================================
// 3D Math definitions
// Nathan G. Smith
// ================================================================================
//
// ---major plane axis definitions:---
//
//     ^                          ^                        
//     |      ^                   |       ^
//     y     /			         z     /
//     |   z                      |    x
//     |  /  (xy plane)      |  /   (yz plane)
//     |/                         |/                       
//     0----x------>          0-----y----->            
//                                                     
//     ^                          ^  
//     |       ^                  |  
//     x     /              90 degrees  <-.
//     |    y                     |                  \ increasing angle = ccw
//     |  /   (zx plane )    |                   |
//     |/                         |                     
//     0----z------>           +--0-degrees-->     
//
//     ^                      
//     |       ^     generic plane axis:
//     D     /      I = independent axis
//     |    /        D = dependent axis             
//     |   P         P = perpendicular axis
//     |/      
//     0----I------>      
//
//
// ---3d angle definitions:---
// general case:
//     x: zx angle
//     y: yz angle
// xy angle case:
//	   x: ANGLE_3D_XY_CASE_X_VALU
//     y: xy angle
//
//
// ---3d line definitions:---
// 
// ---3d plane definitions:---
//
// ---3d ray definitions:---
//
// ---relevant plane definitions:---
// if (dx != 0)
//	  if (dy != 0)
//	     relevant plane = xy
//    else
//       if (dz != 0)
//          relevant plane = zx
//       else
//          undefined
// else
//    if (dy != 0)
//       if (dz != 0)
//          relevant plane = yz
//       else
//          undefined
//    else
//       undefined

#ifndef INC_MATH_3D
#define INC_MATH_3D

#include "math_1d.h"
#include "math_2d.h"

// ************************ 3d constant definitions **********************************

// common float 3d vector definitions
#define ORIG_3D point_3d (0.0f, 0.0f, 0.0f)
#define VECT_3D_UP vect_3d (0.0f, 1.0f, 0.0f)
#define VECT_3D_DOWN vect_3d (0.0f, -1.0f, 0.0)
#define VECT_3D_LEFT vect_3d (-1.0f, 0.0f, 0.0f)
#define VECT_3D_RIGHT vect_3d (1.0f, 0.0f, 0.0f)
#define VECT_3D_FORWARD vect_3d (0.0f, 0.0f, 1.0f)
#define VECT_3D_BACKWARD vect_3d (0.0f, 0.0f, -1.0f)
#define VECT_3D_ZERO vect_3d (0.0f, 0.0f, 0.0f)
#define VECT_3D_UNDEF vect_3d (-INF, INF, -INF)

// common double 3d vector definitions 
#define ORIG_3DD point_3dd (0.0, 0.0, 0.0)
#define VECT_3DD_UP vect_3dd (0.0, 1.0, 0.0)
#define VECT_3DD_DOWN vect_3dd (0.0, -1.0, 0.0)
#define VECT_3DD_LEFT vect_3dd (-1.0, 0.0, 0.0)
#define VECT_3DD_RIGHT vect_3dd (1.0, 0.0, 0.0)
#define VECT_3DD_FORWARD vect_3dd (0.0, 0.0, 1.0)
#define VECT_3DD_BACKWARD vect_3dd (0.0, 0.0, -1.0)
#define VECT_3DD_ZERO vect_3d (0.0, 0.0, 0.0)
#define VECT_3DD_UNDEF vect_3dd (Q_NAN, -INF, S_NAN)

// common 3d angle definitions
#define ANGLE_3D_POS_X angle_3d (-PI_2, Q_NAN)
#define ANGLE_3D_NEG_X angle_3d (PI_2, Q_NAN)
#define ANGLE_3D_POS_Y angle_3d (Q_NAN, 0.0)
#define ANGLE_3D_NEG_Y angle_3d (Q_NAN, PI)
#define ANGLE_3D_POS_Z angle_3d (0.0, PI_2)
#define ANGLE_3D_NEG_Z angle_3d (PI, -PI_2)
#define ANGLE_3D_UNDEF angle_3d (Q_NAN, Q_NAN)

// common 3d line definitions
#define LINE_3D_X_AXIS line_3d (ORIG_3DD, VECT_3DD_LEFT)
#define LINE_3D_Y_AXIS line_3d (ORIG_3D, VECT_3DD_UP)
#define LINE_3D_Z_AXIS line_3d (ORIG_3D, VECT_3DD_FORWARD)

// common 3d slope definitions
#define SLOPE_3D_X_AXIS xy_pair (LIN_M_VERT, 0.0)
#define SLOPE_3D_Y_AXIS m_3d (LIN_M_VERT, LIN_M_VERT)
#define SLOPE_3D_Z_AXIS m_3d (0.0, 0.0)
#define SLOPE_3D_UNDEF m_3d (LIN_M_NAN, LIN_M_NAN)

// to reference coordinates by index
#define POINT_3D_X 0 
#define POINT_3D_Y 1
#define POINT_3D_Z 2
#define POINT_3D_AXIS_UNDEF -1

// to reference 3d axis-planes by index
#define AXIS_PLANE_XY 0
#define AXIS_PLANE_YZ 1
#define AXIS_PLANE_ZX 2
#define AXIS_PLANE_YX 3
#define AXIS_PLANE_ZY 4
#define AXIS_PLANE_XZ 5
#define AXIS_PLANE_UNDEF 6

// define the axes of axis planes by index
disc_2d AXIS_PLANE[] = {
	disc_2d (POINT_3D_X, POINT_3D_Y), // xy plane
	disc_2d (POINT_3D_Y, POINT_3D_Z), // yz plane
	disc_2d (POINT_3D_Z, POINT_3D_X), // zx plane
	disc_2d (POINT_3D_Y, POINT_3D_X), // yx plane
	disc_2d (POINT_3D_Z, POINT_3D_Y), // zy plane
	disc_2d (POINT_3D_X, POINT_3D_Z), // xz plane
	disc_2d (POINT_3D_AXIS_UNDEF, POINT_3D_AXIS_UNDEF) // axis plane undefined
};

// 3d slope definition
// has same properties as slopes of a 3d line
typedef xy_pair m_3d;

/*
// values for line_3d cases
#define LINE_3D_GEN 0
#define LINE_3D_XY 1
#define LINE_3D_UNDEF 2

// values for plane cases
#define PLANE_3D_GEN 0
#define PLANE_3D_XY 1
#define PLANE_3D_Y 2
#define PLANE_3D_UNDEF 3
*/

// ********************** 3d point definitions ***************************

// float 3d point definition (vect_3d)
typedef class point_3d { 
public:
	union {
		struct {
			float x, y, z;
		};

		float member[3];
	};

	inline point_3d () {}

	inline point_3d (double xp, double yp, double zp)
	{
		x = float(xp);
		y = float(yp);
		z = float(zp);
	}

	// constructor by zx angle
	inline point_3d (double azx)
	{
		x = float(sin (azx));
		y = 0.0f;
		z = float(cos (azx));
	}
		
	inline point_3d (double ax, double ay)
	{
		double sin_ay = sin (ay);

		x = float(sin (ax) * sin_ay);
		y = float(cos (ay));
		z = float(cos (ax) * sin_ay);		
	}
	
	point_3d (coord xy, double z)
	{
		x = xy.x;
		y = -1234.0f;//xy.y;
		z = 0.0f;//float(z);
	}

	point_3d (coordd xy, double z)
	{
		x = float(xy.x);
		y = 456.0f;//float(xy.y);
		z = 0.0f;//float(z);
	}

	inline point_3d operator + (point_3d p)
	{
		return point_3d (p.x + x, p.y + y, p.z + z);
	}

	inline point_3d operator += (point_3d p)
	{
		return point_3d (x += p.x, y += p.y, z += p.z);
	}

	inline point_3d operator - (point_3d p)
	{
		return point_3d (p.x - x, p.y - y, p.z - z);
	}

	inline point_3d operator -= (point_3d p)
	{
		return point_3d (x -= p.x, y -= p.y, z -= p.z);
	}

	inline point_3d operator - ()
	{
		return point_3d (-x, -y, -z);
	}

	inline point_3d operator * (double n)
	{
		return point_3d (double(x) * n, double(y) * n, double(z) * n);
	}

	inline point_3d operator / (double n)
	{
		return point_3d (double(x) / n, double(y) / n, double(z) / n);
	}

	// returns abstract member pair (ind, dep) of axis-plane pl
	inline coord pair (int pl)
	{
		return coord (
			member[AXIS_PLANE[pl].ind], member[AXIS_PLANE[pl].dep]
		);
	}

	inline coord xy ()
	{
		return coord (x, y);
	}

	inline coord yz ()
	{
		return coord (y, z);
	}

	inline coord zx ()
	{
		return coord (z, x);
	}

	inline coord yx ()
	{
		return coord (y, x);
	}

	inline coord zy ()
	{
		return coord (z, y);
	}

	inline coord xz ()
	{
		return coord (x, z);
	}

	inline bool operator == (point_3d p)
	{
		return approx_equal (x, p.x) && approx_equal (y, p.y) && approx_equal (z, p.z);
	}

	inline bool operator != (point_3d p)
	{
		return !approx_equal (x, p.x) || !approx_equal (y, p.y) || !approx_equal (z, p.z);
	}

	inline bool valid ()
	{
		return real (x) && real (y) && real (z);
	}

	inline bool invalid ()
	{
		return !real (x) || !real (y) || !real (z);
	}

	inline bool on_seg_inc (point_3d p1, point_3d p2)
	{
		return (
			between_inc (x, p1.x, p2.x) &&
			between_inc (y, p1.y, p2.y) &&
			between_inc (z, p1.z, p2.z)
		);
	}

	inline bool on_seg_exc (point_3d p1, point_3d p2)
	{
		return (
			between_exc (x, p1.x, p2.x) &&
			between_exc (y, p1.y, p2.y) &&
			between_exc (z, p1.z, p2.z)
		);
	}

	inline int major_comp ()
	{
		if (fabs (x) > fabs (y))
			if (fabs (z) > fabs (x))
				return POINT_3D_Z;
			else 
				return POINT_3D_X;
		else
			if (fabs (z) > fabs (y))
				return POINT_3D_Z;
			else
				return POINT_3D_Y;
	}

	inline xy_pair slope_3d ()
	{
		if (approx_zero (z))
			if (approx_zero (x))
				if (approx_zero (y))
					return SLOPE_3D_UNDEF;
				else
					return SLOPE_3D_Y_AXIS;
			else
				return xy_pair (LIN_M_VERT, y / x);
		else
			return xy_pair (x / z, y / z);
	}

	inline float dx (point_3d p)
	{
		return p.x - x;
	}

	inline float dy (point_3d p)
	{
		return p.y - y;
	}

	inline float dz (point_3d p)
	{
		return p.z - z;
	}

	inline double dist_3d (point_3d p)
	{
		return sqrt (sqrd (dx (p)) + sqrd (dy (p)) + sqrd (dz (p)));
	}

	char* print ()
	{
		static char buff[128];
		sprintf_s (buff, "(%6.4f, %6.4f, %6.4f)", x, y, z);
		return buff;
	}

} vect_3d;


// 64-bit (double) version of 3d point (vect_3dd)
typedef class point_3dd { 
public:
	union {
		struct {
			double x, y, z;
		};

		double member[3];
	};

	// void constructor
	inline point_3dd () {}

	// constructor by 3 doubles
	inline point_3dd (double xp, double yp, double zp)
	{
		x = xp;
		y = yp;
		z = zp;
	}

	// constructor from angle in zx plane
	inline point_3dd (double azx)
	{
		vect_2dd zx (azx);

		x = zx.dep;
		y = 0.0;
		z = zx.ind;
	}
		
	// constructor from angle in zx plane and yz plane
	inline point_3dd (double azx, double ayz)
	{
		vect_2dd yz (ayz);
		vect_2dd zx = vect_2dd (azx) * yz.ind;
		
		x = zx.dep;
		y = yz.dep;
		z = zx.ind;
	}
	
	// constructor by 2d coord xy and z coordinate
	inline point_3dd (coord xy, double zc)
	{
		x = xy.x;
		y = xy.y;
		z = zc;
	}

	// constructor by 2d coordd xy and z coordinate
	inline point_3dd (coordd xy, double zc)
	{
		x = xy.x;
		y = xy.y;
		z = zc;
	}

	// returns sum of corresponding members
	inline point_3dd operator + (point_3dd p)
	{
		return point_3dd (p.x + x, p.y + y, p.z + z);
	}

	// increments point_3dd by corresponding members of point_3dd p
	inline point_3dd operator += (point_3dd p)
	{
		return point_3dd (x += p.x, y += p.y, z += p.z);
	}

	// returns members multiplied by float n
	inline point_3dd operator * (float n)
	{
		return point_3dd (x * n, y * n, z * n);
	}

	// returns members multiplied by double n
	inline point_3dd operator * (double n)
	{
		return point_3dd (x * n, y * n, z * n);
	}

	// returns abstract member pair (ind, dep) of axis-plane pl
	inline coordd pair (int pl)
	{
		return coordd (
			member[AXIS_PLANE[pl].ind], member[AXIS_PLANE[pl].dep]
		);
	}

	inline coordd xy ()
	{
		return coordd (x, y);
	}

	inline coordd yzd ()
	{
		return coordd (y, z);
	}

	inline coordd zx ()
	{
		return coordd (z, x);
	}

	inline coordd yx ()
	{
		return coordd (y, x);
	}

	inline coordd zy ()
	{
		return coordd (z, y);
	}

	inline coordd xz ()
	{
		return coordd (x, z);
	}

	inline double dx (point_3dd p)
	{
		return p.x - x;
	}

	inline double dy (point_3dd p)
	{
		return p.y - y;
	}

	inline double dz (point_3dd p)
	{
		return p.z - z;
	}

	inline double dist_3d (point_3dd p)
	{
		return sqrt (sqrd (dx (p)) + sqrd (dy (p)) + sqrd (dz (p)));
	}

	inline bool operator == (point_3dd p)
	{
		return approx_equal (x, p.x) && approx_equal (y, p.y) && approx_equal (z, p.z);
	}

	inline bool operator != (point_3dd p)
	{
		return !approx_equal (x, p.x) || !approx_equal (y, p.y) || !approx_equal (z, p.z);
	}

	inline bool undefined ()
	{
		return !real (x) || !real (y) || !real (z);
	}

	inline bool on_seg_inc (point_3dd p1, point_3dd p2)
	{
		return (
			between_inc (x, p1.x, p2.x) &&
			between_inc (y, p1.y, p2.y) &&
			between_inc (z, p1.z, p2.z)
		);
	}

	inline bool on_seg_exc (point_3dd p1, point_3dd p2)
	{
		return (
			between_exc (x, p1.x, p2.x) &&
			between_exc (y, p1.y, p2.y) &&
			between_exc (z, p1.z, p2.z)
		);
	}

	inline int major_comp ()
	{
		if (fabs (x) > fabs (y))
			if (fabs (z) > fabs (x))
				return POINT_3D_Z;
			else 
				return POINT_3D_X;
		else
			if (fabs (z) > fabs (y))
				return POINT_3D_Z;
			else
				return POINT_3D_Y;
	}

	inline point_3d cv_point_3d ()
	{
		return point_3d (x, y, z);
	}

	// coverts coordinates to text
	char* print ()
	{
		static char buff[64];
		sprintf_s (buff, "(%6.4f, %6.4f, %6.4f)", x, y, z);
		return buff;
	}

} vect_3dd;


inline point_3d p3d_xy (coord p, float z)
{
	return point_3d (p.ind, p.dep, z);
}


inline point_3d p3d_xy (coord p, double z)
{
	return point_3d (p.ind, p.dep, float(z));
}


inline point_3d p3d_yz (coord p, float x)
{
	return point_3d (x, p.ind, p.dep);
}


inline point_3d p3d_yz (coord p, double x)
{
	return point_3d (float(x), p.ind, p.dep);
}


inline point_3d p3d_zx (coord p, float y)
{
	return point_3d (p.dep, y, p.ind);
}


inline point_3d p3d_zx (coord p, double y)
{
	return point_3d (p.dep, float(y), p.ind);
}


inline float dx (point_3d p1, point_3d p2)
{
	return p2.x - p1.x;
}


inline float dy (point_3d p1, point_3d p2)
{
	return p2.y - p1.y;
}


inline float dz (point_3d p1, point_3d p2)
{
	return p2.z - p1.z;
}


inline double dx (point_3dd p1, point_3dd p2)
{
	return p2.x - p1.x;
}


inline double dy (point_3dd p1, point_3dd p2)
{
	return p2.y - p1.y;
}


inline double dz (point_3dd p1, point_3dd p2)
{
	return p2.z - p1.z;
}


inline double dist_xy (point_3d p1, point_3d p2)
{
	return dist_2d (p1.xy (), p2.xy ());
}


inline double dist_yz (point_3d p1, point_3d p2)
{
	return dist_2d (p1.yz (), p2.yz ());
}


inline double dist_zx (point_3d p1, point_3d p2)
{
	return dist_2d (p1.zx (), p2.zx ());
}


inline vect_3d vector_3d (double radius, double ax, double ay)
{
	return vect_3d (ax, ay) * radius;
}


inline vect_3dd vector_3dd (double radius, double ax, double ay)
{
	return vect_3dd (ax, ay) * radius;
}


// tests if points are approx co-lateral
inline bool approx_colateral (point_3d p1, point_3d p2)
{
	return approx_equal (p1.y, p2.y);
}


// converts from point_3d to point_3dd
inline point_3dd cv_point_3dd (point_3d p)
{
	return point_3dd (p.x, p.y, p.z);
}


// converts from point_3dd to point_3d
inline point_3d cv_point_3d (point_3dd p)
{
	return point_3d (p.x, p.y, p.z);
}


// tests if all points are approx equal
inline bool approx_equal (point_3d* p, int ct)
{
	for (int i = 0; i < ct; i++)
		if (p[0] != p[i])
			return false;

	return true;
}


// returns the minimum member in list
inline float mini (point_3d* p, int ct, int member)
{
	return mini (&p[0].member[member], ct, sizeof(point_3d));
}


// returns the maximum member in list
inline float maxi (point_3d* p, int ct, int member)
{
	return maxi (&p[0].member[member], ct, sizeof(point_3d));
}


// returns the range of x, y and z of a list of 3d points 
void range (point_3d* p, int ct, point_3d& min, point_3d& max)
{
	min = point_3d (
		mini (p, ct, POINT_3D_X),
		mini (p, ct, POINT_3D_Y),
		mini (p, ct, POINT_3D_Z)
	);

	max = point_3d (
		maxi (p, ct, POINT_3D_X),
		maxi (p, ct, POINT_3D_Y),
		maxi (p, ct, POINT_3D_Z)
	);
}


// LINE_3D
// defines a 3d line
//
// general case:
//    x.m = dx / dz
//    x.b = x intercept
//    y.m = dy / dz
//    y.b = y intercept
//
//    x = x.m * z + x.b
//    y = y.m * z + y.b
//    z = independent var
// dz = 0 case: 
//    x.m = +infinity
//    x.b = z 
//    y.m = dy / dx
//    y.b = y intercept
//
//    x = independent var
//    y = y.m * x + y.b
//    z = x.b
// dx = dy = dz = 0 case:
//    x.m = NaN
//    x.b = x
//    y.m = y
//    y.b = z
//
//    x = x.b
//    y = y.m
//    z = y.b
//
class line_3d {
	inline void set (point_3dd p1, point_3dd p2)
	{
		if (p1 == p2) {
			x = lin_relat (LIN_M_NAN, p1.x);
			y = lin_relat (p1.y, p1.z); 
		}
		else if (approx_zero (p1.dz (p2))) {
			x = lin_relat (LIN_M_VERT, p1.z);
			y = lin_relat (p1.xy (), p2.xy ());
		}
		else {
			x = lin_relat (p1.zx (), p2.zx ());
			y = lin_relat (p1.zy (), p2.zy ());
		}
	}

public:

	lin_relat x, y;

	inline line_3d () {}

	inline line_3d (lin_relat xp, lin_relat yp)
	{
		x = xp;
		y = yp;
	}

	inline line_3d (double mx, double bx, double my, double by)
	{
		x = lin_relat (mx, bx);
		y = lin_relat (my, by);
	}

	inline line_3d (point_3d p1, point_3d p2)
	{
		set (cv_point_3dd (p1), cv_point_3dd (p2));
	}

	inline line_3d (point_3dd p1, point_3dd p2)
	{
		set (p1, p2);
	}

	// constructor by 3d point and slopes
	inline line_3d (point_3d p, m_3d m)
	{
		x = lin_relat (m.x, linear_init (p.zx (), m.x));
		y = lin_relat (m.y, linear_init (p.zy (), m.y));
	}

	// constructor by 3d point and slopes
	inline line_3d (point_3dd p, m_3d m)
	{
		x = lin_relat (m.x, linear_init (p.zx (), m.x));
		y = lin_relat (m.y, linear_init (p.zy (), m.y));
	}

	// returns function of x
	inline point_3dd func_x (double xc)
	{
		if (nan (x.m))
			return VECT_3DD_UNDEF;
		else if (infinite (x.m))
			return point_3dd (xc, y.func (xc), x.b);
		else {
			double zc = x.inv_func (xc);
			return point_3dd (xc, y.func (zc), zc);
		}
	}

	// returns function of y
	inline point_3dd func_y (double yc)
	{
		if (nan (x.m))
			return VECT_3DD_UNDEF;
		else if (infinite (x.m))
			return point_3dd (y.inv_func (yc), yc, x.b);
		else {
			double zc = y.inv_func (yc);
			return point_3dd (x.func (zc), yc, zc);
		}
	}

	// returns function of z
	inline point_3dd func_z (double zc)
	{
		if (nan (x.m))
			return VECT_3DD_UNDEF;
		else if (infinite (x.m))
			return VECT_3DD_UNDEF;
		else
			return point_3dd (x.func (zc), y.func (zc), zc);
	}

	// get 3d coordinates of point on line from x, y coordinates
	// assumes coordd xy is on top of the xy component of the 3d line
	inline point_3dd func_xy (coordd xy)
	{
		// z = m1 x + b
		if (nan (x.m)) // line 3d undefined
			return VECT_3DD_UNDEF;
		else if (infinite (x.m)) // xy line case
			return point_3dd (xy.x, xy.y, x.b);
		else 
			if (approx_zero (x.m))
				if (approx_zero (y.m))
					return VECT_3DD_UNDEF;
				else
					return point_3dd (xy.x, xy.y, y.inv_func (xy.y));
			else
				return point_3dd (xy.x, xy.y, x.inv_func (xy.x));
	}

	// get 3d coordinates of point on line from y, z coordinates
	// assumes coordd yz is on top of the yz component of the 3d line
	inline point_3dd func_yz (coordd yz)
	{
		if (nan (x.m))
			return VECT_3DD_UNDEF;
		else if (infinite (x.m))
			if (approx_zero (y.m))
				return VECT_3DD_UNDEF;
			else
				return point_3dd (y.inv_func (yz.ind), yz.ind, yz.dep);
		else
			return point_3dd (x.func (yz.dep), yz.ind, yz.dep);
	}

	// gets 3d coordinates of point on line from z, x coordinates
	// assumes coord zx is on top of the zx component of the 3d line
	inline point_3dd func_zx (coordd zx)
	{
		if (nan (x.m))
			return VECT_3DD_UNDEF;
		else if (infinite (x.m))
			if (infinite (y.m))
				return VECT_3DD_UNDEF;
			else
				return point_3dd (zx.dep, y.func (zx.dep), zx.ind);
		else
			return point_3dd (zx.dep, y.func (zx.ind), zx.ind);
	}

	// calculates 2-d xy component of 3d line
	inline lin_relat xy_comp ()
	{
		if (infinite (x.m))
			return y;
		else if (approx_zero (x.m))
			if (approx_zero (y.m))
				return LIN_RELAT_UNDEF;
			else
				return lin_relat (LIN_M_VERT, x.b);
		else {
			double m = y.m / x.m;
			return lin_relat (m, y.b - m * x.b);
		}
	}

	vect_3dd vect ()
	{
		if (nan (x.m))
			return VECT_3DD_UNDEF;
		else if (infinite (x.m))
			return vect_3dd (y.vect (), 0.0);
		else {
			double dz = 1.0 / sqrt (sqrd (x.m) + sqrd (y.m) + 1.0);
			return vect_3dd (x.m * dz, y.m * dz, dz);
		}
	}

	// tests if 3d line l is equal to 3d line
	inline bool operator == (line_3d l)
	{
		return (
			approx_equal (x.m, l.x.m) &&
			approx_equal (x.b, l.x.b) &&
			approx_equal (y.m, l.y.m) &&
			approx_equal (y.b, l.y.b)
		);
	}

	// converts 3d line to text
	char* print ()
	{
		static char buff[128];

		if (nan (x.m)) 
			sprintf_s (buff, "Undefined: (%6.4f, %6.4f, %6.4f)", x.b, y.m, y.b);
		else if (infinite (x.m))
			sprintf_s (buff, "y = %6.4f x + %6.4f", y.m, y.b);
		else
			sprintf_s (buff, 
				"x = %6.4f z + %6.4f\n"
				"y = %6.4f z + %6.4f", 
				x.m, x.b, 
				y.m, y.b
			);

		return buff;
	}
};


line_3d xy_to_3d (lin_relat xy, double z)
{
	return line_3d (lin_relat (INF, z), xy);
}


line_3d yz_to_3d (lin_relat yz, double x)
{
	return line_3d (lin_relat (0.0, x), yz.inverse ());
}


line_3d zx_to_3d (lin_relat zx, double y)
{
	return line_3d (zx, lin_relat (0.0, y));
}


line_3d yx_to_3d (lin_relat yx, double z)
{
	return line_3d (lin_relat (INF, z), yx.inverse ());
}


line_3d zy_to_3d (lin_relat zy, double x)
{
	return line_3d (lin_relat (0.0, x), zy);
}


line_3d xz_to_3d (lin_relat xz, double y)
{
	return line_3d (xz.inverse (), lin_relat (0.0, y));
}


class angle_3d {
public:
	double x, y;

	inline angle_3d () {}

	inline angle_3d (double xp, double yp)
	{
		x = xp;
		y = yp;
	}

	inline angle_3d perpendicular ()
	{
		return angle_3d (simp_angle (x + PI_2), simp_angle (y + PI_2));
	}

	inline angle_3d mirror ()
	{
		return angle_3d (simp_angle (x + PI), simp_angle (y + PI));
	}
};


class ray_3d {
public:
	point_3dd orig;
	angle_3d angle;

	inline ray_3d (point_3dd o, angle_3d a)
	{
		orig = o;
		angle = a;
	}

	inline ray_3d (point_3d o, angle_3d a)
	{
		orig = cv_point_3dd (o);
		angle = a;
	}

	// constructor from point_3dd and angles x and y	
	inline ray_3d (point_3dd o, double x, double y)
	{
		orig = o;
		angle = angle_3d (x, y);
	}

	// constructor from point_3d and angles x and y
	inline ray_3d (point_3d o, double x, double y)
	{
		orig = cv_point_3dd (o);
		angle = angle_3d (x, y);
	}
};


// defines a geometric 3d plane
//
// general case:
//    z = mx * x + my * y + b
//    mx = dz/dx
//    my = dz/dy
//    b = z-intercept
// case PLANE_3D_XY_VALU
//    y = my * x + b
//    mx = PLANE_3D_XY_VALU
//    my = dx/dy
//    b = x-intercept
// case PLANE_3D_Y_VALU
//    y = b
//    mx = PLANE_3D_Y_VALU
//	  my = +inf
//    b = y-intercept
//
class plane_3d {
	// calculates plane by 3 3d points
	void set (point_3d p1, point_3d p2, point_3d p3)
	{
		point_3dd ref, diff1, diff2;
		bool y_plane = false;

		if ((p1 == p2) || (p1 == p3)) { // plane undefined
			mx = LIN_M_ERROR;
			y = lin_relat (p1.x, p1.y);
		}
		else if (p2 == p3) { // plane undefined
			mx = LIN_M_ERROR;
			y = lin_relat (p2.x, p2.y);
		}
		else {
			if (approx_colateral (p1, p2))
				if (approx_colateral (p2, p3))  // y1 = y2 = y3
					y_plane = true;
				else { // y2 != y3
					diff1 = cv_point_3dd (p2);
					diff2 = cv_point_3dd (p3);
					ref = cv_point_3dd (p1);
				}
			else {  // y1 != y2
				diff1 = cv_point_3dd (p1);
				diff2 = cv_point_3dd (p2);
				ref = cv_point_3dd (p3);			
			}

			if (y_plane) {
				mx = LIN_M_NAN;
				y = lin_relat (0.0, p1.y);
			}
			else {
				line_3d l (diff1, diff2);
				point_3dd ref2 = l.func_y (ref.y);
				double m = linear_delta (ref.xz (), ref2.xz ());

				if (infinite (m)) {
					mx = LIN_M_VERT;
					y = lin_relat (diff1.xy (), diff2.xy ());
				}
				else {
					mx = m;
					y = lin_relat (
						coordd (diff1.y, linear_init (diff1.xz (), m)),
						coordd (diff2.y, linear_init (diff2.xz (), m))
					);
				}
			}
		}
	}

public:

	double mx; // dz/dx slope (xz line)
	lin_relat y; // dz/dy (yz line) line when x = 0

	// void constructor
	inline plane_3d () {}

	// constructor by xz slope and yz line
	inline plane_3d (double pmx, lin_relat py)
	{
		mx = pmx;
		y = py;
	}

	// constructor by list of vertices
	inline plane_3d (point_3d* p)
	{
		set (p[0], p[1], p[2]);
	}

	// constructor by 3 vertices
	inline plane_3d (point_3d p1, point_3d p2, point_3d p3)
	{
		set (p1, p2, p3);
	}

	// returns one of three relevant main axis-planes 
	inline int relevant_plane ()
	{
		if (nan (mx)) // 'y = c' plane case
			return AXIS_PLANE_ZX;
		else if (infinite (mx)) // 'y = mx + b' plane case
			return AXIS_PLANE_YZ;
		else // general case
			return AXIS_PLANE_XY;
	}

	// returns x on plane given y and z
	inline double func_x (double yc, double zc)
	{
		if (approx_zero (mx))
			return RESULT_UNDEF;
		else if (!real (mx)) // y = mx + b plane case
			return y.inv_func (yc);
		else // general case
			return (zc - y.m * yc - y.b) / mx;
	}

	// returns y on plane given x and z
	inline double func_y (double xc, double zc)
	{
		if (!real (mx)) // xy plane case
			if (infinite (y.m)) 
				return RESULT_UNDEF;
			else
				return y.func (xc);
		else // general plane case
			if (approx_zero (y.m))
				return RESULT_UNDEF;
			else
				return (zc - mx * xc - y.b) / y.m;
	}

	// returns z on plane given x and y
	inline double func_z (double xc, double yc)
	{
		if (!real (mx))
			return RESULT_UNDEF;
		else 
			return xc * mx + yc * y.m + y.b;
	}

	// returns xy line on plane given z
	inline lin_relat func_xy (double zc)
	{
		if (!real (mx))
			return y;
		else
			return lin_relat (-mx / y.m, (zc - y.b) / y.m);
	}

	// returns yz line on plane given x
	inline lin_relat func_yz (double xc)
	{
		return lin_relat (y.m, y.b + mx * xc);
	}

	// returns xz line on plane given y
	inline lin_relat func_xz (double yc)
	{
		return lin_relat (mx, y.func (yc));
	}

	// returns point of intersection of 3d line l with plane
	point_3dd intersect (line_3d l)
	{
		if (l.x.m = LIN_M_NAN || (mx == LIN_M_ERROR)) // undefined line_3d or plane_3d case
			return VECT_3DD_UNDEF;		
		else if (!real (mx)) // y = mx + b plane_3d case
			return point_3dd (y.intersect (l.xy_comp ()), l.x.b);
		else // z = m1x + m2y + b plane_3d case
			if (infinite (l.x.m)) // y = mx + b line_3d case
				return point_3dd (l.y.intersect (func_xy (l.x.b)), l.x.b);
			else // general line_3d case
				return l.func_z (
					(mx * l.x.b + y.m * l.y.b + y.b) /
					(1.0 - mx * l.x.m - y.m * l.y.m)
				);
	}

	// returns point of intersection of 3d ray r with plane
	point_3dd intersect (ray_3d r)
	{
		return intersect (line_3d (r.orig, vect_3dd (r.angle.x, r.angle.y)));
	}

	angle_3d rel_normal_angles ()
	{
		if (nan (mx))
			return ANGLE_3D_POS_Y;
		else if (!real (mx))
			if (infinite (y.m))
				return ANGLE_3D_POS_X;
			else if (y.m > 0)
				return angle_3d (-PI_2, atan (y.m) + PI_2);
			else
				return angle_3d (PI_2, atan (y.m) + PI_2);
		else {
			double ax = atan (mx);
			return angle_3d (PI_2 - ax, atan (cos (ax) / y.m));
		}
	}

	// converts plane slopes to 3d line slopes
	xy_pair rel_slopes_3d ()
	{		
		if (nan (mx))
			return m_3d (LIN_M_NAN, LIN_M_VERT);
		else if (infinite (mx))
			return xy_pair (LIN_M_VERT, y.m);
		else
			return xy_pair (1.0 / mx, 1.0 / y.m);			
	}

	// converts plane slopes to a pair of 3d line slopes
	// returns if slopes are real (true) or arbitrary (false)
	bool rel_slopes_3d (m_3d& m3dx, m_3d& m3dy)
	{
		if (nan (mx)) { // plane case y = c 
			m3dx = SLOPE_3D_X_AXIS;
			m3dy = SLOPE_3D_Z_AXIS;
			return false; // arbitrary slopes
		}
		else if (infinite (mx)) 
			if (infinite (y.m)) { // plane sub-case x = c
				m3dx = SLOPE_3D_Z_AXIS;
				m3dy = SLOPE_3D_Y_AXIS;
				return false; // arbitrary slopes
			}
			else { // plane case y = mx + b
				m3dx = SLOPE_3D_Z_AXIS;
				m3dy = m_3d (0.0, y.m);
				return true; // real slopes
			}
		else {
			if (approx_zero (mx)) 
				m3dx = SLOPE_3D_X_AXIS;
			else 
				m3dx = m_3d (1.0 / mx, 0.0);

			if (approx_zero (y.m))
				m3dy = SLOPE_3D_Y_AXIS;
			else
				m3dy = m_3d (-mx, 1.0 / y.m);
			// atan (cos (atan (mx)) / y.m)

			return true; // real slopes
		}
	}

	// returns how 3d point p spatially relates to the plane with low precision
	// returns:
	//     -1 behind
	//     +1 in front
	//      0 on plane
	// under general case the z coordinate is judged
	// under special case:
	//    y.m is finite: y is judged (***************incorrect************)
	//    y.m is infinite: x is judged (*************incorrect**************)
	int relative (point_3d p) 
	{
		if (real (mx)) { // general plane 3d case
			lin_relat xz = func_xz (p.y); 
			return trin_cmp (p.z, float(xz.func (p.x)));
		}
		else // y = mx + b plane 3d case
			return y.relative_dep (p.xy ());
	}

	// returns how 3d point p spatially relates to the plane with high precision
	int relative (point_3dd p)
	{
		if (real (mx)) { // general plane 3d case
			lin_relat xz = func_xz (p.y);
			return trin_cmp (p.z, xz.func (p.x));
		}
		else // y = mx + b plane 3d case
			return y.relative_dep (p.xy ());
	}

	// converts plane data to text
	char* print ()
	{
		static char buff[128];
		char* ret;

		if (nan (mx)) {
			sprintf_s (buff, "Undefined: x = %6.4f, y = %6.4f", y.m, y.b);
			ret = buff;
		}
		else if (infinite (mx))
			ret = y.print ("x", "y");
		else {
			sprintf_s (buff, "z = %6.4f x + %6.4f y + %6.4f", mx, y.m, y.b);
			ret = buff;
		}

		return ret;
	}
};


class tri_3d {
public:
	point_3d vert[3];

	// void constructor
	inline tri_3d () {}

	// constructor by 3 3d points
	inline tri_3d (point_3d p1, point_3d p2, point_3d p3)
	{
		vert[0] = p1;
		vert[1] = p2;
		vert[2] = p3;
	}

	// constructor by array of 3 3d points
	inline tri_3d (point_3d* p)
	{
		for (int i = 0; i < 3; i++)
			vert[i] = p[i];
	}

	// returns abstract (ind, dep) components in axis-plane pl of 3d triangle
	inline tri_2d axis_plane (int pl)
	{
		return tri_2d (vert[0].pair (pl), vert[1].pair (pl), vert[2].pair (pl));
	}

	// returns 2d tri of (x, y) components of 3d triangle	
	inline tri_2d xy ()
	{
		return axis_plane (AXIS_PLANE_XY);
	}

	// returns 2d tri of (y, z) components of 3d triangle	
	inline tri_2d yz ()
	{
		return axis_plane (AXIS_PLANE_YZ);
	}

	// returns 2d tri of (z, x) components of 3d triangle	
	inline tri_2d zx ()
	{
		return axis_plane (AXIS_PLANE_ZX);
	}

	// returns:
	//    +1: if p is inside tri and clock-wise in relevant plane
	//    -1: if p is inside tri and counter-clock-wise in relevant plane
	//     0: if p is outside tri
	int relative (point_3d p)
	{
		plane_3d plane (vert);
		int pl = plane.relevant_plane ();
		tri_2d tri = axis_plane (pl);
		
		return tri.relative (p.pair (pl));		
	}

	// returns:
	//     +1: if intersection of 3d line l is in tri inclusively and tri is clock-wise in relevant plane
	//     -1: if intersection of 3d line l is in tri inclusively and tri is counter-clock-wise in relevant plane
	//      0: if parallel or intersection is outside tri
	int intersect (line_3d l, point_3dd& pen)
	{
		plane_3d plane (vert);		
		pen = plane.intersect (l);		
		
		return relative (pen.cv_point_3d ());
	}

	// returns:
	//     3d point if point is on tri inclusively
	//	   VECT_3DD_UNDEF if parallel or intersection lies outside triangle	
	point_3dd intersect (line_3d l)
	{
		plane_3d plane (vert);		
		point_3dd pen = plane.intersect (l);		
		
		if (relative (pen.cv_point_3d ()))
			return pen;
		else
			return VECT_3DD_UNDEF;
	}	

	char* print ()
	{
		static char result[256];
		char s1[64], s2[64], s3[64];

		strcpy_s (s1, vert[0].print ());
		strcpy_s (s2, vert[1].print ());
		strcpy_s (s3, vert[2].print ());

		sprintf_s (result, "%s\n%s\n%s", s1, s2, s3);
		return result;
	}
};


#endif // !INC_MATH_3D