/*!
 * \file:	SpotLight.h
 * \author:	FreeKnight
 * \date:	2014-6-28
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "TestScene.h"
//--------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------
class CSpotLight : public CCSprite
{
private:
	CCRenderTexture*	m_pRenderTex;
	float				m_fSpotLightRadius;
	ccColor4B			m_tagRenderColor;
	GLfloat *			m_pVertices;
	GLfloat *			m_pCoordinates;
	ccColor4B *			m_pColors;
private:
	CSpotLight*		InitWithRenderTexture( CCRenderTexture* pRenderTex,
		float fRadius, ccColor4B tagRenderColor );
	void SpreadOutTimer(float dt);
	void Tick(float dt);
public:
	static CSpotLight* Create( CCRenderTexture* pRenderTex,
		float fRadius, ccColor4B tagRenderColor );
public:
	void RemoveSelf();
};
//--------------------------------------------------------
class CSpotLightLayer : public CCLayer
{
public:
	CREATE_FUNC(CSpotLightLayer);
public:
	virtual bool	init();

	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
private:
	void DelSpotLight();
};
//--------------------------------------------------------
class CSpotLightScene : public CTestScene
{
public:
	CSpotLightScene();
public:
	void Run();
};
//--------------------------------------------------------