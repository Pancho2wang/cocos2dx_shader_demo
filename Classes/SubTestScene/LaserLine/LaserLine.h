/*!
 * \file:	LaserLine.h
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "TestScene.h"
USING_NS_CC;
//--------------------------------------------------------
class CLaserLineLayer : public CCLayer 
{
public:
	virtual bool init();

	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
public:
	void RemoveSprite( CCNode* p );
public:
	CREATE_FUNC( CLaserLineLayer );
};
//--------------------------------------------------------
class CLaserLineScene : public CTestScene
{
public:
	CLaserLineScene();
public:
	void Run();
};
//-------------------------------------------------------
