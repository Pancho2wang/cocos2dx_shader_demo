/*!
 * \file:	EmbossSprite.h
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
class CEmbossLayer : public CCLayer 
{
public:
	virtual bool init();
	virtual void update(float delta);
private:
	int		m_nTimeUniformLocation;
	float	m_fTime;
private:
	bool	CreateEmbossSprite();
	bool	CreateColorRampSprite();
public:
	CREATE_FUNC( CEmbossLayer );
};
//--------------------------------------------------------
class CEmbossSpriteScene : public CTestScene
{
public:
	CEmbossSpriteScene();
public:
	void Run();
};
//--------------------------------------------------------
