/*!
 * \file:	SuckAction.h
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
class CSuckAction : public CCGrid3DAction
{
public:
	static CSuckAction* create( CCSize p_tagSize, float p_fTime, CCPoint p_tagPos ); 
	CSuckAction* initWithGrid( CCSize p_tagSize, float p_fTime, CCPoint p_tagPos );
	virtual void update(float time);
private:
	CCPoint m_tagToPos;
	float	m_fAmplitudeRate;
};
//--------------------------------------------------------
class CSuckScene : public CTestScene
{
public:
	CSuckScene();
public:
	void Run();
	void RemoveSprite( CCNode* p );
};
//--------------------------------------------------------