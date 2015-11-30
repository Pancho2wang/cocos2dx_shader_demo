/*!
 * \file:	RippleGrid3D.h
 * \author:	FreeKnight
 * \date:	2014-6-26
 * \comment: cocos2dxµÄGrid3DWrpper
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//--------------------------------------------------------
#define vertext_len 3
//--------------------------------------------------------
class CRippleGrid3D : public CCGrid3D
{
protected:
	ccVertex3F* m_pBufNext;
	ccVertex3F* m_pBufLast;
	float* m_pBuf;

	unsigned int m_unWidthNum;
	unsigned int m_unHeightNum;
public:
	CRippleGrid3D();
	~CRippleGrid3D();
	bool initWithNum(const CCSize& gridNum);
	static CRippleGrid3D* create(const CCSize& gridNum);
public:
	virtual void update(float dt);
public:
	void DropStone(int x, int y, int stoneSize, int stoneWeight);
};
//--------------------------------------------------------
