/****************************************************************************
 *
 * 			color.h: Color type and operators api
 *      This is part of the yafray package
 *      Copyright (C) 2002  Alejandro Conty Estévez
 *      Copyright (C) 2015  David Bluecame for Color Space additions
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2.1 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library; if not, write to the Free Software
 *      Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#ifndef Y_COLOR_H
#define Y_COLOR_H

#include<yafray_config.h>

#include <iostream>
#include <utilities/mathOptimizations.h>

#define COLOR_SIZE 3

// ensure isnan and isinf are available. I *hope* it works with OSX w. gcc 4.x too
#ifdef _MSC_VER
#include <float.h>
#define isnan _isnan
#define isinf _isinf
#else
using std::isnan; // from cmath
using std::isinf; // from cmath
#endif

__BEGIN_YAFRAY

enum colorSpaces_t
{
	RAW_MANUAL_GAMMA	= 1,
	LINEAR_RGB		= 2,
	SRGB			= 3,
	XYZ_D65			= 4	
};

class YAFRAYCORE_EXPORT color_t
{
	friend color_t operator * (const color_t &a, const color_t &b);
	friend color_t operator * (const CFLOAT f, const color_t &b);
	friend color_t operator * (const color_t &b, const CFLOAT f);
	friend color_t operator / (const color_t &b, const CFLOAT f);
	friend color_t operator + (const color_t &a, const color_t &b);
	friend color_t operator - (const color_t &a, const color_t &b);
	friend CFLOAT maxAbsDiff(const color_t &a, const color_t &b);
	friend YAFRAYCORE_EXPORT void operator >> (unsigned char *data, color_t &c);
	friend YAFRAYCORE_EXPORT void operator << (unsigned char *data, const color_t &c);
	friend YAFRAYCORE_EXPORT void operator >> (float *data, color_t &c);
	friend YAFRAYCORE_EXPORT void operator << (float *data, const color_t &c);
	friend YAFRAYCORE_EXPORT std::ostream & operator << (std::ostream & out, const color_t c);
	friend YAFRAYCORE_EXPORT color_t mix(const color_t &a, const color_t &b, CFLOAT point);
	friend YAFRAYCORE_EXPORT color_t convergenceAccell(const color_t &cn_1, const color_t &cn0, const color_t &cn1);
	public:
		color_t() { R=G=B=0; }
		color_t(CFLOAT r, CFLOAT g, CFLOAT b) {R=r;G=g;B=b;};
		color_t(CFLOAT g) { R=G=B=g; }
		color_t(CFLOAT af[3]) { R=af[0];  G=af[1];  B=af[2]; }
		bool isBlack() const { return ((R==0) && (G==0) && (B==0)); }
		bool isNaN() const { return (isnan(R) || isnan(G) || isnan(B)); }
		bool isInf() const { return (isinf(R) || isinf(G) || isinf(B)); }
		~color_t() {}
		void set(CFLOAT r, CFLOAT g, CFLOAT b) { R=r;  G=g;  B=b; }

		color_t & operator +=(const color_t &c);
		color_t & operator -=(const color_t &c);
		color_t & operator *=(const color_t &c);
		color_t & operator *=(CFLOAT f);

		CFLOAT energy() const {return (R+G+B)*0.333333f;};
		// Using ITU/Photometric values Y = 0.2126 R + 0.7152 G + 0.0722 B
		CFLOAT col2bri() const { return (0.2126f*R + 0.7152f*G + 0.0722f*B); }
		CFLOAT abscol2bri() const { return (0.2126f*std::fabs(R) + 0.7152f*std::fabs(G) + 0.0722f*std::fabs(B)); }
		void gammaAdjust(CFLOAT g){ R = fPow(R, g); G = fPow(G, g); B = fPow(B, g); }
		void expgam_Adjust (CFLOAT e, CFLOAT g, bool clamp_rgb);
		CFLOAT getR() const { return R; }
		CFLOAT getG() const { return G; }
		CFLOAT getB() const { return B; }

		// used in blendershader
		void invertRGB()
		{
			if (R!=0.f) R=1.f/R;
			if (G!=0.f) G=1.f/G;
			if (B!=0.f) B=1.f/B;
		}
		void absRGB() { R=std::fabs(R);  G=std::fabs(G);  B=std::fabs(B); }
		void darkenRGB(const color_t &col)
		{
			if (R>col.R) R=col.R;
			if (G>col.G) G=col.G;
			if (B>col.B) B=col.B;
		}
		void lightenRGB(const color_t &col)
		{
			if (R<col.R) R=col.R;
			if (G<col.G) G=col.G;
			if (B<col.B) B=col.B;
		}

		void black() { R=G=B=0; }
		CFLOAT minimum() const { return std::min(R, std::min(G, B)); }
		CFLOAT maximum() const { return std::max(R, std::max(G, B)); }
		CFLOAT absmax() const { return std::max(std::fabs(R), std::max(std::fabs(G), std::fabs(B))); }
		void clampRGB0()
		{
			if (R<0.0) R=0.0;
			if (G<0.0) G=0.0;
			if (B<0.0) B=0.0;
		}

		void clampRGB01()
		{
			if (R<0.0) R=0.0; else if (R>1.0) R=1.0;
			if (G<0.0) G=0.0; else if (G>1.0) G=1.0;
			if (B<0.0) B=0.0; else if (B>1.0) B=1.0;
		}
		
		void clampProportionalRGB(float maxValue);
		
		CFLOAT linearRGB_from_sRGB(CFLOAT value_sRGB);
		CFLOAT sRGB_from_linearRGB(CFLOAT value_linearRGB);
		
		void linearRGB_from_ColorSpace(colorSpaces_t colorSpace, float gamma);
		void ColorSpace_from_linearRGB(colorSpaces_t colorSpace, float gamma);		
		
//	protected:
		CFLOAT R, G, B;
};

class YAFRAYCORE_EXPORT colorA_t : public color_t
{
	friend colorA_t operator * (const colorA_t &a, const colorA_t &b);
	friend colorA_t operator * (const CFLOAT f, const colorA_t &b);
	friend colorA_t operator * (const colorA_t &b, const CFLOAT f);
	friend colorA_t operator / (const colorA_t &b, const CFLOAT f);
	friend colorA_t operator + (const colorA_t &a, const colorA_t &b);
	friend colorA_t operator - (const colorA_t &a, const colorA_t &b);
	friend YAFRAYCORE_EXPORT void operator >> (unsigned char *data, colorA_t &c);
	friend YAFRAYCORE_EXPORT void operator << (unsigned char *data, const colorA_t &c);
	friend YAFRAYCORE_EXPORT void operator >> (float *data, colorA_t &c);
	friend YAFRAYCORE_EXPORT void operator << (float *data, const colorA_t &c);
	friend YAFRAYCORE_EXPORT std::ostream & operator << (std::ostream & out, const colorA_t c);
	friend YAFRAYCORE_EXPORT colorA_t mix(const colorA_t &a, const colorA_t &b, CFLOAT point);
	public:
		colorA_t() { /* A=0; */ }
		colorA_t(const color_t &c):color_t(c), A(1.f) { /* A=0; */ }
		colorA_t(const color_t &c, CFLOAT a):color_t(c), A(a) {}
		colorA_t(CFLOAT r, CFLOAT g, CFLOAT b, CFLOAT a=0):color_t(r,g,b), A(a) {}
		colorA_t(CFLOAT g):color_t(g) { A=g; }
		colorA_t(CFLOAT af[4]):color_t(af) { A=af[3]; }
		~colorA_t() {};
		void set(CFLOAT r, CFLOAT g, CFLOAT b, CFLOAT a=0) { color_t::set(r,g,b);  A=a; }

		colorA_t & operator +=(const colorA_t &c);
		colorA_t & operator -=(const colorA_t &c);
		colorA_t & operator *=(const colorA_t &c);
		colorA_t & operator *=(CFLOAT f);

		void alphaPremultiply() { R*=A; G*=A; B*=A; }
		CFLOAT getA() const { return A; }
		void setAlpha(CFLOAT a) { A=a; }

		void clampRGBA0()
		{
			clampRGB0();
			if (A<0.0) A=0.0;
		}

		void clampRGBA01()
		{
			clampRGB01();
			if (A<0.0) A=0.0; else if (A>1.0) A=1.0;
		}

		CFLOAT colorDifference(colorA_t color2, bool useRGBcomponents = false);

//	protected:
		CFLOAT A;
};

class YAFRAYCORE_EXPORT rgbe_t
{
	public:
		rgbe_t() {rgbe[3]=0;};
		rgbe_t(const color_t &s);
		operator color_t ()const
		{
			color_t res;
			CFLOAT f;
			if (rgbe[3])
			{   /*nonzero pixel*/
				f = fLdexp(1.0,rgbe[3]-(int)(128+8));
				return color_t(rgbe[0] * f,rgbe[1] * f,rgbe[2] * f);
			}
			else return color_t(0,0,0);
		}
//		unsigned char& operator [] (int i){ return rgbe[i]; }

//	protected:
		unsigned char rgbe[4];
};

inline void color_t::expgam_Adjust(CFLOAT e, CFLOAT g, bool clamp_rgb)
{
	if ((e==0.f) && (g==1.f)) {
		if (clamp_rgb) clampRGB01();
		return;
	}
	if (e!=0.f) {
		// exposure adjust
		clampRGB0();
		R = 1.f - fExp(R*e);
		G = 1.f - fExp(G*e);
		B = 1.f - fExp(B*e);
	}
	if (g!=1.f) {
		// gamma adjust
		clampRGB0();
		R = fPow(R, g);
		G = fPow(G, g);
		B = fPow(B, g);
	}
}

YAFRAYCORE_EXPORT void operator >> (unsigned char *data,color_t &c);
YAFRAYCORE_EXPORT void operator << (unsigned char *data,const color_t &c);
YAFRAYCORE_EXPORT void operator >> (float *data, color_t &c);
YAFRAYCORE_EXPORT void operator << (float *data, const color_t &c);
YAFRAYCORE_EXPORT std::ostream & operator << (std::ostream & out,const color_t c);
YAFRAYCORE_EXPORT color_t mix(const color_t &a,const color_t &b,CFLOAT point);

YAFRAYCORE_EXPORT void operator >> (unsigned char *data,colorA_t &c);
YAFRAYCORE_EXPORT void operator << (unsigned char *data,const colorA_t &c);
YAFRAYCORE_EXPORT void operator >> (float *data, colorA_t &c);
YAFRAYCORE_EXPORT void operator << (float *data, const colorA_t &c);
YAFRAYCORE_EXPORT std::ostream & operator << (std::ostream & out,const colorA_t c);
YAFRAYCORE_EXPORT colorA_t mix(const colorA_t &a,const colorA_t &b,CFLOAT point);


inline color_t operator * (const color_t &a,const color_t &b)
{
	return color_t(a.R*b.R,a.G*b.G,a.B*b.B);
}

inline color_t operator * (const CFLOAT f,const color_t &b)
{
	return color_t(f*b.R,f*b.G,f*b.B);
}

inline color_t operator * (const color_t &b,const CFLOAT f)
{
	return color_t(f*b.R,f*b.G,f*b.B);
}

inline color_t operator / (const color_t &b,CFLOAT f)
{
	return color_t(b.R/f,b.G/f,b.B/f);
}

inline color_t operator + (const color_t &a,const color_t &b)
{
	return color_t(a.R+b.R,a.G+b.G,a.B+b.B);
}

inline color_t operator - (const color_t &a, const color_t &b)
{
	return color_t(a.R-b.R, a.G-b.G, a.B-b.B);
}

/*
inline color_t & color_t::operator *=(const color_t &c)
{
	FLUSH_3DNOW();
	MMX_LOAD64(MM0,c.R);
	MMX_LOAD32(MM1,c.B);
	MMX_LOAD64(MM2,R);
	MMX_LOAD32(MM3,B);
	MMX_MULF(MM0,MM2);
	MMX_MULF(MM1,MM3);
	MMX_STORE64(R,MM0);
	MMX_STORE32(B,MM1);
	FLUSH_3DNOW();
	return *this;
}*/

inline color_t & color_t::operator +=(const color_t &c)
{ R += c.R;  G += c.G;  B += c.B;  return *this; }
inline color_t & color_t::operator *=(const color_t &c)
{ R *= c.R;  G *= c.G;  B *= c.B;  return *this; }
inline color_t & color_t::operator *=(CFLOAT f)
{ R *= f;  G*= f;  B *= f;  return *this; }
inline color_t & color_t::operator -=(const color_t &c)
{ R -= c.R;  G -= c.G;  B -= c.B;  return *this; }

inline colorA_t operator * (const colorA_t &a,const colorA_t &b)
{
	return colorA_t(a.R*b.R, a.G*b.G, a.B*b.B, a.A*b.A);
}

inline colorA_t operator * (const CFLOAT f,const colorA_t &b)
{
	return colorA_t(f*b.R, f*b.G, f*b.B, f*b.A);
}

inline colorA_t operator * (const colorA_t &b,const CFLOAT f)
{
	return colorA_t(f*b.R, f*b.G, f*b.B, f*b.A);
}

inline colorA_t operator / (const colorA_t &b,CFLOAT f)
{
	if (f!=0) f=1.0/f;
	return colorA_t(b.R*f, b.G*f, b.B*f, b.A*f);
}

inline colorA_t operator + (const colorA_t &a,const colorA_t &b)
{
	return colorA_t(a.R+b.R, a.G+b.G, a.B+b.B, a.A+b.A);
}

inline colorA_t operator - (const colorA_t &a, const colorA_t &b)
{
	return colorA_t(a.R-b.R, a.G-b.G, a.B-b.B, a.A-b.A);
}

inline colorA_t & colorA_t::operator +=(const colorA_t &c) { R += c.R;  G += c.G;  B += c.B;  A += c.A;  return *this; }
inline colorA_t & colorA_t::operator *=(const colorA_t &c) { R *= c.R;  G *= c.G;  B *= c.B;  A *= c.A;  return *this; }
inline colorA_t & colorA_t::operator *=(CFLOAT f) { R *= f;  G*= f;  B *= f;  A *= f;  return *this; }
inline colorA_t & colorA_t::operator -=(const colorA_t &c) { R -= c.R;  G -= c.G;  B -= c.B;  A -= c.A;  return *this; }

inline CFLOAT maxAbsDiff(const color_t &a,const color_t &b)
{
	return (a - b).absmax();
}

YAFRAYCORE_EXPORT color_t convergenceAccell(const color_t &cn_1,const color_t &cn0,const color_t &cn1);

//Matrix information from: http://www.color.org/chardata/rgb/sRGB.pdf
static float linearRGB_from_XYZ_D65[3][3] =
{
	 { 3.2406255, -1.537208,  -0.4986286 },
	 {-0.9689307,  1.8757561,  0.0415175 },
	 { 0.0557101, -0.2040211,  1.0569959 }
};

//Inverse matrices
static float XYZ_D65_from_linearRGB[3][3] =
{
	{ 0.412400,   0.357600,   0.180500 },
	{ 0.212600,   0.715200,   0.072200 },
	{ 0.019300,   0.119200,   0.950500 }
};

inline CFLOAT color_t::linearRGB_from_sRGB(CFLOAT value_sRGB)
{
	//Calculations from http://www.color.org/chardata/rgb/sRGB.pdf
	if(value_sRGB <= 0.04045f) return (value_sRGB / 12.92f);
	else return fPow(((value_sRGB + 0.055f) / 1.055f), 2.4f);
}

inline CFLOAT color_t::sRGB_from_linearRGB(CFLOAT value_linearRGB)
{
	//Calculations from http://www.color.org/chardata/rgb/sRGB.pdf
	if(value_linearRGB <= 0.0031308f) return (value_linearRGB * 12.92f);
	else return ((1.055f * fPow(value_linearRGB, 0.416667f)) - 0.055f); //0,416667f = 1/2.4
}

inline void color_t::linearRGB_from_ColorSpace(colorSpaces_t colorSpace, float gamma)
{
	//NOTE: Alpha value is not converted from linear to color space and vice versa. Should it be converted?
	if(colorSpace == SRGB)
	{
		R = linearRGB_from_sRGB(R);
		G = linearRGB_from_sRGB(G);
		B = linearRGB_from_sRGB(B);
	}
	else if(colorSpace == XYZ_D65)
	{
		float oldR = R, oldG = G, oldB = B;
		R = linearRGB_from_XYZ_D65[0][0] * oldR + linearRGB_from_XYZ_D65[0][1] * oldG + linearRGB_from_XYZ_D65[0][2] * oldB;
		G = linearRGB_from_XYZ_D65[1][0] * oldR + linearRGB_from_XYZ_D65[1][1] * oldG + linearRGB_from_XYZ_D65[1][2] * oldB;
		B = linearRGB_from_XYZ_D65[2][0] * oldR + linearRGB_from_XYZ_D65[2][1] * oldG + linearRGB_from_XYZ_D65[2][2] * oldB;
	}
	else if(colorSpace == RAW_MANUAL_GAMMA && gamma != 1.f)
	{
		gammaAdjust(gamma);
	}
}

inline void color_t::ColorSpace_from_linearRGB(colorSpaces_t colorSpace, float gamma)
{
	//NOTE: Alpha value is not converted from linear to color space and vice versa. Should it be converted?
	if(colorSpace == SRGB)
	{
		R = sRGB_from_linearRGB(R);
		G = sRGB_from_linearRGB(G);
		B = sRGB_from_linearRGB(B);
	}
	else if(colorSpace == XYZ_D65)
	{
		float oldR = R, oldG = G, oldB = B;
		R = XYZ_D65_from_linearRGB[0][0] * oldR + XYZ_D65_from_linearRGB[0][1] * oldG + XYZ_D65_from_linearRGB[0][2] * oldB;
		G = XYZ_D65_from_linearRGB[1][0] * oldR + XYZ_D65_from_linearRGB[1][1] * oldG + XYZ_D65_from_linearRGB[1][2] * oldB;
		B = XYZ_D65_from_linearRGB[2][0] * oldR + XYZ_D65_from_linearRGB[2][1] * oldG + XYZ_D65_from_linearRGB[2][2] * oldB;
	}
	else if(colorSpace == RAW_MANUAL_GAMMA && gamma != 1.f)
	{
		if(gamma <= 0.f) gamma = 1.0e-2f;	//Arbitrary lower boundary limit for the output gamma, to avoid division by 0
		float invGamma = 1.f / gamma;
		gammaAdjust(invGamma);
	}
}

inline void color_t::clampProportionalRGB(float maxValue)	//Function to clamp the current color to a maximum value, but keeping the relationship between the color components. So it will find the R,G,B component with the highest value, clamp it to the maxValue, and adjust proportionally the other two components 
{
	if(maxValue > 0.f)	//If maxValue is 0, no clamping is done at all.
	{
		//If we have to clamp the result, calculate the maximum RGB component, clamp it and scale the other components acordingly to preserve color information.
		
		float maxRGB = std::max(R, std::max(G, B));
		float proportionalAdjustment = maxValue / maxRGB;
		
		if(maxRGB > maxValue)
		{
			if(R >= maxRGB)
			{
				R = maxValue;
				G *= proportionalAdjustment;
				B *= proportionalAdjustment;
			}
			
			else if(G >= maxRGB)
			{
				G = maxValue;
				R *= proportionalAdjustment;
				B *= proportionalAdjustment;
			}

			else
			{
				B = maxValue;
				R *= proportionalAdjustment;
				G *= proportionalAdjustment;
			}
		}
	}
}

inline CFLOAT colorA_t::colorDifference(colorA_t color2, bool useRGBcomponents)
{	
	float colorDifference = std::fabs(color2.col2bri() - col2bri());

	if(useRGBcomponents)
	{
		float Rdiff = std::fabs(color2.R - R);
		float Gdiff = std::fabs(color2.G - G);
		float Bdiff = std::fabs(color2.B - B);
		float Adiff = std::fabs(color2.A - A);
		
		if(colorDifference < Rdiff) colorDifference = Rdiff; 
		if(colorDifference < Gdiff) colorDifference = Gdiff; 
		if(colorDifference < Bdiff) colorDifference = Bdiff; 
		if(colorDifference < Adiff) colorDifference = Adiff; 
	}
	
	return colorDifference;
}

__END_YAFRAY

#endif // Y_COLOR_H
