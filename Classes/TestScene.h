/*!
 * \file:	TestScene.h
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "VisibleRect.h"
//--------------------------------------------------------
USING_NS_CC;
using namespace std;
//--------------------------------------------------------
class CTestScene : public CCScene
{
public: 
	CTestScene(bool bPortrait = false);
public:
	virtual void onEnter();
	virtual void Run() = 0;
	// The CallBack for back to the main menu scene
	virtual void MainMenuCallback(CCObject* pSender);
};
//--------------------------------------------------------
typedef CCLayer* (*NEWTESTFUNC)();
//--------------------------------------------------------
#define TESTLAYER_CREATE_FUNC(className) \
	static CCLayer* create##className() \
{ return new className(); }
//--------------------------------------------------------
#define CF(className) create##className
//--------------------------------------------------------