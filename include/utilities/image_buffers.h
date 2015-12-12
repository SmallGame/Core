/****************************************************************************
 *      image_buffers.h: An image buffer handler
 *      This is part of the yafray package
 *      Copyright (C) 2010 Rodrigo Placencia Vazquez
 *      Copyright (C) 2015 David Bluecame for rgba8888, rgb888 and rgb565 buffers
 * 		for texture RAM optimization
 *		(pixel_t class wa originally written by someone else, don't know exactly who)
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
 */
 
#ifndef Y_IMAGE_BUFFERS_H
#define Y_IMAGE_BUFFERS_H

#include <yafray_config.h>
#include <core_api/color.h>
#include <vector>
#include <stdint.h>

__BEGIN_YAFRAY

/*! Color weighted pixel structure */
class pixel_t
{
	public:
	pixel_t() { col = colorA_t(0.f); weight = 0.f;}
	colorA_t normalized() const
	{
		if(weight > 0.f) return col / weight;
		else return colorA_t(0.f);
	}
	
	colorA_t col;
	float weight;
};

/*! Gray scale weighted pixel structure */
class pixelGray_t 
{
	public:
	pixelGray_t() { val = 0.f; weight = 0.f;}
	float normalized() const
	{
		if(weight > 0.f) return val / weight;
		else return 0.f;
	}
	float val;
	float weight;
};

class rgba8888_t
{
	public:
		rgba8888_t():r(0),g(0),b(0),a(1) {}
	
		void setColor(const colorA_t& col) { setR((uint8_t)floorf(col.R*255.f)); setG((uint8_t)floorf(col.G*255.f)); setB((uint8_t)floorf(col.B*255.f)); setA((uint8_t)floorf(col.A*255.f)); }
		
		void setR(uint8_t red8) { r = red8; }
		void setG(uint8_t green8) { g = green8; }
		void setB(uint8_t blue8) { b = blue8; }
		void setA(uint8_t alpha8) { a = alpha8; }
		
		uint8_t getR() const { return r; }
		uint8_t getG() const { return g; }
		uint8_t getB() const { return b; }
		uint8_t getA() const { return a; }

		colorA_t getColor() { return colorA_t((float) getR()/255.f, (float) getG()/255.f, (float) getB()/255.f, (float) getA()/255.f); }
	
	protected:
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
};

class rgb888_t
{
	public:
		rgb888_t():r(0),g(0),b(0) {}
	
		void setColor(const colorA_t& col) { setR((uint8_t)floorf(col.R*255.f)); setG((uint8_t)floorf(col.G*255.f)); setB((uint8_t)floorf(col.B*255.f)); }
		
		void setR(uint8_t red8) { r = red8; }
		void setG(uint8_t green8) { g = green8; }
		void setB(uint8_t blue8) { b = blue8; }
		void setA(uint8_t alpha8) {}
		
		uint8_t getR() const { return r; }
		uint8_t getG() const { return g; }
		uint8_t getB() const { return b; }
		uint8_t getA() const { return 255; }

		colorA_t getColor() { return colorA_t((float) getR()/255.f, (float) getG()/255.f, (float) getB()/255.f, 1.f); }
	
	protected:
		uint8_t r;
		uint8_t g;
		uint8_t b;
};

class rgb565_t
{
    //RGB565 lossy 16bit format: rrrr rggg gggb bbbb
    public:
		rgb565_t():rgb565(0) {}
		
		void setColor(const colorA_t& col) { setR((uint8_t)floorf(col.R*255.f)); setG((uint8_t)floorf(col.G*255.f)); setB((uint8_t)floorf(col.B*255.f)); }
		
		void setR(uint8_t red8) { rgb565 = (rgb565 & 0x07FF) | ((red8 & 0xF8) << 8); }
		void setG(uint8_t green8) { rgb565 = (rgb565 & 0xF81F) | ((green8 & 0xFC) << 3); }
		void setB(uint8_t blue8) { rgb565 = (rgb565 & 0xFFE0) | ((blue8 & 0xF8) >> 3); }
		void setA(uint8_t alpha8) {}
		
		uint8_t getR() const { return ((rgb565 & 0xF800) >> 8); }
		uint8_t getG() const { return ((rgb565 & 0x07E0) >> 3); }
		uint8_t getB() const { return ((rgb565 & 0x001F) << 3); }
		uint8_t getA() const { return 255; }

		colorA_t getColor() { return colorA_t((float) getR()/255.f, (float) getG()/255.f, (float) getB()/255.f, 1.f); }
    
    protected:
		uint16_t rgb565;
};

template <class T> class generic2DBuffer_t
{
public:
	generic2DBuffer_t() {  }
	
	generic2DBuffer_t(int w, int h) : width(w), height(h)
	{
		data.resize(width);
		for(int i = 0; i < width; i++) data[i].resize(height);
	}
	
	~generic2DBuffer_t()
	{
		if(data.size() > 0)
		{
			for(int i = 0; i < width; i++) data[i].clear();
			data.clear();
		}
	}
	
	inline void clear()
	{
		if(data.size() > 0)
		{
			for(int i = 0; i < width; i++) data[i].clear();
			data.clear();
		}
		data.resize(width);
		for(int i = 0; i < width; i++) data[i].resize(height);
	}

	inline T &operator()(int x, int y)
	{
		return data[x][y];
	}

	inline const T &operator()(int x, int y) const
	{
		return data[x][y];
	}
		
private:
	
	std::vector< std::vector< T > > data;
	int width;
	int height;
};

template <class T> class genericScanlineBuffer_t
{
public:
	genericScanlineBuffer_t() {  }
	
	genericScanlineBuffer_t(int w, int h) : width(w), height(h)
	{
		data.resize(width * height);
	}
	
	~genericScanlineBuffer_t()
	{
		data.clear();
	}
	
	inline void clear()
	{
		data.clear();
		data.resize(width * height);
	}

	inline T &operator()(int x, int y)
	{
		return data[x * height + y];
	}

	inline const T &operator()(int x, int y) const
	{
		return data[x * height + y];
	}
		
private:
	
	std::vector< T > data;
	int width;
	int height;
};

typedef generic2DBuffer_t<pixel_t> 		rgba2DImage_t; //!< Weighted RGBA image buffer typedef
typedef generic2DBuffer_t<pixelGray_t> 	gray2DImage_t; //!< Weighted monochromatic image buffer typedef
typedef generic2DBuffer_t<color_t> 		rgb2DImage_nw_t; //!< Non-weighted RGB (96bit/pixel) image buffer typedef
typedef generic2DBuffer_t<colorA_t> 	rgba2DImage_nw_t; //!< Non-weighted RGBA (128bit/pixel) image buffer typedef
typedef generic2DBuffer_t<float> 		gray2DImage_nw_t; //!< Non-weighted gray scale (32bit/gray pixel) image buffer typedef
typedef generic2DBuffer_t<rgb565_t>		rgb565Image_nw_t; //!< Non-weighted RGB565 (16bit/pixel) LOSSY image buffer typedef
typedef generic2DBuffer_t<rgb888_t>		rgb888Image_nw_t; //!< Non-weighted RGB888 (24bit/pixel) without alpha image buffer typedef
typedef generic2DBuffer_t<rgba8888_t>	rgba8888Image_nw_t; //!< Non-weighted RGB8888 (32bit/pixel) with alpha buffer typedef
__END_YAFRAY

#endif
