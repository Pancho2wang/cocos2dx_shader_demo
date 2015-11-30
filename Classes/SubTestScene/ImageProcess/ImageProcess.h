/*!
 * \file:	ImageProcess.h
 * \author:	FreeKnight
 * \date:	2014-7-1
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "TestScene.h"
USING_NS_CC;
//--------------------------------------------------------
#define tcOffsetColumns 5
#define tcOffsetRows	5
//--------------------------------------------------------
class CImageProcessSprite : public CCSprite
{ 
public:  
	CImageProcessSprite();  
	virtual ~CImageProcessSprite();  
public:
	static CImageProcessSprite* create(const char* pszFileName, 
		const GLchar* pszVSH, const GLchar* pszFSH );  
public:
	virtual bool initWithFile(const char *pszFilename,
		const GLchar* pszVSH, const GLchar* pszFSH );
	virtual bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect,
		const GLchar* pszVSH, const GLchar* pszFSH );  
	virtual void draw();  
private:
	void		genTexCoordOffsets(GLuint width, GLuint height, GLfloat* pOutTexCoordOff, GLfloat step = 1.0f);
}; 
//--------------------------------------------------------
class CImageProcessSpriteScene : public CTestScene
{
public:
	CImageProcessSpriteScene();
private:
	bool	CreateSprite( const GLchar* szFSH, std::string szInfo, CCPoint tagPos );
public:
	void Run();
};
//------------------------------------------------------
