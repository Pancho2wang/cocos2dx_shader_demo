/*!
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#include "LaserLine.h"
#include "Resource.h"
#include "Common/FKCommon.h"
#include "Common/StringConvert.h"
//--------------------------------------------------------
bool CLaserLineLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCLabelTTF* pLabel = CCLabelTTF::create(
			FKStringConvert::a2u("发射点").c_str(),
			"Arial", 36 );
		pLabel->setColor( ccRED );
		pLabel->setPosition( ccp( winSize.width / 2, 300) );
		pLabel->setTag( 10001 );
		addChild( pLabel );

		// 开启Input消息
		this->setTouchEnabled(true);
		return true;
	}while(0);
	return false;
}
//--------------------------------------------------------
void CLaserLineLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
	cocos2d::CCPoint touchLocation = pTouch->getLocation();

	CCLabelTTF* pLabel = (CCLabelTTF*)getChildByTag( 10001 );
	CCPoint tagSrcPoint = pLabel->getPosition();

	float dis = ccpDistance(tagSrcPoint, touchLocation);
	float offx = touchLocation.x - tagSrcPoint.x;
	float offy = touchLocation.y - tagSrcPoint.y;
	float angleRadians = atanf((offy) / (offx));

	float angleDegree = CC_RADIANS_TO_DEGREES(angleRadians);
	if( offx  <  0 )
	{
		angleDegree += 180;
	}

	CCSprite* pSprite = CCSprite::create( s_szLaserLineImg );
	CCProgressTimer* pLaserSprite = CCProgressTimer::create( pSprite );
	pLaserSprite->setType( kCCProgressTimerTypeBar );
	pLaserSprite->setAnchorPoint( CCPointZero );
	pLaserSprite->setScaleX( dis / 20.0f );
	pLaserSprite->setScaleY( 0.5f );
	pLaserSprite->setRotation( -1 * angleDegree );
	pLaserSprite->setPosition( tagSrcPoint );
	addChild( pLaserSprite );

	CCProgressTo * pTo = CCProgressTo::create( 0.2f, 100.0f );
	CCSpawn* pStart = CCSpawn::create( pTo, CCFadeIn::create( 0.2f ), NULL );
	pLaserSprite->runAction( CCSequence::create( pStart, 
		CCDelayTime::create( 0.5f ),
		CCScaleTo::create(0.3f, 1.0f, 0.1f ),
		CCCallFuncN::create( this, callfuncN_selector(CLaserLineLayer::RemoveSprite )),
		NULL ));

	return;
}
//--------------------------------------------------------
void CLaserLineLayer::RemoveSprite( CCNode* p )
{
	if( p != NULL )
	{
		p->removeAllChildren();
		p->removeFromParentAndCleanup( true );
	}
}
//--------------------------------------------------------
void CLaserLineLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------
void CLaserLineLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------
void CLaserLineLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------
// CLaserLineScene
//--------------------------------------------------------
CLaserLineScene::CLaserLineScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CLaserLineLayer* pLaserLineLayer = (CLaserLineLayer*)CLaserLineLayer::create();
	if( pLaserLineLayer )
	{
		this->addChild( pLaserLineLayer );
	}
}
//--------------------------------------------------------
void CLaserLineScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------