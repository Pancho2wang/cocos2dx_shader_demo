/*!
 * \file:	LabelShuffleTile.h
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
class CLabelShuffleTileLayer : public CCLayer 
{
public:
	virtual bool init();
	virtual void onEnter();
public:
	void RemoveSprite( CCNode* p );
public:
	CREATE_FUNC( CLabelShuffleTileLayer );
};
//--------------------------------------------------------
class CLabelShuffleTileScene : public CTestScene
{
public:
	CLabelShuffleTileScene();
public:
	void Run();
};
//-------------------------------------------------------
