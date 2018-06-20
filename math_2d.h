// ================================================================================
// 2D Math definitions
// Nathan G. Smith
// ================================================================================

#ifndef INC_MATH_2D
#define INC_MATH_2D

#include <math.h>
#include "math_1d.h"

// ******************** 2d constant definitions *************************

// common float 2d vector definitions
#define ORIG_2D point_2d (0.0f, 0.0f)
#define VECT_2D_UP vect_2d (0.0f, 1.0f)
#define VECT_2D_DOWN vect_2d (0.0f, -1.0f)
#define VECT_2D_LEFT vect_2d (-1.0f, 0.0f)
#define VECT_2D_RIGHT vect_2d (1.0f, 0.0f)
#define VECT_2D_ZERO ORIG_2D
#define VECT_2D_UNDEF vect_2d (-INF, INF)

// common double 2d vector definitions
#define ORIG_2DD point_2dd (0.0, 0.0)
#define VECT_2DD_UP vect_2dd (0.0, 1.0)
#define VECT_2DD_DOWN vect_2dd (0.0, -1.0)
#define VECT_2DD_LEFT vect_2dd (-1.0, 0.0)
#define VECT_2DD_RIGHT vect_2dd (1.0, 0.0)
#define VECT_2DD_ZERO ORIG_2DD
#define VECT_2DD_UNDEF vect_2dd (Q_NAN, -INF)

// to pass coordinates by index
#define POINT_2D_X 0
#define POINT_2D_Y 1
#define COORD_IND 0
#define COORD_DEP 1

// special line case definitions
#define LIN_M_VERT INF // line where independent var = c
#define LIN_M_NAN Q_NAN // line where d dep = d ind = 0
#define LIN_M_ERROR S_NAN // error calculating slope (bad input)
#define LIN_RELAT_UNDEF lin_relat (S_NAN, S_NAN) // returns error calculating line

// definitions for point relative to edge
#define POINT_REL_EDGE_CW_INSIDE -1
#define POINT_REL_EDGE_CW_OUTSIDE 1
#define POINT_REL_EDGE_ON 0
#define POINT_REL_EDGE_CCW_INSIDE POINT_REL_EDGE_CW_OUTSIDE
#define POINT_REL_EDGE_CCW_OUTSIDE POINT_REL_EDGE_CW_INSIDE

// definitions for point relative to triangle
#define POINT_REL_TRI_CW_INSIDE -1
#define POINT_REL_TRI_CW_OUTSIDE 1
#define POINT_REL_TRI_ON 0
#define POINT_REL_TRI_CCW_INSIDE POINT_REL_TRI_CW_OUTSIDE
#define POINT_REL_TRI_CCW_OUTSIDE POINT_REL_TRI_CW_INSIDE

// ************************* 2-d coordinate definitions ***************************

// discrete x, y values
class disc_2d { 
public:
	union {
		int x, ind;
	};
	
	union {
		int y, dep;
	};

	disc_2d () {}

	disc_2d (int xp, int yp)
	{
		x = xp;
		y = yp;
	}
};


// floating point 2-d coordinate
typedef class coord { 
public:
	union {
		// data members (x, y) or anonymous (independent, dependent)
		struct {
			union {
				float ind, x;
			};	
			
			union {
				float dep, y;
			};
		};

		// to access data members via index
		float member[2];
	};

	// void constructor
	inline coord () {}

	// constructor for 2 doubles
	inline coord (double i, double d) 
	{
		ind = float(i);
		dep = float(d);
	}

	// returns vector of radius 1.0 from an angle
	inline coord (double a)
	{
		ind = float(cos (a));
		dep = float(sin (a));
	}

	inline coord operator + (coord c)
	{
		return coord (x + c.x, y + c.y);
	}

	inline coord operator - (coord c)
	{
		return coord (x - c.x, y - c.y);
	}

	inline coord operator += (coord c)
	{
		return coord (x += c.x, y += c.y);
	}

	inline coord operator -= (coord c)
	{
		return coord (x -= c.x, y -= c.y);
	}

	// tests for approximate equality to another coord
	inline bool operator == (coord c)
	{
		return approx_equal (ind, c.ind) && approx_equal (dep, c.dep);
	}

	// tests for approximate inequality to another coord
	inline bool operator != (coord c)
	{
		return !approx_equal (ind, c.ind) || !approx_equal (dep, c.dep);
	}

	// multiplies both members by constant n
	inline coord operator * (double n)
	{
		return coord (double(ind) * n, double(dep) * n);
	}

	// divides both members by constant n
	inline coord operator / (double n)
	{
		return coord (double(ind) / n, double(dep) / n);
	}

	// tests if either member is +/- infinity or NaN
	inline bool invalid ()
	{
		return !real (ind) || !real (dep);
	}

	// swaps independent and dependent variables
	inline coord inverse ()
	{
		return coord (dep, ind);
	}

	// converts the 2d vector to a slope
	inline double slope ()
	{
		return dep / ind;
	}

	inline float dx (coord c)
	{
		return x - c.x;
	}

	inline float dy (coord c)
	{
		return y - c.y;
	}

	inline float di (coord c)
	{
		return ind - c.ind;
	}

	inline float dd (coord c)
	{
		return dep - c.dep;
	}

	// tests if coord is within 2d coord segment inclusively
	inline bool on_seg_inc (coord p1, coord p2)
	{
		return between_inc (ind, p1.ind, p2.ind) && between_inc (dep, p1.dep, p2.dep);
	}

	// tests if coord is within 2d coord segment exclusively
	inline bool on_seg_exc (coord p1, coord p2)
	{
		return between_exc (ind, p1.ind, p2.ind) && between_exc (dep, p1.dep, p2.dep);
	}

	// gets the component of vector that is furthest from zero
	inline int major_comp ()
	{
		if (fabs (x) > fabs (y))
			return POINT_2D_X;
		else
			return POINT_2D_Y;
	}

	// converts coord into text
	char* print ()
	{
		static char buff[64];
		sprintf_s (buff, "(%6.4f, %6.4f)", ind, dep);
		return buff;
	}

} point_2d, vect_2d;


// double floating point 2-d coordinate
typedef class coordd { 
public:
	union {
		// data members (x, y) or anonymous (independent, dependent)
		struct {
			union {
				double ind, x;
			};	
			
			union {
				double dep, y;
			};
		};

		// to access data members via index
		double member[2];
	};

	// void constructor
	coordd () {}

	// constructor for 2 doubles
	coordd (double i, double d) 
	{
		ind = i;
		dep = d;
	}

	// constructor by radius 1.0, angle a
	coordd (double a)
	{
		ind = cos (a);
		dep = sin (a);
	}

	// constructor by a coord
	coordd (coord c)
	{
		ind = c.ind;
		dep = c.dep;
	}

	inline coordd operator + (coordd c)
	{
		return coordd (x + c.x, y + c.y);
	}

	inline coordd operator - (coordd c)
	{
		return coordd (x - c.x, y - c.y);
	}

	inline coordd operator += (coordd c)
	{
		return coordd (x += c.x, y += c.y);
	}

	inline coordd operator -= (coordd c)
	{
		return coordd (x -= c.x, y -= c.y);
	}

	// tests for approximate equality to another coordd
	inline bool operator == (coordd c)
	{
		return approx_equal (ind, c.ind) && approx_equal (dep, c.dep);
	}

	// tests for approximate inequality to another coordd
	inline bool operator != (coordd c)
	{
		return !approx_equal (ind, c.ind) || !approx_equal (dep, c.dep);
	}

	// multiplies both members by constant n
	inline coordd operator * (double n)
	{
		return coordd (ind * n, dep * n);
	}

	// divides both members by constant n
	inline coordd operator / (double n)
	{
		return coordd (ind / n, dep / n);
	}

	// tests if either member is an invalid number
	inline bool invalid ()
	{
		return !real (ind) || !real (dep);
	}

	// swaps independent and dependent variables
	inline coordd inverse ()
	{
		return coordd (dep, ind);
	}

	// converts to coord
	inline coord cv_coord ()
	{
		return coord (ind, dep);
	}

	inline double dx (coordd c)
	{
		return x - c.x;
	}

	inline double dy (coordd c)
	{
		return y - c.y;
	}

	// tests if coordd is on 2d coordd segment inclusively
	inline bool on_seg_inc (coordd p1, coordd p2)
	{
		return between_inc (ind, p1.ind, p2.ind) && between_inc (dep, p1.dep, p2.dep);
	}

	// tests if coordd is on 2d coordd segment exclusively
	inline bool on_seg_exc (coordd p1, coordd p2)
	{
		return between_exc (ind, p1.ind, p2.ind) && between_exc (dep, p1.dep, p2.dep);
	}

	// gets the component of vector that is furthest from zero
	inline int major_comp ()
	{
		if (fabs (x) > fabs (y))
			return POINT_2D_X;
		else
			return POINT_2D_Y;
	}

	// converts coordd into text
	char* print () 
	{
		static char buff[64];
		sprintf_s (buff, "(%6.4f, %6.4f)", round (ind, .0001), round (dep, .0001));
		return buff;
	}

} point_2dd, vect_2dd, xy_pair;


// converts coordd to coord
coord cv_coord (coordd p)
{
	return coord (p.ind, p.dep);
}


// converts coord to coordd
coordd cv_coordd (coord p)
{
	return coordd (p.ind, p.dep);
}


void rotate (coord* p, int ct, double angle, coord center)
{
	double sina = sin (angle);
	double cosa = cos (angle);

	for (int i = 0; i < ct; i++) {
		double dx = double(center.dx (p[i].x));
		double dy = double(center.dy (p[i].y));
		p[i] = center + coord (cosa * dx - sina * dy, cosa * dy + sina * dx);
	}
}


void rotate (coordd* p, int ct, double angle, coordd center)
{
	double sina = sin (angle);
	double cosa = cos (angle);

	for (int i = 0; i < ct; i++) {
		double dx = center.dx (p[i].x);
		double dy = center.dy (p[i].y);
		p[i] = center + coordd (cosa * dx - sina * dy, cosa * dy + sina * dx);
	}
}


// gets the minimum value of member in list
float mini (coord* p, int ct, int member)
{
	return mini (&p[0].member[member], ct, sizeof (coord));
}


// gets the maximum value of member in list
float maxi (coord* p, int ct, int member)
{
	return maxi (&p[0].member[member], ct, sizeof (coord));
}


// gets max and min independent and dependent vars
void get_range (coord* p, int ct, coord& min, coord& max)
{
	min = coord (mini (p, ct, POINT_2D_X), mini (p, ct, POINT_2D_Y));
	max = coord (maxi (p, ct, POINT_2D_X), maxi (p, ct, POINT_2D_Y));
}

// **************************** 2d linear definitions ************************************

// calculates rate of change: delta dependent / delta independent for 'coord'
inline double linear_delta (coord p1, coord p2)
{
	if (approx_equal (p1.ind, p2.ind))
		if (approx_equal (p1.dep, p2.dep))
			return LIN_M_NAN;
		else if (p2.dep > p1.dep)
			return LIN_M_VERT;
		else
			return -LIN_M_VERT;
	else
		return double(p2.dep - p1.dep) / double(p2.ind - p1.ind);
}


// calculates rate of change: delta dependent / delta independent for 'coordd'
inline double linear_delta (coordd p1, coordd p2)
{
	if (approx_equal (p1.ind, p2.ind))
		if (approx_equal (p1.dep, p2.dep))
			return LIN_M_NAN;
		else if (p2.dep > p1.dep)
			return LIN_M_VERT;
		else
			return -LIN_M_VERT;
	else	
		return (p2.dep - p1.dep) / (p2.ind - p1.ind);
}


// calculates dependent value when independent = 0.0 for 'coord'
inline double linear_init (coord p, double m)
{
	if (nan (m))
		return p.dep;
	else if (infinite (m))
		return p.ind;			
	else
		return double(p.dep) - m * double(p.ind);		
}


// calculates dependent value when independent = 0.0 for 'coordd'
inline double linear_init (coordd p, double m)
{
	if (nan (m))
		return p.dep;
	else if (infinite (m))
		return p.ind;			
	else
		return p.dep - m * p.ind;
}


// linear relationship definition
//
// case LIN_RELAT_UNDEF:
//    m = NaN
//    b = dep
// case LIN_RELAT_IND:
//    m = +/- infinity
//    b = ind
// general case:
//    m = delta dependent / delta independent
//    b = dependent intercept
typedef class lin_relat {
public:
	// slope (delta dependent / delta independent)
	double m;
	
	// dependent intercept 
	double b;

	// void constructor 
	inline lin_relat () {}

	// constructor by 2 doubles
	inline lin_relat (double mp, double bp)
	{
		m = mp;
		b = bp;
	}

	// constructor by a coord and slope
	inline lin_relat (coord p, double mp)
	{
		m = mp;
		b = linear_init (p, mp);
	}

	// constructor by a coordd and slope
	inline lin_relat (coordd p, double mp)
	{
		m = mp;
		b = linear_init (p, mp);
	}

	// constructor by 2 coords
	inline lin_relat (coord p1, coord p2)
	{
		m = linear_delta (p1, p2);
		b = linear_init (p1, m);
	}

	// constructor by 2 coordds
	inline lin_relat (coordd p1, coordd p2)
	{
		m = linear_delta (p1, p2);
		b = linear_init (p1, m);
	}

	// returns if any members are not real numbers
	inline bool invalid ()
	{
		return !real (b);
	}

	// linear calculation of dependent by double independent
	inline double func (double ind)
	{
		if (nan (m) || infinite (m))
			return RESULT_UNDEF;
		else
			return m * ind + b;
	}

	// linear calculation of independent by double dependent
	inline double inv_func (double dep)
	{
		if (nan (m) || approx_zero (m))
			return RESULT_UNDEF;
		else if (infinite (m))
			return b;
		else
			return (dep - b) / m;
	}

	// linear calculation of coord by double independent
	inline coord func_coord (double ind)
	{
		return coord (float(ind), float(func (ind)));
	}

	// linear calculation of coordd by double independent
	inline coordd func_coordd (double ind)
	{
		return coordd (ind, func (ind));
	}

	// tests coord p relative to the lin_relat with float precision
	// returns:
	//    -1: negative side
	//     0: directly on
	//    +1: positive side
	int relative_dep (coord p)
	{
		if (nan (m))
			return 0;
		else if (infinite (m))
			return trin_cmp (p.ind, float(b));
		else 
			return trin_cmp (float(func (p.ind)), p.dep);
	}

	// tests coordd p relative to the lin_relat with double precision
	int relative_dep (coordd p)
	{
		if (nan (m))
			return 0;
		else if (infinite (m))
			return trin_cmp (p.ind, b);
		else 
			return trin_cmp (func (p.ind), p.dep);
	}

	// tests if the line is approx equal to another line
	bool operator == (lin_relat l)
	{
		if (nan (m) || nan (l.m))
			return false;
		else if (infinite (m))
			return infinite (l.m) && approx_equal (b, l.b);
		else if (infinite (l.m))
			return false;
		else 
			return (approx_equal (m, l.m) && approx_equal (b, l.b));
	}

	// tests if the line is not approx equal to another line
	bool operator != (lin_relat l)
	{
		if (nan (m) || nan (l.m))
			return false;
		else if (infinite (m))
			return !infinite (l.m) || !approx_equal (b, l.b);
		else if (infinite (l.m))
			return true;
		else 
			return !approx_equal (m, l.m) || !approx_equal (b, l.b);
	}

	// returns a line on coordd 'p' perpendicular to line
	lin_relat perp (coordd p)
	{
		if (nan (m) || p.invalid ())
			return LIN_RELAT_UNDEF;
		else if (infinite (m))
			return lin_relat (0.0, p.dep);
		else if (approx_zero (m))
			return lin_relat (INF, p.ind); 
		else 
			return lin_relat (p, -1.0 / m);
	}

	// returns a line on coord p perpendicular to line
	lin_relat perp (coord p)
	{
		return perp (cv_coordd (p));
	}

	// returns linear relationship where independent and dependent axes are swapped
	lin_relat inverse ()
	{
		if (nan (m))
			return LIN_RELAT_UNDEF;
		else if (infinite (m))
			return lin_relat (0.0, b);
		else if (approx_zero (m))
			return lin_relat (LIN_M_VERT, b);
		else
			return lin_relat (1.0 / m, -b / m);
	}

	// calculates independent coordinate of intersection of line with another line 
	coordd intersect (lin_relat l)
	{
		if (nan (m) || nan (l.m) || approx_equal (m, l.m))
			return VECT_2DD_UNDEF;
		else if (infinite (m)) 
			return l.func (b);
		else if (infinite (l.m))
			return func (l.b);
		else 
			return func ((b - l.b) / (l.m - m));
	}

	// converts slope to a 2d vector with radius 1.0
	// note: vector dependent is always (+)
	vect_2dd vect ()
	{
		double ind = 1.0 / sqrt (1.0 + m * m); // independent
		return vect_2dd (ind, ind * m);
	}

	// converts line to text with user lables for ind & dep
	char* print (const char* ind, const char* dep)
	{
		static char buff[128];
		
		if (nan (m))
			sprintf_s (buff, "Undefined, %s = %6.4f", dep, b);
		else if (infinite (m))
			sprintf_s (buff, "%s = %6.4f", ind, b);
		else
			sprintf_s (buff, "%s = %6.4f %s + %6.4f", dep, m, ind, b);

		return buff;
	}

	// converts line to text
	char* print ()
	{
		return print ("ind", "dep");
	}

} line_2d;


// point relative to segment
// returns:
//    -1 if inside a clock-wise edge or
//       outside a counter-clock-wise edge
//    +1 if outside a clock-wise edge or
//       inside a counter-clock-wise edge
//     0 if on edge
int point_rel_edge (coord p, coord e1, coord e2)
{
	if (approx_equal (e1.ind, e2.ind))
		return trin_cmp (e1.dep, e2.dep) * trin_cmp (p.ind, e1.ind);
	else {
		line_2d l (e1, e2);
		return l.relative_dep (p) * trin_cmp (e2.ind, e1.ind);
	}
}


class ray_2d {
public:
	coordd center;
	double angle;

	ray_2d (coordd c, double a)
	{
		center = c;
		angle = a;
	}    

	lin_relat line ()
	{
		return lin_relat (center, atan (angle));
	}
};

// ****************************** 2d triangle definitions*************************************

class tri_2d {
public:
	coord vert[3];

	// void constructor
	inline tri_2d () {}

	inline tri_2d (coord v1, coord v2, coord v3)
	{
		vert[0] = v1;
		vert[1] = v2;
		vert[2] = v3;
	}

	inline tri_2d (coord* v)
	{
		for (int i = 0; i < 3; i++)
			vert[i] = v[i];
	}

	// point relative to triangle
	// returns:
	//    -1 if inside a clock-wise tri
	//    +1 if inside a counter-clock-wise tri
	//     0 if outside either kind of tri
	inline int relative (coord p)
	{
		int rel_1 = point_rel_edge (p, vert[0], vert[1]);
		int rel_2 = point_rel_edge (p, vert[1], vert[2]);
		int rel_3 = point_rel_edge (p, vert[2], vert[0]);

		if ((rel_1 == rel_2) && (rel_2 == rel_3))
			return rel_1;
		else
			return 0;
	}
};

// ****************************** 2d trig functions ****************************************

// converts radians to degrees 
inline double rad_to_deg (double a)
{
	return a * (180.0 / PI);
}


// converts degrees to radians
inline double deg_to_rad (double a)
{
	return a * (PI / 180.0);
}


// returns equivalent angle in [-Pi, Pi]
inline double simp_angle (double a)
{
	return atan2(sin(a), cos(a));
}


// returns equivalent angle in [0, 2 * Pi]
inline double simp_angle_pos (double angle)
{
	double a = atan2 (sin (angle), cos (angle));

	if (a > 0)
		return a;	
	else
		return a + 2 * PI;
}


// calculates angle if the independent and dependent axes were swapped
inline double invert_angle (double a) 
{
	return simp_angle (PI_2 - a);
}


// returns the shortest distance between 2 coords
inline double dist_2d (coord p1, coord p2)
{
	return sqrt (sqrd (p2.ind - p1.ind) + sqrd (p2.dep - p1.dep));
}


// returns the shortest distance between 2 coordds
inline double dist_2d (coordd p1, coordd p2)
{
	return sqrt (sqrd (p2.ind - p1.ind) + sqrd (p2.dep - p1.dep));
}


// returns float 2d vector from angle and radius
inline vect_2d vector_2d (double angle, double radius)
{
	return vect_2d (radius * cos (angle), radius * sin (angle));
}


// returns double 2d vector from angle and radius
inline vect_2dd vector_2dd (double angle, double radius)
{
	return vect_2dd (radius * cos (angle), radius * sin (angle));
}


#endif // !INC_MATH_2D
