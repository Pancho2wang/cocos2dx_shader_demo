/*!
 * \file:	AWTextureFilter.h
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "Texture2DMutable.h"
//--------------------------------------------------------
class AWTextureFilter 
{
/**
	@param input: Original texture data
	@param output: Empty (or not) buffer
	@param format: Pixel format of the data
	@param width: Real width (is a power of two)
	@param height: Real height (is a power of two)
	@param position: Top left vertex of the blur effect
	@param size: The size of the blur effect
	@param contentSize:
	@param radios: It's the radius of the blur effect
*/
public:
	AWTextureFilter() {};
	~AWTextureFilter() {};
public:
	void blurInput(void* input, void* output, CCTexture2DPixelFormat format, int width, int height, CCSize contentSize, int radius, CCRect rect);
	CCTexture2DMutable* blur(CCTexture2DMutable* texture, int radius);
	CCTexture2DMutable* blur(CCTexture2DMutable* texture, int radius, CCRect rect);
};
