/*!
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#include "ScratchOff.h"
#include "Resource.h"
//--------------------------------------------------------
bool CScratchOffLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		m_bReset = true;

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite* pBackground = CCSprite::create( s_szScratchOffBkImg );
		pBackground->setPosition( ccp( winSize.width / 2, winSize.height / 2 ) );
		pBackground->setZOrder( 0 );
		pBackground->setTag( 10002 );
		addChild( pBackground );

		m_pBurshSprite = CCSprite::create( s_szScratchBurshImg );
		ccBlendFunc blendFuc = { GL_ZERO, GL_ONE_MINUS_SRC_ALPHA };
		m_pBurshSprite->setBlendFunc( blendFuc );
		m_pBurshSprite->retain();

		m_pScratchFront = CCSprite::create( s_szScratchFrontImg );
		m_pScratchFront->setPosition( ccp( m_pScratchFront->getContentSize().width / 2,
			m_pScratchFront->getContentSize().height / 2 ) );
		m_pScratchFront->retain();

		CCRenderTexture* pScratchLayer = CCRenderTexture::create( 
			m_pScratchFront->getContentSize().width, m_pScratchFront->getContentSize().height );
		pScratchLayer->setPosition( ccp( winSize.width / 2, winSize.height / 2 ) );
		ccBlendFunc newBlendFuc = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
		pScratchLayer->getSprite()->setBlendFunc( newBlendFuc );
		pScratchLayer->setZOrder( 2 );
		pScratchLayer->setTag( 10001 );
		addChild( pScratchLayer );

		// 开启Input消息
		this->setTouchEnabled(true);
		// 开启帧更新
		this->scheduleUpdate();

		return true;
	}while(0);
	return false;
}
//--------------------------------------------------------
void CScratchOffLayer::update(float delta)
{
	CCRenderTexture* pScratchLayer = (CCRenderTexture*)getChildByTag( 10001 );
	if( m_bReset )
	{
		pScratchLayer->clear( 0.0f, 0.0f, 0.0f, 1.0f );

		// 开始重绘
		pScratchLayer->begin();
		m_pScratchFront->visit();
		pScratchLayer->end();

		m_bReset = false;
	}
	// 更新renderTexture
	pScratchLayer->begin();
	glColorMask( 0.0f, 0.0f, 0.0f, 1.0f );
	m_pBurshSprite->visit();
	glColorMask( 1.0f, 1.0f, 1.0f, 1.0f );
	pScratchLayer->end();
}
//--------------------------------------------------------
void CScratchOffLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
	cocos2d::CCPoint touchLocation = pTouch->getLocation();
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_pBurshSprite->setPosition( touchLocation - 
		ccp( winSize.width / 2, winSize.height / 2 )
		+ ccp( m_pScratchFront->getContentSize().width / 2, 
		m_pScratchFront->getContentSize().height / 2 ));
}
//--------------------------------------------------------
void CScratchOffLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
	cocos2d::CCPoint touchLocation = pTouch->getLocation();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_pBurshSprite->setPosition( touchLocation - 
		ccp( winSize.width / 2, winSize.height / 2 )
		+ ccp( m_pScratchFront->getContentSize().width / 2, 
		m_pScratchFront->getContentSize().height / 2 ));
}
//--------------------------------------------------------
void CScratchOffLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------
void CScratchOffLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------
// CScratchOffScene
//--------------------------------------------------------
CScratchOffScene::CScratchOffScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CScratchOffLayer* pScratchOffLayer = (CScratchOffLayer*)CScratchOffLayer::create();
	if( pScratchOffLayer )
	{
		this->addChild( pScratchOffLayer );
	}
}
//--------------------------------------------------------
void CScratchOffScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------