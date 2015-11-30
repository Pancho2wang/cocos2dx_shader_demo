/*!
 * \file:	ScratchOff.h
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
class CScratchOffLayer : public CCLayer 
{
public:
	virtual bool init();
	virtual void update(float delta);

	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
public:
	CREATE_FUNC( CScratchOffLayer );
private:
	bool		m_bReset;
	CCSprite*	m_pScratchFront;
	CCSprite*	m_pBurshSprite;
};
//--------------------------------------------------------
class CScratchOffScene : public CTestScene
{
public:
	CScratchOffScene();
public:
	void Run();
};
//--------------------------------------------------------

