/*!
 * \file:	TestListControllerLayer.h
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
//--------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------
class CTestListControllerLayer : public CCLayer
{
public:
	CTestListControllerLayer();
	~CTestListControllerLayer();
public:
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);

	void menuCallback(CCObject * pSender);
	void closeCallback(CCObject * pSender);
private:
	CCPoint m_tBeginPos;
	CCMenu* m_pItemMenu;
};
//--------------------------------------------------------