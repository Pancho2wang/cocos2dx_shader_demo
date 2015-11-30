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
	@param: p_Bounds ˮ��
	@param: p_nCount ˮƽvectex������Խ��Խ����
	@param: p_fDamping ����
	@param: p_fDiffuse ��ɢ�ٶ�
	*/
	CWave1DNode* initWithBounds( CCRect p_Bounds, int p_nCount, float p_fDamping, float p_fDiffuse );
	static CWave1DNode* create( CCRect p_Bounds, int p_nCount, float p_fDamping, float p_fDiffuse );
	// �����ɽ�Ч��
	void MakeSplashAt( float p_fX );
	// ����
	virtual void draw();
private:
	// ǣ���˶�
	inline void Verlet();
	// ��ɢ�˶�
	inline float Diffuse(float diff, float damp, float prev, float curr, float next);
	// ������ɢ
	void Diffuse();
	// ����������
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