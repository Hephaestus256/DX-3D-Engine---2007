// ================================================================================
// 1D Math definitions
// Nathan G. Smith
// ================================================================================

#ifndef INC_MATH_1D
#define INC_MATH_1D

#include <math.h>
#include "my_data.h"

// ********************** 1d constant definitions ****************************

// non-real constants
#define INF direct_double (0x7ff00000, 0x00000000) // double +infinity
#define Q_NAN direct_double (0x00000000, 0x7ff7ffff) // double quiet NaN
#define S_NAN direct_double (0x00000000, 0x7fffffff) // double signaling NaN
#define F_INF direct_float (0x7f800000) // float +infinity

// real constants
#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844
#define PI_2 PI / 2.0
#define PI_4 PI / 4.0
#define PI_6 PI / 6.0
#define FL_CMP_PREC .001f // floating point compare precision
#define DBL_CMP_PREC .000001 // double compare precision
#define DBL_CMP_PREC_LOW double(FL_CMP_PREC) // double low precision compare

// represents error in calculation
#define RESULT_UNDEF -S_NAN // double result undefined
#define F_RESULT_UNDEF -F_INF // float result undefined

// ******************** 1d comparison definitions ************************

// tests if double is +/- infinity
inline bool infinite (double x)
{
	return (x == INF) || (x == -INF);
}


// tests if double is quiet NaN or signaling NaN
inline bool nan (double x)
{
	return (x == Q_NAN) || (x == S_NAN);
}


// tests if double is not +/- infinity or any form of NaN 
inline bool real (double x)
{
	return !infinite (x) && !nan (x);
}


// tests if float x is exactly between min and max inclusively
inline bool between_inc (float x, float min, float max)
{
	return (x >= min) && (x <= max);
}


// tests if double x is exactly between min and max inclusively
inline bool between_inc (double x, double min, double max)
{
	return (x >= min) && (x <= max);
}


// tests if float x is exactly between min and max exclusively
inline bool between_exc (float x, float min, float max)
{
	return (x > min) && (x < max);
}


// tests if double x is exactly between min and max exclusively
inline bool between_exc (double x, double min, double max)
{
	return (x > min) && (x < max);
}


// tests if float x is approx zero
inline bool approx_zero (float x) 
{
	return between_exc (x, -FL_CMP_PREC, FL_CMP_PREC);	
}


// tests if double x is approx zero
inline bool approx_zero (double x) 
{
	return between_exc (x, -DBL_CMP_PREC, DBL_CMP_PREC);
}


// tests with low precision if double n is approx zero
inline bool approx_zero_low (double x) 
{
	return between_exc (x, -DBL_CMP_PREC_LOW, DBL_CMP_PREC_LOW);
}


// tests approximate equality of 2 floats
inline bool approx_equal (float x1, float x2) 
{
	return between_exc (x1, x2 - FL_CMP_PREC, x2 + FL_CMP_PREC);
}


// tests approximate equality of 2 doubles
inline bool approx_equal (double x1, double x2) 
{
	return between_exc (x1, x2 - DBL_CMP_PREC, x2 + DBL_CMP_PREC);
}


// tests low precision approximate equality of 2 doubles
inline bool approx_equal_low (double x1, double x2) 
{
	return between_exc (x1, x2 - DBL_CMP_PREC_LOW, x2 + DBL_CMP_PREC_LOW);
}


// tests approx greater than or equal to for 2 floats
inline bool approx_greater_equ (float x1, float x2) 
{
	return x1 > (x2 - FL_CMP_PREC);
}


// tests approx greater than or equal to for 2 doubles
inline bool approx_greater_equ (double x1, double x2) 
{
	return x1 > (x2 - DBL_CMP_PREC);
}


// tests approx less than or equal to for 2 floats
inline bool approx_lesser_equ (float x1, float x2) 
{
	return x1 < (x2 + FL_CMP_PREC);
}


// tests approx less than or equal to for 2 doubles
inline bool approx_lesser_equ (double x1, double x2) 
{
	return x1 < (x2 + DBL_CMP_PREC);
}


// tests approx greater than for 2 floats
inline bool approx_greater (float x1, float x2) 
{
	return x1 >= (x2 + FL_CMP_PREC);	
}


// tests approx greater than for 2 doubles
inline bool approx_greater (double x1, double x2) 
{
	return x1 >= (x2 + DBL_CMP_PREC);	
}


// tests approx less than for 2 floats
inline bool approx_lesser (float x1, float x2) 
{
	return x1 <= (x2 - FL_CMP_PREC);	
}


// tests approx less than for 2 doubles
inline bool approx_lesser (double x1, double x2) 
{
	return x1 <= (x2 - DBL_CMP_PREC);	
}


// tests if double x is approx between doubles min and b inclusively
inline bool approx_between_inc (double x, double min, double max) 
{
	return approx_greater_equ (x, min) && approx_lesser_equ (x, max);
}


// tests if double x is approx between doubles min and max exclusively
inline bool approx_between_exc (double x, double min, double max) 
{
	return approx_greater (x, min) && approx_lesser (x, max);
}


// tests if float x is approx between floats min and max inclusively
inline bool approx_between_inc (float x, float min, float max) 
{
	return approx_greater_equ (x, min) && approx_lesser_equ (x, max);
}


// tests if float x is approx between floats min and max exclusively
inline bool approx_between_exc (float x, float min, float max) 
{
	return approx_greater (x, min) && approx_lesser (x, max);
}


// tests if float x is approx between double min inclusively and max exclusively
inline bool approx_between_inc_exc (double x, double min, double max) 
{
	return approx_greater_equ (x, min) && approx_lesser (x, max);
}


// tests if float x is approx between double min exclusively and max inclusively
inline bool approx_between_exc_inc (double x, double min, double max) 
{
	return approx_greater (x, min) && approx_lesser_equ (x, max);
}


// trinary compare of 2 ints
inline int trin_cmp (int x1, int x2)
{
	if (x1 == x2)
		return 0;
	else if (x1 < x2)
		return -1;
	else
		return 1;
}

// trinary compare of 2 floats
// returns:
//     approx less than: -1
//     approx greater than: 1
//     approx equal: 0
inline int trin_cmp (float x1, float x2)
{
	if (approx_equal (x1, x2))
		return 0;
	else if (x1 < x2)
		return -1;
	else
		return 1;
}


// trinary compare of 2 doubles
inline int trin_cmp (double x1, double x2)
{
	if (approx_equal (x1, x2))
		return 0;
	else if (x1 < x2)
		return -1;
	else
		return 1;
}

// *************************** truncation definitions **********************************

// returns fractional part of double x
inline double frac (double x)
{
	return x - floor(x);
}


// rounds double x to nearest value double p
inline double round (double x, double p)
{
	return p * floor((x + .5 * p) / p);
}


inline float clip_min_valu (float x, float min)
{
	if (x < min)
		return min;
	else
		return x;
}


inline double clip_min_valu (double x, double min)
{
	if (x < min)
		return min;
	else
		return x;
}


inline float clip_max_valu (float x, float max)
{
	if (x > max)
		return max;
	else
		return x;
}

inline double clip_max_valu (double x, double max)
{
	if (x > max)
		return max;
	else
		return x;
}


inline int clip_valu (int x, int min, int max)
{
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}


inline float clip_valu (float x, float min, float max)
{
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}


inline double clip_valu (double x, double min, double max)
{
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}


inline int wrap_valu (int x, int min, int max)
{
	return min + (((max - min) + (x - min)) % (max - min));
}


inline double wrap_valu (double x, double min, double max)
{
	return min + fmod ((max - min) + (x - min), (max - min));
}


inline double interpolate (double x_in, double min_in, double max_in, double min_ou, double max_ou)
{
	double range_in = max_in - min_in;
	double range_ou = max_ou - min_ou;
	double extent_in = x_in - min_in;
	double proportion = extent_in / range_in;

	return min_ou + proportion * range_ou;
}

// ************************* misc 1d definitions ******************************

// returns the square of a double
inline double sqrd (double x)
{
	return x * x;
}


inline int mini (int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}


inline float mini (float a, float b)
{
	if (a < b)
		return a;
	else
		return b;
}


inline double mini (double a, double b)
{
	if (a < b)
		return a;
	else
		return b;
}


double mini (double* n, int ct, size_t step)
{
	double min = *n;
	size_t offs = 0;

	for (int i = 0; i < ct; i++) {
		double d = *(double*)offset (n, offs);

		if (d < min)
			min = d;

		offs += step;
	}
		
	return min;
}


float mini (float* n, int ct, size_t step)
{
	float min = *n;
	size_t offs = 0;

	for (int i = 0; i < ct; i++) {
		float f = *(float*)offset (n, offs);

		if (f < min)
			min = f;

		offs += step;
	}
		
	return min;
}


inline int maxi (int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}


inline float maxi (float a, float b)
{
	if (a > b)
		return a;
	else
		return b;
}


inline double maxi (double a, double b)
{
	if (a > b)
		return a;
	else
		return b;
}

double maxi (double* n, int ct, size_t step)
{
	double max = *n;
	size_t offs = 0;

	for (int i = 0; i < ct; i++) {
		double d = *(double*)offset (n, offs);

		if (d > max)
			max = d;

		offs += step;
	}
		
	return max;
}


float maxi (float* n, int ct, size_t step)
{
	float max = *n;
	size_t offs = 0;

	for (int i = 0; i < ct; i++) {
		float f = *(float*)offset (n, offs);

		if (f < max)
			max = f;

		offs += step;
	}
		
	return max;
}


#endif // !INC_MATH_1D