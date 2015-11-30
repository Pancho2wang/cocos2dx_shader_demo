/*!
 * \author:	FreeKnight
 * \date:	2014-6-26
 * \comment:
 */
//--------------------------------------------------------
#include "RippleScene.h"
#include "Resource.h"
//--------------------------------------------------------
CRippleScene::CRippleScene()
{
	CRippleLayer *layer = CRippleLayer::create();
	this->addChild(layer);
}
//--------------------------------------------------------
void CRippleScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------
bool CRippleLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	// ����Input��Ϣ
	this->setTouchEnabled(true);
	// ����update
	this->scheduleUpdate();

	// ����Background����ͼ
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pBackgroundNode = CCSprite::create(s_szCommonBackgroundImgPath);
	m_pBackgroundNode->setPosition( ccp(size.width/2, size.height/2) );
	this->addChild(m_pBackgroundNode, 0);

	// ͨ��Grid3D����Grid������
	m_unGridWidthNum = RIPPLE_GRID_WIDTH_NUM;
	m_unGridHeightNum = RIPPLE_GRID_HEIGHT_NUM;
	CCSize m_sGridSize = CCSize(m_unGridWidthNum-1,m_unGridHeightNum-1);
	m_pRippleGrid = CRippleGrid3D::create(m_sGridSize);
	m_pRippleGrid->setActive(true);
	m_pRippleGrid->retain();    

	// ����Grid
	m_pBackgroundNode->setGrid(m_pRippleGrid);

	return true;
}
//--------------------------------------------------------
void CRippleLayer::update(float dt)
{
	m_pRippleGrid->update(dt);
}
//--------------------------------------------------------
void CRippleLayer::DropStone(int x, int y, int stoneSize, int stoneWeight)
{
	m_pRippleGrid->DropStone(x, y, stoneSize + 1, stoneWeight + 1);
}
//--------------------------------------------------------
void CRippleLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(*(pTouches->begin()));
	CCPoint point = touch->getLocation();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int xw = size.width/m_unGridWidthNum;
	int yh = size.height/m_unGridHeightNum;
	int x = point.x/xw;
	int y = point.y/yh;

	// ����õ���Դ���ڸ��ӱ��λ��������Դ
	DropStone(x, y, 16, 48 );
}
//--------------------------------------------------------
void CRippleLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(*(pTouches->begin()));
	CCPoint point = touch->getLocation();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int xw = size.width/m_unGridWidthNum;
	int yh = size.height/m_unGridHeightNum;
	int x = point.x/xw;
	int y = point.y/yh;

	// ����õ���Դ���ڸ��ӱ��λ������С��Դ
	DropStone(x, y, 4, 16);
}
//--------------------------------------------------------
void CRippleLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}
//--------------------------------------------------------
void CRippleLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}
//--------------------------------------------------------