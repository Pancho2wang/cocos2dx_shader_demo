/*!
 * \file:	ActiveGrass.h
 * \author:	FreeKnight
 * \date:	2014-6-28
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "TestScene.h"
USING_NS_CC;
//--------------------------------------------------------
class CActiveGrassLayer : public CCLayer 
{
public:
	virtual bool init();
	virtual void update(float delta);
private:
	int		m_nTimeUniformLocation;
	float	m_fTime;
public:
	CREATE_FUNC( CActiveGrassLayer );
};
//--------------------------------------------------------
class CActiveGrassScene : public CTestScene
{
public:
	CActiveGrassScene();
public:
	void Run();
};
//--------------------------------------------------------
