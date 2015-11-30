/*!
 * \file:	RippleScene.h
 * \author:	FreeKnight
 * \date:	2014-6-26
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "RippleGrid3D.h"
#include "TestScene.h"
//--------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------
class CRippleScene : public CTestScene
{
public:
	CRippleScene();
public:
	void Run();
};
//--------------------------------------------------------
// 分割波纹格横纵向个数
#define RIPPLE_GRID_WIDTH_NUM		240
#define RIPPLE_GRID_HEIGHT_NUM		160
//--------------------------------------------------------
class CRippleLayer : public cocos2d::CCLayer
{
private:
	CCNode*			m_pBackgroundNode;		// 背景图
	CRippleGrid3D*	m_pRippleGrid;			// 表面水波纹格
private:
	unsigned int	m_unGridWidthNum;		// 波纹格横向个数
	unsigned int	m_unGridHeightNum;		// 波纹格纵向个数
public:
	virtual bool	init();

	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	virtual void	update(float dt);
public:
	/**
	@brief:	加入石头 -  创建波纹的函数
	@param:	x,y				波源坐标
	@param: stoneSize		石头大小 [1, max]越大则抖动纹理越深，建议在[4,16]之间效果较佳
	@param: stoneWeight		石头重量 [1, max]越大则抖动越剧烈，建议在[8,64]之间效果较佳
	@remark: 这个函数生成的波源会离原始触摸点有所偏移，若 RIPPLE_GRID_WIDTH， RIPPLE_GRID_HEIGHT 越大，
			则波纹格越细，该偏差值将越小。
	@return:
	*/ 
	void			DropStone(int x, int y, int stoneSize, int stoneWeight);
public:
	CREATE_FUNC(CRippleLayer);
};
//--------------------------------------------------------