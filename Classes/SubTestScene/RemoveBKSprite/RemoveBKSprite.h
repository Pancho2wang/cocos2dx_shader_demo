/*!
 * \file:	RemoveBKSprite.h
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
class CRemoveBKSprite : public CCSprite {

public:
	static CRemoveBKSprite* Create(const char* pszFileName);
public:
	virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect);
	virtual void draw(void);
};
//--------------------------------------------------------
class CRemoveBKSpriteScene : public CTestScene
{
public:
	CRemoveBKSpriteScene();
public:
	void Run();
};
//--------------------------------------------------------