/*!
 * \file:	ColorSprite.h
 * \author:	FreeKnight
 * \date:	2014-6-27
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "TestScene.h"
USING_NS_CC;
//--------------------------------------------------------
class CColorSprite : public CCSprite
{ 
public:  
	CColorSprite();  
	virtual ~CColorSprite();  
public:
	static CColorSprite* create(const char* pszFileName, 
		const GLchar* pszVSH, const GLchar* pszFSH );  
public:
	virtual bool initWithFile(const char *pszFilename,
		const GLchar* pszVSH, const GLchar* pszFSH );
	virtual bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect,
		const GLchar* pszVSH, const GLchar* pszFSH );  
	virtual void draw();  
}; 
//--------------------------------------------------------
class CColorSpriteScene : public CTestScene
{
public:
	CColorSpriteScene();
private:
	bool	CreateSprite( const GLchar* szFSH, std::string szInfo, CCPoint tagPos );
public:
	void Run();
};
//--------------------------------------------------------