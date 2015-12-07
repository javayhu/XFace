// ========================================================================
// (C) Copyright 2004,2005 Sony Corporation, All Rights Reserved
// @(#) $Id: Complex.h,v 2.0 2005/11/08 00:50:11 kuni Exp $ 
//
// NAME
//  Complex.h 
//
// DESCRIPTION
//   
//
// HISTORY
//
// LOG
//   $Log: Complex.h,v $
//   Revision 2.0  2005/11/08 00:50:11  kuni
//   no message
//
//   Revision 1.1.1.1  2005/11/08 00:48:22  kuni
//   Initial Revision
//
//   Revision 1.3  2005/05/27 08:48:08  takano
//   ヘッダーにヘッダーをつけた。
//
// ========================================================================

/* Complex.h -Complex function declarations */
#include<math.h>

struct Complex {double i,q;};
typedef struct Complex Complex;

inline Complex cmplx(double,double);
inline Complex xRot(double b);
inline Complex xZero();
inline Complex conjg(Complex);
inline Complex cadd(Complex,Complex);
inline Complex csub(Complex,Complex);
inline Complex cmul(Complex,Complex);
inline Complex rmul(double,Complex);
inline Complex cdiv(Complex,Complex);
inline Complex rdiv(Complex,double);
inline double real(Complex);
inline double aimag(Complex);
inline double complexabs(Complex);
inline double complexpower(Complex);
inline Complex cexp(Complex);
inline Complex csqrt(Complex z);
inline Complex cinit2D(Complex, int, int, double);

//******************************************************************************//

inline Complex cmplx(double i,double q) // define Complex number Z=i+jq 
{
	Complex z;
	z.i=i;
	z.q=q;
	return(z);
}

#define PI_BY_TWO   6.283185307178	// 2.0 * PI
inline Complex xRot(double b)
{
	Complex c;
	b = b * PI_BY_TWO;
	c.i = cos(b);
	c.q = sin(b);
	return c;
}

inline Complex xZero()
{
  Complex a;
  a.i = 0.0;
  a.q = 0.0;
  return a;
}

inline Complex conjg(Complex z) // Complex conjugate
{
	return(cmplx(z.i,-z.q));
}

inline Complex cadd(Complex a,Complex b) // Complex addition
{
	return(cmplx(a.i+b.i,a.q+b.q));
}

inline Complex csub(Complex a,Complex b) // Complex substraction
{
	return(cmplx(a.i-b.i,a.q-b.q));  
}

inline Complex cmul(Complex a,Complex b) // Complex multiplication
{
	return(cmplx(a.i*b.i-a.q*b.q,a.i*b.q+a.q*b.i));
}

inline Complex rmul(double a,Complex z) // multiplication by real
{
	return cmplx(a*z.i,a*z.q);
}

inline Complex cdiv(Complex a,Complex b) // Complex division
{
	double D=b.i*b.i+b.q*b.q;
	return(cmplx((a.i*b.i+a.q*b.q)/D,(a.q*b.i-a.i*b.q)/D));
}

inline Complex rdiv(Complex z,double a) // division by real
{
	return(cmplx(z.i/a,z.q/a));
}

inline double real(Complex z) // real part
{
	return(z.i);
}

inline double aimag(Complex z) // imaginary part
{
	return(z.q);
}

inline double complexabs(Complex z) // Complex magnitude
{
	return (sqrt(z.i*z.i+z.q*z.q));
}

inline double complexpower(Complex z)
{
	return(z.i*z.i+z.q*z.q);
}

inline Complex cexp(Complex z) // Complex exponential
{
	double R=exp(z.i);
	return(cmplx(R*cos(z.q),R*sin(z.q)));
}

inline Complex csqrt(Complex z) // Complex Root z.iが0の場合は注意
{
	double  theta, r;
	Complex y;
	theta = atan(z.q/z.i);
	r = sqrt(z.i * z.i + z.q * z.q);
	y.i = sqrt(r) * cos(theta/2.0);
	y.q = sqrt(r) * sin(theta/2.0);
	return(y);
}


// Complex型2F配列を任意の値に初期化
inline Complex cinit2D(
			Complex **ppxData,
			int i1st,
			int i2nd,
			double dSetValue
)
{
	int iIdx0;
	int iIdx1;

	for (iIdx0 = 0; iIdx0 < i1st; iIdx0++) {
		for (iIdx1 = 0; iIdx1 < i2nd; iIdx1++) {
			ppxData[iIdx0][iIdx1].i = dSetValue;
			ppxData[iIdx0][iIdx1].q = dSetValue;
		}
	}
	return(**ppxData);
}
