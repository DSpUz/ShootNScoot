#pragma once

#include "Colors.h"
#include "Graphics.h"

namespace SpriteEffect
{
	class Chroma
	{
	public:
		Chroma( Color c )
			:
			chroma( c )
		{}
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			if( cSrc != chroma )
			{
				gfx.PutPixel( xDest,yDest,cSrc );
			}
		}
	private:
		Color chroma;
	};
	class Substitution
	{
	public:
		Substitution( Color c,Color s )
			:
			chroma( c ),
			sub( s )
		{}
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			if( cSrc != chroma )
			{
				gfx.PutPixel( xDest,yDest,sub );
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};
	class Copy
	{
	public:
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			gfx.PutPixel( xDest,yDest,cSrc );
		}
	};
	class Ghost
	{
	public:
		Ghost( Color c )
			:
			chroma( c )
		{}
		void operator()( Color src,int xDest,int yDest,Graphics& gfx ) const
		{
			if( src != chroma )
			{
				const Color dest = gfx.GetPixel( xDest,yDest );
				const Color blend = {
					unsigned char( (src.GetR() + dest.GetR()) / 2 ),
					unsigned char( (src.GetG() + dest.GetG()) / 2 ),
					unsigned char( (src.GetB() + dest.GetB()) / 2 )
				};
				gfx.PutPixel( xDest,yDest,blend );
			}
		}
	private:
		Color chroma;
	};
	class Alpha
	{
	public:
		void operator()( Color src1, int xDest, int yDest, Graphics& gfx ) const
		{
			const auto srcAlpha1 = src1.GetA();
			const auto src2 = gfx.GetPixel( xDest, yDest );
			if( srcAlpha1 == 255 )
			{
				gfx.PutPixel( xDest, yDest, src1 );
			}
			else if( srcAlpha1 > 0 && srcAlpha1 < 255)
			{
				const auto srcAlpha2 = 255 - srcAlpha1;

				const auto dest = Color{
					uint8_t( 255 ),
					uint8_t( ( ( src1.GetR() * srcAlpha1 ) + ( src2.GetR() * srcAlpha2 ) ) >> 8 ),
					uint8_t( ( ( src1.GetG() * srcAlpha1 ) + ( src2.GetG() * srcAlpha2 ) ) >> 8 ),
					uint8_t( ( ( src1.GetB() * srcAlpha1 ) + ( src2.GetB() * srcAlpha2 ) ) >> 8 )
				};

				gfx.PutPixel( xDest, yDest, dest );
			}
		}
	};
}