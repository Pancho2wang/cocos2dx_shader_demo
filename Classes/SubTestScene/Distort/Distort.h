/*!
 * \file:	Distort.h
 * \author:	FreeKnight
 * \date:	2014-7-2
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
#define GRID_SIZE_X  64
#define GRID_SIZE_Y  64
#define CLIP_NEAR	(-1024 * CC_CONTENT_SCALE_FACTOR())
#define CLIP_FAR	(1024 * CC_CONTENT_SCALE_FACTOR())
#define SPRING_KS	0.3
#define DRAG		0.5
#define THRESHOLD	2.0
//--------------------------------------------------------
typedef struct {
	float x[3];
	float v[3];
	float t[2];
	int nail;
} MASS;
//--------------------------------------------------------
typedef struct {
	int i, j;
	float r;
} SPRING;
//--------------------------------------------------------
class CDistortLayer : public CCLayer
{
private:
	bool			m_bIsShake;
	bool			m_bIsTakeImage;
	CCTexture2D* 	m_pTexture2D;
	float			m_fMouseX;
	float			m_fMouseY;
	int				m_nGrab;
	MASS *			m_pMass;
	int				m_nSpringCount;
	SPRING *		m_pSpring;
	GLushort		m_usIndices[ GRID_SIZE_X * GRID_SIZE_Y * 6 ];
public:
	CDistortLayer();
	~CDistortLayer();
	virtual bool	init();

	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	virtual void	update(float dt);
	virtual void	draw();
private:
	void			MyInit();
	void			MyRedraw();
	void			MyUpdatePos( int p_nX, int p_nY );
	int				Grab( int p_nX, int p_nY );
public:
	CREATE_FUNC(CDistortLayer);
};
//--------------------------------------------------------
class CDistortScene : public CTestScene
{
public:
	CDistortScene();
public:
	void Run();
};
//--------------------------------------------------------