/*!
 * \file:	Texture2DMutable.h
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//--------------------------------------------------------
// 是否保存原数据
#define CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA 1
//--------------------------------------------------------
class CCTexture2DMutable : public cocos2d::CCTexture2D
{
private:
#if CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
		CC_PROPERTY_READONLY(void*, originalData_, OriginalTexData);
#endif
		CC_PROPERTY(void*, data_, TexData);
		unsigned int bytesPerPixel_;
		bool dirty_;
public:
	CCTexture2DMutable();
	~CCTexture2DMutable(void);

	void restore();

	// reread data from texture
	void updateData();
	// update data from fbo
	void updateData(unsigned int fbo, unsigned int oldfbo);
	/** These functions are needed to create mutable textures */
	void releaseData(void *data);
	void* keepData(void *data, unsigned int length);

	/** Intializes with a texture2d with data */
	bool initWithData(const void* data, cocos2d::CCTexture2DPixelFormat pixelFormat, 
		unsigned int pixelsWide, unsigned int pixelsHigh, const cocos2d::CCSize& contentSize);

	/** Initializes a texture from a string with font name and font size */
	bool initWithString(const char *text, const char *fontName, float fontSize);
	bool initWithImage(CCImage *uiImage);
	bool initPremultipliedATextureWithImage(CCImage *image, unsigned int width, unsigned int height);
	cocos2d::ccColor4B pixelAt(const cocos2d::CCPoint& pt);

	/* retrieve/fill with RGBA8888 data */
	unsigned int pixelAt(int x, int y);
	bool setPixelAt(int x, int y, unsigned int c);

	unsigned int* getDataRGBA();
	void putDataRGBA();

	void fillRect(const cocos2d::CCRect& rect, unsigned int c);
	void fillRect(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& c);

	//	@param pt is a point to get a pixel (0,0) is top-left to (width,height) bottom-right
	//	@param c is a ccColor4B which is a colour.
	//	@returns if a pixel was set
	//	Remember to call apply to actually update the texture canvas.
	bool setPixelAt(const cocos2d::CCPoint& pt, const cocos2d::ccColor4B& c);

	//	Fill with specified colour
	void fill(const cocos2d::ccColor4B& c);
	//	@param textureToCopy is the texture image to copy over
	//	@param offset also offset's the texture image
	CCTexture2D* copyMutable(bool isMutable);
	CCTexture2DMutable* copy();
	void copy(CCTexture2DMutable* textureToCopy, const cocos2d::CCPoint& offset);
	//	apply actually updates the texture with any new data we added.
	void apply();
};
//--------------------------------------------------------