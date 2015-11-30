/*!
 * \file:	Wave1D.h
 * \author:	FreeKnight
 * \date:	2014-7-2
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "TestScene.h"
USING_NS_CC;
//--------------------------------------------------------
class CWave1DNode : public CCNode
{
public:
	CWave1DNode();
	~CWave1DNode();
	/**
	@param: p_Bounds 水块
	@param: p_nCount 水平vectex数量，越大越逼真
	@param: p_fDamping 阻尼
	@param: p_fDiffuse 扩散速度
	*/
	CWave1DNode* initWithBounds( CCRect p_Bounds, int p_nCount, float p_fDamping, float p_fDiffuse );
	static CWave1DNode* create( CCRect p_Bounds, int p_nCount, float p_fDamping, float p_fDiffuse );
	// 产生飞溅效果
	void MakeSplashAt( float p_fX );
	// 重载
	virtual void draw();
private:
	// 牵引运动
	inline void Verlet();
	// 扩散运动
	inline float Diffuse(float diff, float damp, float prev, float curr, float next);
	// 阻尼扩散
	void Diffuse();
	// 相邻两个点
	float Dx();
private:
	CCRect		m_Bounds;
	int			m_nCount;
	float		m_fDamping;
	float		m_fDiffusion;
	float*		m_pH1;
	float*		m_pH2;
};
//--------------------------------------------------------
class CWave1DNodeLayer : public CCLayer
{
public:
	CREATE_FUNC(CWave1DNodeLayer);
public:
	virtual bool	init();

	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
};
//--------------------------------------------------------
class CWave1DNodeScene : public CTestScene
{
public:
	CWave1DNodeScene();
public:
	void Run();
};
//--------------------------------------------------------