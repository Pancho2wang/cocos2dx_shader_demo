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
// �ָ�Ƹ���������
#define RIPPLE_GRID_WIDTH_NUM		240
#define RIPPLE_GRID_HEIGHT_NUM		160
//--------------------------------------------------------
class CRippleLayer : public cocos2d::CCLayer
{
private:
	CCNode*			m_pBackgroundNode;		// ����ͼ
	CRippleGrid3D*	m_pRippleGrid;			// ����ˮ���Ƹ�
private:
	unsigned int	m_unGridWidthNum;		// ���Ƹ�������
	unsigned int	m_unGridHeightNum;		// ���Ƹ��������
public:
	virtual bool	init();

	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	virtual void	update(float dt);
public:
	/**
	@brief:	����ʯͷ -  �������Ƶĺ���
	@param:	x,y				��Դ����
	@param: stoneSize		ʯͷ��С [1, max]Խ���򶶶�����Խ�������[4,16]֮��Ч���ϼ�
	@param: stoneWeight		ʯͷ���� [1, max]Խ���򶶶�Խ���ң�������[8,64]֮��Ч���ϼ�
	@remark: ����������ɵĲ�Դ����ԭʼ����������ƫ�ƣ��� RIPPLE_GRID_WIDTH�� RIPPLE_GRID_HEIGHT Խ��
			���Ƹ�Խϸ����ƫ��ֵ��ԽС��
	@return:
	*/ 
	void			DropStone(int x, int y, int stoneSize, int stoneWeight);
public:
	CREATE_FUNC(CRippleLayer);
};
//--------------------------------------------------------